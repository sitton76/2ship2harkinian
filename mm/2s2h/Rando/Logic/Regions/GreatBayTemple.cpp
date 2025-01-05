#include <libultraship/libultraship.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_GREAT_BAY_TEMPLE_BABA_CHEST_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_BABA_CHEST, CAN_BE_ZORA || CAN_USE_PROJECTILE || HAS_ITEM(ITEM_HOOKSHOT)),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_COMPASS_ROOM,    true),
            CONNECTION(RR_GREAT_BAY_TEMPLE_MAP_ROOM,        true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_BEFORE_WART] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_05, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_06, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_07, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_08, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_09, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_10, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_11, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BEFORE_WART_POT_12, true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART,  KEY_COUNT(GREAT_BAY_TEMPLE) >= 1),
            CONNECTION(RR_GREAT_BAY_TEMPLE_WART,                  true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_BOSS_ROOM] = RandoRegion{ .sceneId = SCENE_SEA_BS,
        .checks = {
            // TODO: CAN_KILL_BOSS(Gyorg)?
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_HEART_CONTAINER, CAN_BE_ZORA && HAS_MAGIC),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_WARP, CAN_BE_ZORA && HAS_MAGIC),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_POT_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_POT_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_POT_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_POT_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_UNDERWATER_POT_01, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_UNDERWATER_POT_02, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_UNDERWATER_POT_03, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_UNDERWATER_POT_04, CAN_BE_ZORA),
            CHECK(RC_GIANTS_CHAMBER_OATH_TO_ORDER, CAN_BE_ZORA && HAS_MAGIC),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ZORA_CAPE, 9),                             ONE_WAY_EXIT, true),
        },
        .oneWayEntrances = {
            ENTRANCE(GYORGS_LAIR, 0), // Great Bay Temple Pre Boss Room
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_SF_CENTRAL_ROOM_BARREL,           CAN_BE_GORON || CAN_USE_EXPLOSIVE),
            CHECK(RC_GREAT_BAY_TEMPLE_CENTRAL_ROOM_POT_01,              true),
            CHECK(RC_GREAT_BAY_TEMPLE_CENTRAL_ROOM_POT_02,              true),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_CENTRAL_ROOM_UNDERWATER_POT,   CAN_BE_ZORA),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_GREEN_PIPE_1,            CAN_USE_MAGIC_ARROW(ICE)),
            CONNECTION(RR_GREAT_BAY_TEMPLE_MAP_ROOM,                CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_PRE_BOSS_ROOM,           CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART,    true),
            CONNECTION(RR_GREAT_BAY_TEMPLE_WATER_WHEEL_ROOM,        true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_COMPASS_ROOM_WITH_BOSS_KEY_CHEST] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_BOSS_KEY, true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_COMPASS_ROOM,    true),
            CONNECTION(RR_GREAT_BAY_TEMPLE_GEKKO,           CAN_USE_MAGIC_ARROW(ICE)),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_COMPASS_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_CHEST,              true),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_UNDERWATER,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_SURFACE_POT_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_SURFACE_POT_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_SURFACE_POT_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_SURFACE_POT_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_WATER_POT_01,   CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_WATER_POT_02,   CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_WATER_POT_03,   CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_COMPASS_ROOM_POT,        CAN_BE_ZORA || (CAN_USE_PROJECTILE && HAS_ITEM(ITEM_MASK_GREAT_FAIRY))),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_FREESTANDING_RUPEE_01, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_COMPASS_ROOM_FREESTANDING_RUPEE_02, CAN_BE_ZORA),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_BABA_CHEST_ROOM,                   true),
            CONNECTION(RR_GREAT_BAY_TEMPLE_COMPASS_ROOM_WITH_BOSS_KEY_CHEST,  CAN_BE_ZORA && CAN_USE_MAGIC_ARROW(ICE)),
            CONNECTION(RR_GREAT_BAY_TEMPLE_GEKKO,                             CAN_USE_MAGIC_ARROW(ICE)),
            CONNECTION(RR_GREAT_BAY_TEMPLE_GREEN_PIPE_2,                      CAN_BE_ZORA),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_ENTRANCE] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_05, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_06, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_07, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_BARREL_08, true),
            CHECK(RC_GREAT_BAY_TEMPLE_ENTRANCE_CHEST,     CAN_LIGHT_TORCH_NEAR_ANOTHER),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ZORA_CAPE, 7),                    ENTRANCE(GREAT_BAY_TEMPLE, 0), HAS_ITEM(ITEM_HOOKSHOT)),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_WATER_WHEEL_ROOM,  true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_GEKKO] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_05, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_06, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_07, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_08, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_09, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_10, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_11, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_12, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_13, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_14, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_15, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GEKKO_SMALL_CRATE_16, true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_COMPASS_ROOM_WITH_BOSS_KEY_CHEST,    CAN_USE_MAGIC_ARROW(ICE)),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_GREEN_PIPE_1] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_CHEST,                 HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_POT_01,                CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_POT_02,                CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_POT_03,                CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_POT_04,                CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_FREESTANDING_RUPEE_01, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_FREESTANDING_RUPEE_02, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_BARREL_01,             HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_BARREL_02,             HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_1_BARREL_03,             HAS_ITEM(ITEM_HOOKSHOT)),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM,  true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_GREEN_PIPE_2] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_BARREL_01,   true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_BARREL_02,   true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_LOWER_CHEST, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_UPPER_CHEST, (HAS_ITEM(ITEM_HOOKSHOT) && CAN_USE_MAGIC_ARROW(ICE))),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_01,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_02,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_03,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_04,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_05,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_06,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_07,      CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_2_POT_08,      CAN_BE_ZORA),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_COMPASS_ROOM,  CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_GREEN_PIPE_3,  CAN_USE_MAGIC_ARROW(ICE)),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_GREEN_PIPE_3] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_CHEST,          CAN_USE_MAGIC_ARROW(FIRE)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LOWER_POT,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_UPPER_POT_01,   CAN_USE_MAGIC_ARROW(FIRE)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_UPPER_POT_02,   CAN_USE_MAGIC_ARROW(FIRE)),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_GREEN_PIPE_3_BARREL,      CAN_BE_ZORA && CAN_USE_MAGIC_ARROW(FIRE)),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_05, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_06, true),
            CHECK(RC_GREAT_BAY_TEMPLE_GREEN_PIPE_3_LARGE_CRATE_07, true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_GREEN_PIPE_2,  true),
            CONNECTION(RR_GREAT_BAY_TEMPLE_MAP_ROOM,      CAN_USE_MAGIC_ARROW(FIRE)),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_MAP_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_CHEST,                HAS_ITEM(ITEM_HOOKSHOT) || CAN_USE_MAGIC_ARROW(ICE)),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_SURFACE_POT_01,  true),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_SURFACE_POT_02,  true),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_SURFACE_POT_03,  CAN_BE_ZORA || CAN_USE_MAGIC_ARROW(ICE)),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_01,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_02,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_03,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_04,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_05,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_06,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_07,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_MAP_ROOM_WATER_POT_08,    CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_MAP_ROOM_POT,          CAN_BE_ZORA || CAN_USE_MAGIC_ARROW(ICE)),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_BABA_CHEST_ROOM,         CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM,            CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM,    true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_PRE_BOSS_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_01,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_02,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_03,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_04,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_05,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_06,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_07,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_PRE_BOSS_POT_08,          CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_PRE_BOSS_ABOVE_WATER, CAN_USE_PROJECTILE && HAS_ITEM(ITEM_MASK_GREAT_FAIRY)),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_PRE_BOSS_UNDERWATER,  CAN_BE_ZORA),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(GYORGS_LAIR, 0),                           ONE_WAY_EXIT, CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, DUNGEON_INDEX_GREAT_BAY_TEMPLE)),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM, CAN_BE_ZORA),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART_POT_01, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART_POT_02, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART_POT_03, CAN_BE_ZORA),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_BEFORE_WART_POT_04, CAN_BE_ZORA),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM,    CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_BEFORE_WART,     KEY_COUNT(GREAT_BAY_TEMPLE) >= 1),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_BARREL_01,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_BARREL_02,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_BARREL_03,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_BARREL_04,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_BARREL_05,      true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_LARGE_CRATE_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_LARGE_CRATE_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_LARGE_CRATE_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_LARGE_CRATE_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_RED_PIPE_SWITCH_ROOM_LARGE_CRATE_05, true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_MAP_ROOM, true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_WART] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_ICE_ARROW_CHEST,  true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_01,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_02,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_03,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_04,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_05,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_06,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_07,       true),
            CHECK(RC_GREAT_BAY_TEMPLE_WART_POT_08,       true),
        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_BEFORE_WART, true),
        },
    };
    Regions[RR_GREAT_BAY_TEMPLE_WATER_WHEEL_ROOM] = RandoRegion{ .sceneId = SCENE_SEA,
        .checks = {
            CHECK(RC_GREAT_BAY_TEMPLE_SF_WATER_WHEEL_PLATFORM,           CAN_BE_ZORA || (HAS_ITEM(ITEM_BOW) && HAS_ITEM(ITEM_MASK_GREAT_FAIRY))),
            CHECK(RC_GREAT_BAY_TEMPLE_SF_WATER_WHEEL_SKULLTULA,          true),
            CHECK(RC_GREAT_BAY_TEMPLE_WATER_WHEEL_FREESTANDING_RUPEE_01, true),
            CHECK(RC_GREAT_BAY_TEMPLE_WATER_WHEEL_FREESTANDING_RUPEE_02, true),
            CHECK(RC_GREAT_BAY_TEMPLE_WATER_WHEEL_FREESTANDING_RUPEE_03, true),
            CHECK(RC_GREAT_BAY_TEMPLE_WATER_WHEEL_FREESTANDING_RUPEE_04, true),
            CHECK(RC_GREAT_BAY_TEMPLE_WATER_WHEEL_FREESTANDING_RUPEE_05, true),

        },
        .connections = {
            CONNECTION(RR_GREAT_BAY_TEMPLE_CENTRAL_ROOM,  CAN_BE_ZORA),
            CONNECTION(RR_GREAT_BAY_TEMPLE_ENTRANCE,      true),
        },
    };
}, {});
// clang-format on
