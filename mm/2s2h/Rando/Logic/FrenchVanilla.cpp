#include "Logic.h"
#include <libultraship/libultraship.h>
#include "2s2h/Rando/Types.h"

extern "C" {
#include "variables.h"
#include "ShipUtils.h"
uint64_t GetUnixTimestamp();
}

namespace Rando {

namespace Logic {

struct RandoPoolEntry {
    bool shuffled;
    RandoItemId vanillaItemId;
    RandoItemId placedItemId;
    bool itemPlaced;
    bool checkFilled;
    bool inPool;
};

struct RandoPoolPlacement {
    RandoCheckId randoCheckId;
    RandoCheckId randoCheckIdFromItem;
    RandoItemId placedItemId;
};

void ApplyFrenchVanillaLogicToSaveContext() {
    uint64_t tick = GetUnixTimestamp();
    std::set<RandoCheckId> allChecksThatAreInLogic;
    std::set<RandoCheckId> allChecksThatHaveBeenReachedAtLeastOnce;

    // Used across all iterations
    std::unordered_map<RandoCheckId, RandoPoolEntry> currentCheckPool;
    std::set<RandoRegionId> currentReachableRegions = { RR_MAX };
    std::set<RandoEvent*> currentEventsTriggered;

    // Used for backtracking
    std::vector<RandoPoolPlacement> placements;
    std::vector<RandoRegionId> newlyAccessibleRegions;
    std::vector<int> amountOfNewlyAccessibleRegions;
    std::vector<RandoCheckId> newlyAccessibleChecks;
    std::vector<int> amountOfNewlyAccessibleChecks;
    std::vector<RandoEvent*> newlyTriggeredEvents;
    std::vector<int> amountOfNewlyTriggeredEvents;

    // TODO: This should be decided by the player from the UI
    std::vector<RandoItemId> startingItems = {
        RI_OCARINA,
        RI_SONG_TIME,
        // French Vanilla not compatible with starting without these items
        RI_SWORD_KOKIRI,
        RI_SHIELD_HERO,
    };

    // Grant the starting items
    for (RandoItemId startingItem : startingItems) {
        GiveItem(ConvertItem(startingItem));
    }

    SaveContext copiedSaveContext;
    memcpy(&copiedSaveContext, &gSaveContext, sizeof(SaveContext));

    // First loop through all regions and add checks/items to the pool
    for (auto& [randoRegionId, randoRegion] : Rando::Logic::Regions) {
        for (auto& [randoCheckId, _] : randoRegion.checks) {
            auto& randoStaticCheck = Rando::StaticData::Checks[randoCheckId];
            bool isShuffled = true;

            if (randoStaticCheck.randoCheckType == RCTYPE_SKULL_TOKEN &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_GOLD_SKULLTULAS] == RO_GENERIC_NO) {
                isShuffled = false;
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_OWL &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_OWL_STATUES] == RO_GENERIC_NO) {
                isShuffled = false;
            }

            if ((randoStaticCheck.randoCheckType == RCTYPE_POT || randoStaticCheck.randoCheckType == RCTYPE_BARREL ||
                 randoStaticCheck.randoCheckType == RCTYPE_CRATE ||
                 randoStaticCheck.randoCheckType == RCTYPE_FREESTANDING) &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_MUNDANE] == RO_GENERIC_NO) {
                isShuffled = false;
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_SHOP) {
                if (RANDO_SAVE_OPTIONS[RO_SHUFFLE_SHOPS] == RO_GENERIC_NO &&
                    randoCheckId != RC_CURIOSITY_SHOP_SPECIAL_ITEM &&
                    randoCheckId != RC_BOMB_SHOP_ITEM_04_OR_CURIOSITY_SHOP_ITEM) {
                    isShuffled = false;
                } else {
                    int price = Ship_Random(0, 200);
                    // We need the price to be saved in the current save context for logic, as well as the backed
                    // up context that will be used in the actual playthrough
                    RANDO_SAVE_CHECKS[randoCheckId].price = price;
                    copiedSaveContext.save.shipSaveInfo.rando.randoSaveChecks[randoCheckId].price = price;
                }
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_REMAINS &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_BOSS_REMAINS] == RO_GENERIC_NO) {
                isShuffled = false;
            }

