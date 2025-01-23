#include "Logic.h"
#include <libultraship/libultraship.h>
#include "2s2h/Rando/Types.h"

#include <numeric>
#include <iterator>

extern "C" {
#include "variables.h"
#include "ShipUtils.h"
uint64_t GetUnixTimestamp();
}

namespace Rando {

namespace Logic {

void ApplyGlitchlessLogicToSaveContext() {
    uint64_t tick = GetUnixTimestamp();

    // TODO: This should be decided by the player from the UI
    std::vector<RandoItemId> startingItems = {
        RI_SWORD_KOKIRI,
        RI_SHIELD_HERO,
        RI_OCARINA,
        RI_SONG_TIME,
    };

    // Grant the starting items
    for (RandoItemId startingItem : startingItems) {
        GiveItem(ConvertItem(startingItem));
    }

    SaveContext copiedSaveContext;
    memcpy(&copiedSaveContext, &gSaveContext, sizeof(SaveContext));

    std::vector<RandoItemId> itemPool;
    std::unordered_map<RandoCheckId, bool> checkPool;

    std::set<RandoRegionId> regionsInLogic = { RR_MAX };
    std::unordered_map<RandoCheckId, bool> checksInLogic;
    std::set<RandoEvent*> eventsInLogic;

    RandoCheckId checkWithJunk = RC_UNKNOWN;
    std::set<RandoItemId> nonJunkItemsThatWeHaveTried;
    std::vector<RandoCheckId> checksWithJunk;
    std::vector<int> checksWithJunkWeights;
    int weight = 1;

    // First loop through all regions and add checks/items to the pool
    for (auto& [randoRegionId, randoRegion] : Rando::Logic::Regions) {
        for (auto& [randoCheckId, _] : randoRegion.checks) {
            auto& randoStaticCheck = Rando::StaticData::Checks[randoCheckId];
            // Skip checks that are already in the pool
            if (checkPool.find(randoCheckId) != checkPool.end()) {
                continue;
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_SKULL_TOKEN &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_GOLD_SKULLTULAS] == RO_GENERIC_NO) {
                continue;
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_OWL &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_OWL_STATUES] == RO_GENERIC_NO) {
                continue;
            }

            if ((randoStaticCheck.randoCheckType == RCTYPE_POT || randoStaticCheck.randoCheckType == RCTYPE_BARREL ||
                 randoStaticCheck.randoCheckType == RCTYPE_CRATE ||
                 randoStaticCheck.randoCheckType == RCTYPE_FREESTANDING) &&
                RANDO_SAVE_OPTIONS[RO_SHUFFLE_MUNDANE] == RO_GENERIC_NO) {
                continue;
            }

            if (randoStaticCheck.randoCheckType == RCTYPE_SHOP) {
                if (RANDO_SAVE_OPTIONS[RO_SHUFFLE_SHOPS] == RO_GENERIC_NO &&
                    randoCheckId != RC_CURIOSITY_SHOP_SPECIAL_ITEM &&
                    randoCheckId != RC_BOMB_SHOP_ITEM_04_OR_CURIOSITY_SHOP_ITEM) {
                    continue;
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
                continue;
            }

            checkPool.insert({ randoCheckId, true });
            itemPool.push_back(randoStaticCheck.randoItemId);
        }
    }

    // Add sword and shield to the pool if they aren't in the starting items
    if (std::find(startingItems.begin(), startingItems.end(), RI_SWORD_KOKIRI) == startingItems.end()) {
        itemPool.push_back(RI_SWORD_KOKIRI);
    }
    if (std::find(startingItems.begin(), startingItems.end(), RI_SHIELD_HERO) == startingItems.end()) {
        itemPool.push_back(RI_SHIELD_HERO);
    }

    // Add other items that don't have a vanilla location like Sun's Song or Song of Double Time

    // Remove starting items from the pool (but only one per entry in startingItems)
    for (RandoItemId startingItem : startingItems) {
        auto it = std::find(itemPool.begin(), itemPool.end(), startingItem);
        if (it != itemPool.end()) {
            itemPool.erase(it);
        }
    }

    if (checkPool.empty()) {
        throw std::runtime_error("No checks in logic");
    }
    if (itemPool.empty()) {
        throw std::runtime_error("No items in logic");
    }
    // Add/Remove junk items to/from the pool to make the item pool size match the check pool size
    while (checkPool.size() != itemPool.size()) {
        if (checkPool.size() > itemPool.size()) {
            itemPool.push_back(RI_JUNK);
        } else {
            for (int i = 0; i < itemPool.size(); i++) {
                if (Rando::StaticData::Items[itemPool[i]].randoItemType == RITYPE_JUNK) {
                    itemPool.erase(itemPool.begin() + i);
                    break;
                }
            }
        }
    }

