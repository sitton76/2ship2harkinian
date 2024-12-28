#include <libultraship/libultraship.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_CUCCO_SHACK] = RandoRegion{ .sceneId = SCENE_F01C,
        .checks = {
            CHECK(RC_ROMANI_RANCH_GROG, HAS_ITEM(ITEM_MASK_BREMEN)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROMANI_RANCH, 4),                 ENTRANCE(CUCCO_SHACK, 0), true),
        },
    };
    Regions[RR_DEKU_KINGS_CHAMBER_HOLDING_CELL] = RandoRegion{ .name = "Holding Cell", .sceneId = SCENE_DEKU_KING,
        .checks = {
            CHECK(RC_DEKU_KINGS_CHAMBER_MONKEY, CAN_BE_DEKU && HAS_ITEM(ITEM_OCARINA_OF_TIME)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(DEKU_PALACE, 3),                  ENTRANCE(DEKU_KINGS_CHAMBER, 1), true),
        },
    };
    Regions[RR_DEKU_KINGS_CHAMBER] = RandoRegion{ .sceneId = SCENE_DEKU_KING,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(DEKU_PALACE, 2),                  ENTRANCE(DEKU_KINGS_CHAMBER, 0), true),
        },
    };
    Regions[RR_DEKU_PALACE_INSIDE] = RandoRegion{ .name = "Inside", .sceneId = SCENE_22DEKUCITY,
        .checks = {
            CHECK(RC_DEKU_PALACE_HP, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(DEKU_KINGS_CHAMBER, 0),           ENTRANCE(DEKU_PALACE, 2), true),
            EXIT(ENTRANCE(DEKU_KINGS_CHAMBER, 1),           ENTRANCE(DEKU_PALACE, 3), CAN_BE_DEKU), // Cell TODO: Is there something to do with beans here?
        },
        .connections = {
            CONNECTION(RR_DEKU_PALACE_OUTSIDE, true),
        },
    };
    Regions[RR_DEKU_PALACE_OUTSIDE] = RandoRegion{ .name = "Outside", .sceneId = SCENE_22DEKUCITY,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 3),      ENTRANCE(DEKU_PALACE, 0), true),
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 4),      ENTRANCE(DEKU_PALACE, 5), CAN_BE_DEKU), // Treetop
            EXIT(ENTRANCE(DEKU_SHRINE, 0),                  ENTRANCE(DEKU_PALACE, 4), CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)),
        },
        .connections = {
            CONNECTION(RR_DEKU_PALACE_INSIDE, CAN_BE_DEKU),
        },
    };
    Regions[RR_DEKU_SHRINE] = RandoRegion{ .sceneId = SCENE_DANPEI,
        .checks = {
            // Commenting out checks because I'm not sure on the logic. 
            // Glitchless logic doesn't finish when uncommented
            // CHECK(RC_DEKU_SHRINE_POT_1, CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)),
            // CHECK(RC_DEKU_SHRINE_POT_2, CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(DEKU_PALACE, 4),                  ENTRANCE(DEKU_SHRINE, 0), true),
        },
    };
    Regions[RR_DOGGY_RACETRACK] = RandoRegion{ .sceneId = SCENE_F01_B,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROMANI_RANCH, 5),                 ENTRANCE(DOGGY_RACETRACK, 0), true),
        },
    };
    Regions[RR_GORMAN_TRACK] = RandoRegion{ .sceneId = SCENE_KOEPONARACE,
        .checks = {
            // TODO : Uncomment this out once the Alien section is implemented.
            //CHECK(RC_CREMIA_ESCORT, CHECK_WEEKEVENTREG(WEEKEVENTREG_DEFENDED_AGAINST_THEM))
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(MILK_ROAD, 2),                    ENTRANCE(GORMAN_TRACK, 3), CAN_PLAY_SONG(EPONA)),
            EXIT(ENTRANCE(MILK_ROAD, 3),                    ENTRANCE(GORMAN_TRACK, 0), true),
        },
    };
    Regions[RR_MAGIC_HAGS_POTION_SHOP] = RandoRegion{ .sceneId = SCENE_WITCH_SHOP,
        .checks = {
            CHECK(RC_HAGS_POTION_SHOP_ITEM_1, CAN_AFFORD(RC_HAGS_POTION_SHOP_ITEM_1)),
            CHECK(RC_HAGS_POTION_SHOP_ITEM_2, CAN_AFFORD(RC_HAGS_POTION_SHOP_ITEM_2)),
            CHECK(RC_HAGS_POTION_SHOP_ITEM_3, CAN_AFFORD(RC_HAGS_POTION_SHOP_ITEM_3)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 5),      ENTRANCE(MAGIC_HAGS_POTION_SHOP, 0), true),
        },
    };
    Regions[RR_MILK_ROAD] = RandoRegion{ .sceneId = SCENE_ROMANYMAE,
        .checks = {
            CHECK(RC_MILK_ROAD_OWL_STATUE, CAN_USE_SWORD),
            CHECK(RC_MILK_ROAD_TINGLE_MAP_1, CAN_USE_PROJECTILE),
            CHECK(RC_MILK_ROAD_TINGLE_MAP_2, CAN_USE_PROJECTILE),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(TERMINA_FIELD, 5),                ENTRANCE(MILK_ROAD, 0), true),
            EXIT(ENTRANCE(ROMANI_RANCH, 0),                 ENTRANCE(MILK_ROAD, 1), true),
            EXIT(ENTRANCE(GORMAN_TRACK, 3),                 ENTRANCE(MILK_ROAD, 2), CAN_PLAY_SONG(EPONA)),
            EXIT(ENTRANCE(GORMAN_TRACK, 0),                 ENTRANCE(MILK_ROAD, 3), true),
        },
        .events = {
            EVENT_OWL_WARP(OWL_WARP_MILK_ROAD),
        },
        .oneWayEntrances = {
            ENTRANCE(MILK_ROAD, 4), // From Song of Soaring
        }
    };
    Regions[RR_RANCH_BARN] = RandoRegion{ .sceneId = SCENE_OMOYA,
        .checks = {
            CHECK(RC_ROMANI_RANCH_BARN_COW_LEFT, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG)),
            CHECK(RC_ROMANI_RANCH_BARN_COW_MIDDLE, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG)),
            CHECK(RC_ROMANI_RANCH_BARN_COW_RIGHT, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG))
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROMANI_RANCH, 2),                 ENTRANCE(RANCH_HOUSE, 0), true),
        },
    };
    Regions[RR_RANCH_HOUSE] = RandoRegion{ .sceneId = SCENE_OMOYA,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROMANI_RANCH, 3),                 ENTRANCE(RANCH_HOUSE, 1), true),
        },
    };
    Regions[RR_ROAD_TO_SOUTHERN_SWAMP] = RandoRegion{ .sceneId = SCENE_24KEMONOMITI,
        .checks = {
            CHECK(RC_ROAD_TO_SOUTHERN_SWAMP_HP, true),
            CHECK(RC_ROAD_TO_SOUTHERN_SWAMP_TINGLE_MAP_1, CAN_USE_PROJECTILE),
            CHECK(RC_ROAD_TO_SOUTHERN_SWAMP_TINGLE_MAP_2, CAN_USE_PROJECTILE),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(TERMINA_FIELD, 1),                ENTRANCE(ROAD_TO_SOUTHERN_SWAMP, 0), true),
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 0),      ENTRANCE(ROAD_TO_SOUTHERN_SWAMP, 1), true),
            EXIT(ENTRANCE(SWAMP_SHOOTING_GALLERY, 0),       ENTRANCE(ROAD_TO_SOUTHERN_SWAMP, 2), true),
        },
        .events = {
            EVENT_ACCESS(RANDO_ACCESS_SPRING_WATER, true),
        },
    };
    Regions[RR_ROMANI_RANCH] = RandoRegion{ .sceneId = SCENE_F01,
        .checks = {
            // TODO : Alien reward check
            CHECK(RC_ROMANI_RANCH_EPONAS_SONG, CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG)),
            CHECK(RC_ROMANI_RANCH_FIELD_COW_ENTRANCE, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG)),
            CHECK(RC_ROMANI_RANCH_FIELD_COW_NEAR_HOUSE_BACK, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG)),
            CHECK(RC_ROMANI_RANCH_FIELD_COW_NEAR_HOUSE_FRONT, CAN_PLAY_SONG(EPONA) && CAN_BE_GORON && HAS_ITEM(ITEM_POWDER_KEG))
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(MILK_ROAD, 1),                    ENTRANCE(ROMANI_RANCH, 0), true),
            EXIT(ENTRANCE(RANCH_HOUSE, 0),                  ENTRANCE(ROMANI_RANCH, 2), true), // Barn
            EXIT(ENTRANCE(RANCH_HOUSE, 1),                  ENTRANCE(ROMANI_RANCH, 3), true), // House
            EXIT(ENTRANCE(CUCCO_SHACK, 0),                  ENTRANCE(ROMANI_RANCH, 4), true),
            EXIT(ENTRANCE(DOGGY_RACETRACK, 0),              ENTRANCE(ROMANI_RANCH, 5), true),
        },
    };
    Regions[RR_SOUTHERN_SWAMP_NORTH] = RandoRegion{ .name = "North Section", .sceneId = SCENE_20SICHITAI,
        .checks = {
            CHECK(RC_SOUTHERN_SWAMP_HP, CAN_BE_DEKU && Flags_GetRandoInf(RANDO_INF_OBTAINED_DEED_LAND)),
            CHECK(RC_SOUTHERN_SWAMP_SCRUB_DEED, Flags_GetRandoInf(RANDO_INF_OBTAINED_DEED_LAND)),
            CHECK(RC_SOUTHERN_SWAMP_SCRUB_BEANS, CAN_BE_DEKU),
            CHECK(RC_SOUTHERN_SWAMP_OWL_STATUE, CAN_USE_SWORD),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROAD_TO_SOUTHERN_SWAMP, 1),       ENTRANCE(SOUTHERN_SWAMP_POISONED, 0), true),
            EXIT(ENTRANCE(TOURIST_INFORMATION, 0),          ENTRANCE(SOUTHERN_SWAMP_POISONED, 1), true),
            EXIT(ENTRANCE(MAGIC_HAGS_POTION_SHOP, 0),       ENTRANCE(SOUTHERN_SWAMP_POISONED, 5), true),
            EXIT(ENTRANCE(WOODS_OF_MYSTERY, 0),             ENTRANCE(SOUTHERN_SWAMP_POISONED, 7), true),
        },
        .connections = {
            CONNECTION(RR_SOUTHERN_SWAMP_SOUTH, (Flags_GetSceneSwitch(SCENE_20SICHITAI, 1) || CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE))),
        },
        .events = {
            EVENT_OWL_WARP(OWL_WARP_SOUTHERN_SWAMP),
            EVENT_ACCESS(RANDO_ACCESS_SPRING_WATER, true),
            EVENT_ACCESS(RANDO_ACCESS_BEANS_REFILL, CAN_BE_DEKU && HAS_ITEM(ITEM_MAGIC_BEANS)),
        },
        .oneWayEntrances = {
            ENTRANCE(SOUTHERN_SWAMP_POISONED, 9), // From river in Ikana
            ENTRANCE(SOUTHERN_SWAMP_POISONED, 10), // From Song of Soaring
        }
    };
    Regions[RR_SOUTHERN_SWAMP_SOUTH] = RandoRegion{ .name = "South Section", .sceneId = SCENE_20SICHITAI,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(WOODFALL, 0),                     ENTRANCE(SOUTHERN_SWAMP_POISONED, 2), CAN_BE_DEKU || CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)),
            EXIT(ENTRANCE(DEKU_PALACE, 0),                  ENTRANCE(SOUTHERN_SWAMP_POISONED, 3), true),
            EXIT(ENTRANCE(DEKU_PALACE, 5),                  ENTRANCE(SOUTHERN_SWAMP_POISONED, 4), CAN_BE_DEKU), // Treetop
            EXIT(ENTRANCE(SWAMP_SPIDER_HOUSE, 0),           ENTRANCE(SOUTHERN_SWAMP_POISONED, 8), CAN_LIGHT_TORCH_NEAR_ANOTHER),
        },
        .connections = {
            CONNECTION(RR_SOUTHERN_SWAMP_NORTH, (Flags_GetSceneSwitch(SCENE_20SICHITAI, 1) || CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE))),
        },
    };
    Regions[RR_SWAMP_SHOOTING_GALLERY] = RandoRegion{ .sceneId = SCENE_SYATEKI_MORI,
        .checks = {
            CHECK(RC_SWAMP_SHOOTING_GALLERY_HIGH_SCORE, HAS_ITEM(ITEM_BOW)),
            CHECK(RC_SWAMP_SHOOTING_GALLERY_PERFECT_SCORE, HAS_ITEM(ITEM_BOW)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ROAD_TO_SOUTHERN_SWAMP, 2),       ENTRANCE(SWAMP_SHOOTING_GALLERY, 0), true),
        },
    };
    Regions[RR_TOURIST_INFORMATION] = RandoRegion{ .sceneId = SCENE_MAP_SHOP,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 1),      ENTRANCE(TOURIST_INFORMATION, 0), true),
        },
        .events = {
            EVENT( // Killing Octorok blocking the southern swamp south section
                "Kill Octorok", 
                Flags_GetSceneSwitch(SCENE_20SICHITAI, 0x01), 
                Flags_SetSceneSwitch(SCENE_20SICHITAI, 0x01), 
                Flags_ClearSceneSwitch(SCENE_20SICHITAI, 0x01), 
                true // TODO: Conditions for starting swamp tour
            ),
        },
    };
    Regions[RR_WOODFALL_GREAT_FAIRY_FOUNTAIN] = RandoRegion{ .name = "Woodfall", .sceneId = SCENE_YOUSEI_IZUMI,
        .checks = {
            // TODO: We can't add this till all stray fairies are in the pool
            // CHECK(RC_WOODFALL_GREAT_FAIRY, gSaveContext.save.saveInfo.inventory.strayFairies[DUNGEON_INDEX_WOODFALL_TEMPLE] >= 15),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(WOODFALL, 2),                     ENTRANCE(FAIRY_FOUNTAIN, 1), true),
        },
    };
    Regions[RR_WOODFALL] = RandoRegion{ .sceneId = SCENE_21MITURINMAE,
        .checks = {
            CHECK(RC_WOODFALL_ENTRANCE_CHEST, CAN_BE_DEKU || CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)),
            CHECK(RC_WOODFALL_HP_CHEST, CAN_BE_DEKU),
            CHECK(RC_WOODFALL_OWL_STATUE, CAN_USE_SWORD),
            CHECK(RC_WOODFALL_NEAR_OWL_CHEST, CAN_BE_DEKU),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 2),      ENTRANCE(WOODFALL, 0), true),
            EXIT(ENTRANCE(WOODFALL_TEMPLE, 0),              ENTRANCE(WOODFALL, 1), CAN_BE_DEKU && CAN_PLAY_SONG(SONATA)),
            EXIT(ENTRANCE(FAIRY_FOUNTAIN, 1),               ENTRANCE(WOODFALL, 2), CAN_BE_DEKU),
        },
    };
    Regions[RR_WOODS_OF_MYSTERY] = RandoRegion{ .sceneId = SCENE_26SARUNOMORI,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(SOUTHERN_SWAMP_POISONED, 7),      ENTRANCE(WOODS_OF_MYSTERY, 0), true),
        },
    };
}, {});
// clang-format on