            allChecksThatAreInLogic.insert(randoCheckId);
            currentCheckPool[randoCheckId] = {
                isShuffled, randoStaticCheck.randoItemId, RI_UNKNOWN, false, false, false
            };
        }
    }

    while (true) {
        // Break if we've been running for too long
        if (GetUnixTimestamp() - tick > 5000) {
            tick = GetUnixTimestamp();

            // Log checks that have never been reached
            std::vector<RandoCheckId> checksThatHaveNeverBeenReached;

            std::set_difference(allChecksThatAreInLogic.begin(), allChecksThatAreInLogic.end(),
                                allChecksThatHaveBeenReachedAtLeastOnce.begin(),
                                allChecksThatHaveBeenReachedAtLeastOnce.end(),
                                std::inserter(checksThatHaveNeverBeenReached, checksThatHaveNeverBeenReached.begin()));

            SPDLOG_ERROR("Checks that have never been reached:");
            for (RandoCheckId randoCheckId : checksThatHaveNeverBeenReached) {
                SPDLOG_ERROR("{}", Rando::StaticData::Checks[randoCheckId].name);
            }

            throw std::runtime_error("Logic Generation Timeout");
        }

        // Crawl through all reachable regions and add any new reachable regions
        int currentAmountOfNewlyAccessibleRegions = newlyAccessibleRegions.size();
        std::set<RandoRegionId> currentReachableRegionsCopy = currentReachableRegions;
        for (RandoRegionId regionId : currentReachableRegions) {
            FindReachableRegions(regionId, currentReachableRegions);
        }
        // Difference between the new and old reachable regions
        std::set_difference(currentReachableRegions.begin(), currentReachableRegions.end(),
                            currentReachableRegionsCopy.begin(), currentReachableRegionsCopy.end(),
                            std::inserter(newlyAccessibleRegions,
                                          newlyAccessibleRegions.begin() + currentAmountOfNewlyAccessibleRegions));
        amountOfNewlyAccessibleRegions.push_back(newlyAccessibleRegions.size() - currentAmountOfNewlyAccessibleRegions);

        // Track newly accessible checks
        int currentAmountOfNewlyAccessibleChecks = newlyAccessibleChecks.size();
        for (RandoRegionId regionId : currentReachableRegions) {
            auto& randoRegion = Rando::Logic::Regions[regionId];
            for (auto& [randoCheckId, accessLogicFunc] : randoRegion.checks) {
                if (
                    // Check is not already in the pool
                    currentCheckPool[randoCheckId].inPool == false &&
                    // Check is accessible
                    accessLogicFunc.first()) {
                    allChecksThatHaveBeenReachedAtLeastOnce.insert(randoCheckId);
                    currentCheckPool[randoCheckId].inPool = true;
                    newlyAccessibleChecks.push_back(randoCheckId);
                }
            }
        }
        amountOfNewlyAccessibleChecks.push_back(newlyAccessibleChecks.size() - currentAmountOfNewlyAccessibleChecks);

        // Track newly triggered events
        int currentAmountOfNewlyTriggeredEvents = newlyTriggeredEvents.size();
        for (RandoRegionId regionId : currentReachableRegions) {
            auto& randoRegion = Rando::Logic::Regions[regionId];
            for (auto& randoEvent : randoRegion.events) {
                if (
                    // Event is not already triggered
                    !currentEventsTriggered.contains(&randoEvent) &&
                    // Event condition is met
                    randoEvent.condition()) {
                    currentEventsTriggered.insert(&randoEvent);
                    newlyTriggeredEvents.push_back(&randoEvent);
                    randoEvent.onApply();
                }
            }
        }
        amountOfNewlyTriggeredEvents.push_back(newlyTriggeredEvents.size() - currentAmountOfNewlyTriggeredEvents);

        // Determine if we have placed all items
        int checksLeft = 0;
        std::vector<RandoCheckId> currentCheckList;
        std::vector<std::pair<RandoItemId, RandoCheckId>> currentItemList;
        for (auto& [randoCheckId, randoPoolEntry] : currentCheckPool) {
            if (randoPoolEntry.inPool) {
                if (!randoPoolEntry.itemPlaced && randoPoolEntry.shuffled) {
                    currentItemList.push_back({ randoPoolEntry.vanillaItemId, randoCheckId });
                }
                if (!randoPoolEntry.checkFilled) {
                    currentCheckList.push_back(randoCheckId);
                }
            }
            if (!randoPoolEntry.checkFilled) {
                checksLeft++;
            }
        }
        if (checksLeft == 0) {
            break; // All items placed
        }

        // If there are no items to place, backtrack
        if (currentCheckList.size() == 0) {
            for (int i = 0; i < amountOfNewlyAccessibleRegions.back(); i++) {
                currentReachableRegions.erase(newlyAccessibleRegions.back());
                newlyAccessibleRegions.pop_back();
            }
            amountOfNewlyAccessibleRegions.pop_back();
            for (int i = 0; i < amountOfNewlyAccessibleChecks.back(); i++) {
                currentCheckPool[newlyAccessibleChecks.back()].inPool = false;
                newlyAccessibleChecks.pop_back();
            }
            amountOfNewlyAccessibleChecks.pop_back();
            for (int i = 0; i < amountOfNewlyTriggeredEvents.back(); i++) {
                newlyTriggeredEvents.back()->onRemove();
                currentEventsTriggered.erase(newlyTriggeredEvents.back());
                newlyTriggeredEvents.pop_back();
            }
            amountOfNewlyTriggeredEvents.pop_back();
            auto [randoCheckId, randoCheckIdFromItem, convertedItemId] = placements.back();
            currentCheckPool[randoCheckId].checkFilled = false;
            currentCheckPool[randoCheckId].placedItemId = RI_UNKNOWN;
            currentCheckPool[randoCheckIdFromItem].itemPlaced = false;
            RemoveItem(convertedItemId);
            placements.pop_back();
            continue;
        }

        // Select a random check
        size_t checkIndex = currentCheckList.size() > 1 ? Ship_Random(0, currentCheckList.size() - 1) : 0;
        RandoCheckId randoCheckId = currentCheckList[checkIndex];
        if (currentCheckPool[randoCheckId].shuffled) {
            // Select a random item
            size_t itemIndex = currentItemList.size() > 1 ? Ship_Random(0, currentItemList.size() - 1) : 0;
            auto [randoItemId, randoCheckIdFromItem] = currentItemList[itemIndex];
            SPDLOG_TRACE("Placing item {} in check {}", Rando::StaticData::Items[randoItemId].spoilerName,
                         Rando::StaticData::Checks[randoCheckId].name);
            // Place the item in the check
            currentCheckPool[randoCheckId].checkFilled = true;
            currentCheckPool[randoCheckId].placedItemId = randoItemId;
            currentCheckPool[randoCheckIdFromItem].itemPlaced = true;
            RandoItemId convertedItemId = ConvertItem(randoItemId);
            GiveItem(convertedItemId);
            placements.push_back({ randoCheckId, randoCheckIdFromItem, convertedItemId });
        } else {
            // Place vanilla item in the check
            currentCheckPool[randoCheckId].checkFilled = true;
            currentCheckPool[randoCheckId].placedItemId = currentCheckPool[randoCheckId].vanillaItemId;
            currentCheckPool[randoCheckId].itemPlaced = true;
            RandoItemId convertedItemId = ConvertItem(currentCheckPool[randoCheckId].vanillaItemId);
            GiveItem(convertedItemId);
            placements.push_back({ randoCheckId, randoCheckId, convertedItemId });
        }
    }

    memcpy(&gSaveContext, &copiedSaveContext, sizeof(SaveContext));

    for (auto& [randoCheckId, randoPoolEntry] : currentCheckPool) {
        if (randoPoolEntry.shuffled) {
            RANDO_SAVE_CHECKS[randoCheckId].randoItemId = randoPoolEntry.placedItemId;
            RANDO_SAVE_CHECKS[randoCheckId].shuffled = true;
        }
    }

    SPDLOG_INFO("Successfully placed all items with French Vanilla logic");
}

} // namespace Logic

} // namespace Rando