    // Inital shuffle of the item pool (Following shuffles done at the end of the loop)
    if (itemPool.size() > 1) {
        for (size_t i = 0; i < itemPool.size(); i++) {
            size_t j = Ship_Random(0, itemPool.size() - 1);
            std::swap(itemPool[i], itemPool[j]);
        }
    }

    auto handleError = [&](std::string message) {
        SPDLOG_ERROR("Items/Checks: {}/{}", itemPool.size(), checkPool.size());

        // Log out the checks that are still in the pool
        for (auto& [randoCheckId, _] : checkPool) {
            SPDLOG_ERROR("Check still in pool: {}", Rando::StaticData::Checks[randoCheckId].name);
        }
        // Log out the items that are still in the pool
        for (RandoItemId randoItemId : itemPool) {
            SPDLOG_ERROR("Item still in pool: {}", Rando::StaticData::Items[randoItemId].spoilerName);
        }

        memcpy(&gSaveContext, &copiedSaveContext, sizeof(SaveContext));
        throw std::runtime_error(message);
    };

    while (true) {
        // Break if we've been running for too long
        if (GetUnixTimestamp() - tick > 10000) {
            handleError("Logic Generation Timeout");
        }

        bool regionsInLogicChanged = false;
        bool eventsInLogicChanged = false;
        bool checksInLogicChanged = false;

        // Crawl through all reachable regions and add any new reachable regions
        auto prevRegionsInLogicSize = regionsInLogic.size();
        for (RandoRegionId regionId : regionsInLogic) {
            FindReachableRegions(regionId, regionsInLogic);
        }
        if (regionsInLogic.size() != prevRegionsInLogicSize) {
            regionsInLogicChanged = true;
        }

        for (RandoRegionId regionId : regionsInLogic) {
            auto& randoRegion = Regions[regionId];

            // Apply any new events
            for (auto& randoEvent : randoRegion.events) {
                if (!eventsInLogic.contains(&randoEvent) && randoEvent.condition()) {
                    randoEvent.onApply();
                    eventsInLogic.insert(&randoEvent);
                    SPDLOG_TRACE("Event: {}", randoEvent.name);
                    eventsInLogicChanged = true;
                }
            }

            // Apply any new checks
            for (auto& [randoCheckId, checkLogic] : randoRegion.checks) {
                if (checksInLogic.find(randoCheckId) == checksInLogic.end() && checkLogic.first()) {
                    bool isShuffled = checkPool.find(randoCheckId) != checkPool.end();
                    checksInLogic.insert({ randoCheckId, isShuffled });
                    checkPool.erase(randoCheckId);

                    RandoItemId randoItemId;

                    if (isShuffled) {
                        randoItemId = itemPool.back();
                        itemPool.pop_back();

                        if (Rando::StaticData::Items[randoItemId].randoItemType == RITYPE_JUNK ||
                            Rando::StaticData::Items[randoItemId].randoItemType == RITYPE_HEALTH) {
                            checksWithJunk.push_back(randoCheckId);
                            checksWithJunkWeights.push_back(weight);
                        }
                        SPDLOG_TRACE("Check: {}:{}", Rando::StaticData::Checks[randoCheckId].name,
                                     Rando::StaticData::Items[randoItemId].spoilerName);
                    } else {
                        randoItemId = Rando::StaticData::Checks[randoCheckId].randoItemId;
                    }

                    RANDO_SAVE_CHECKS[randoCheckId].randoItemId = randoItemId;
                    RANDO_SAVE_CHECKS[randoCheckId].shuffled = isShuffled;
                    GiveItem(ConvertItem(randoItemId));
                    checksInLogicChanged = true;
                }
            }
        }

        if (itemPool.empty()) {
            // Done!
            break;
        }

        // Choose a random check with junk, and attempt to place progressive items until we unlock something
        if (!regionsInLogicChanged && !checksInLogicChanged && !eventsInLogicChanged) {
            if (checkWithJunk == RC_UNKNOWN) {
                if (checksWithJunk.empty()) {
                    handleError("No checks with junk, not sure what to do");
                }

                if (checksWithJunk.size() == 1) {
                    checkWithJunk = checksWithJunk[0];
                } else {
                    std::vector<double> cumulativeWeights(checksWithJunkWeights.size());
                    std::partial_sum(checksWithJunkWeights.begin(), checksWithJunkWeights.end(),
                                     cumulativeWeights.begin());
                    double random = Ship_Random(0, cumulativeWeights.back());
                    auto it = std::lower_bound(cumulativeWeights.begin(), cumulativeWeights.end(), random);
                    size_t index = std::distance(cumulativeWeights.begin(), it);

                    checkWithJunk = checksWithJunk[index];

                    // Remove the check from the list of checks with junk
                    checksWithJunk.erase(checksWithJunk.begin() + index);
                    checksWithJunkWeights.erase(checksWithJunkWeights.begin() + index);
                }
            }

            std::vector<std::pair<RandoItemId, int>> nonJunkItemsThatWeHaveNotTried;
            bool anyNonJunkItemsLeft = false;
            for (size_t i = 0; i < itemPool.size(); i++) {
                if (Rando::StaticData::Items[itemPool[i]].randoItemType != RITYPE_JUNK &&
                    Rando::StaticData::Items[itemPool[i]].randoItemType != RITYPE_HEALTH) {
                    anyNonJunkItemsLeft = true;
                    if (nonJunkItemsThatWeHaveTried.find(itemPool[i]) == nonJunkItemsThatWeHaveTried.end()) {
                        nonJunkItemsThatWeHaveNotTried.push_back({ itemPool[i], i });
                    }
                }
            }

            if (!anyNonJunkItemsLeft) {
                handleError("No non-junk items left");
            }

            if (nonJunkItemsThatWeHaveNotTried.empty()) {
                SPDLOG_TRACE("Already tried all non-junk items, leaving the last non-junk item in place: {}: {}",
                             Rando::StaticData::Checks[checkWithJunk].name,
                             Rando::StaticData::Items[RANDO_SAVE_CHECKS[checkWithJunk].randoItemId].spoilerName);
                checkWithJunk = RC_UNKNOWN;
                nonJunkItemsThatWeHaveTried.clear();
                continue;
            }

            // Remove item and place it back in the pool
            RandoItemId oldRandoItemId = RANDO_SAVE_CHECKS[checkWithJunk].randoItemId;
            auto& [newRandoItemId, indexInPool] = nonJunkItemsThatWeHaveNotTried[0];

            RANDO_SAVE_CHECKS[checkWithJunk].randoItemId = newRandoItemId;

            RemoveItem(oldRandoItemId);
            GiveItem(ConvertItem(newRandoItemId));

            itemPool.erase(itemPool.begin() + indexInPool);
            itemPool.push_back(oldRandoItemId);

            nonJunkItemsThatWeHaveTried.insert(newRandoItemId);
            SPDLOG_TRACE("Attempting to replaced junk item: {}:{}", Rando::StaticData::Checks[checkWithJunk].name,
                         Rando::StaticData::Items[newRandoItemId].spoilerName);
        } else {
            weight++;
            if (checkWithJunk != RC_UNKNOWN) {
                SPDLOG_TRACE("Successfully Replaced junk item with: {}:{}",
                             Rando::StaticData::Checks[checkWithJunk].name,
                             Rando::StaticData::Items[RANDO_SAVE_CHECKS[checkWithJunk].randoItemId].spoilerName);
            }
            checkWithJunk = RC_UNKNOWN;
            nonJunkItemsThatWeHaveTried.clear();

            // Shuffle the item pool
            if (itemPool.size() > 1) {
                for (size_t i = 0; i < itemPool.size(); i++) {
                    size_t j = Ship_Random(0, itemPool.size() - 1);
                    std::swap(itemPool[i], itemPool[j]);
                }
            }
        }
    }

    for (auto& [randoCheckId, isShuffled] : checksInLogic) {
        copiedSaveContext.save.shipSaveInfo.rando.randoSaveChecks[randoCheckId].randoItemId =
            RANDO_SAVE_CHECKS[randoCheckId].randoItemId;
        copiedSaveContext.save.shipSaveInfo.rando.randoSaveChecks[randoCheckId].shuffled = isShuffled;
    }

    memcpy(&gSaveContext, &copiedSaveContext, sizeof(SaveContext));

    SPDLOG_INFO("Successfully placed all items with Glitchless logic");
}

} // namespace Logic

} // namespace Rando
