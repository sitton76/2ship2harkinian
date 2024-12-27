#include <libultraship/libultraship.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    // Rightside Temple.
    Regions[RR_STONE_TOWER_TEMPLE_ENTRANCE] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            // TODO: Add a Stray Fairy chest check here later, spawns from inverted.
            CHECK(RC_STONE_TOWER_TEMPLE_ENTRANCE_CHEST, HAS_ITEM(ITEM_BOW)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_ENTRANCE_1, true),
            //CHECK(RC_STONE_TOWER_TEMPLE_POT_ENTRANCE_2, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER, 2),                  ENTRANCE(STONE_TOWER_TEMPLE, 0), true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SWITCH_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_DEEP_POOL_ROOM, Flags_GetSceneSwitch(SCENE_CASTLE, 0x03) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_BRIDGE, false) //This is one way, not sure how to go about doing this with a connection.
        },
        .events = {
            EVENT( // Spawns Stray Fairy chest in inverted
                "Spawn Stray Fairy chest in Inverted Entrance",
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x05),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x05),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x05),
                CAN_USE_MAGIC_ARROW(LIGHT)
            )
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_SWITCH_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_ENTRANCE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_OUTSIDE_SWITCH_ROOM, CAN_BE_GORON && CAN_PLAY_SONG(ELEGY) && CAN_USE_EXPLOSIVE)
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_OUTSIDE_SWITCH_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SWITCH_ROOM, false),
            CONNECTION(RR_STONE_TOWER_TEMPLE_ARMOS_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_SHALLOW_POOL_ROOM, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1)
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_ARMOS_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            // TODO : Add check for stray fairy chest that spawns here later.
            CHECK(RC_STONE_TOWER_TEMPLE_UNDER_WEST_GARDEN_LEDGE_CHEST, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_UNDER_WEST_GARDEN_LAVA_CHEST, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))),
            //CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_MAP, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_AFTER_BLOCK_1, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_AFTER_BLOCK_2, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_AFTER_BLOCK_3, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_LAVA_ROOM_AFTER_BLOCK_4, CAN_BE_GORON && ((CAN_USE_EXPLOSIVE && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT)))
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_OUTSIDE_SWITCH_ROOM, true)
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_SHALLOW_POOL_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            // IF you get this check you must be Zora to leave, but you could in theory get this as Human too without the Zora mask...kinda iffy to me
            CHECK(RC_STONE_TOWER_TEMPLE_CENTER_ACROSS_WATER_CHEST, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_CENTER_SUN_BLOCK_CHEST, CAN_BE_ZORA && CAN_USE_MAGIC_ARROW(LIGHT))
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_OUTSIDE_SWITCH_ROOM, true), // Double check if this side needs a key too
            CONNECTION(RR_STONE_TOWER_TEMPLE_DEEP_POOL_ROOM, CAN_BE_ZORA)
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_DEEP_POOL_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            // TODO : Go back and add stay fairy chest that spawns from inverted.
            CHECK(RC_STONE_TOWER_TEMPLE_COMPASS, Flags_GetSceneSwitch(SCENE_CASTLE, 0x03)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_BRIDGE_1, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_BRIDGE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_1, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_2, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_3, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_UPPER_1, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_UPPER_2, CAN_BE_ZORA)
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SHALLOW_POOL_ROOM, CAN_BE_ZORA),
            CONNECTION(RR_STONE_TOWER_TEMPLE_ENTRANCE, Flags_GetSceneSwitch(SCENE_CASTLE, 0x03) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_MIRROR_PILLAR_ROOM, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1)
        },
        .events = {
            EVENT( // Spawns Stray Fairy chest in inverted
                "Spawn Stray Fairy chest in Inverted part", // TODO FILL THIS IN WITH THE PROPER NAME LATER!!!
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x07),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x07),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x07),
                CAN_BE_ZORA
            ),
            EVENT( // Spawns Stray Fairy chest in inverted
                "Spawn Stray Fairy chest in Inverted part", // TODO FILL THIS IN WITH THE PROPER NAME LATER!!!
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x1d),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x1d),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x1d),
                CAN_USE_MAGIC_ARROW(FIRE)
            ),
            EVENT( // Gets rid of Sun Block
                "Remove the Sun Block",
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x03),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x03),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x03),
                (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR) || CAN_USE_MAGIC_ARROW(LIGHT)
            )
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_MIRROR_PILLAR_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_MIRRORS_ROOM_CENTER_CHEST, (CAN_BE_GORON && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CHECK(RC_STONE_TOWER_TEMPLE_MIRRORS_ROOM_RIGHT_CHEST, (CAN_BE_GORON && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_MIRROR_ROOM_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_MIRROR_ROOM_2, true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_DEEP_POOL_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_LAVA_WIND_ROOM, (CAN_BE_GORON && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)) || CAN_USE_MAGIC_ARROW(LIGHT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_LAVA_WIND_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_WIND_ROOM_LEDGE_CHEST, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_WIND_ROOM_JAIL_CHEST, CAN_BE_GORON && (CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WIND_ROOM_1, CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WIND_ROOM_2, CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WIND_ROOM_3, CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT)),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WIND_ROOM_4, CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT)),
        },
        .connections = {
            // The end of this is a one way exit, so logically you should be able to return without issue.
            CONNECTION(RR_STONE_TOWER_TEMPLE_MIRROR_PILLAR_ROOM, true),
            // Literally only just now found the hidden sun switch to trigger the ladder spawn, wild. Look above the fire switch in the lava that spawns the chest.
            CONNECTION(RR_STONE_TOWER_TEMPLE_GARO_MASTER_ROOM, CAN_BE_DEKU || CAN_USE_MAGIC_ARROW(LIGHT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_GARO_MASTER_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_LIGHT_ARROW, CAN_KILL_GARO_MASTER),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_LAVA_WIND_ROOM, CAN_KILL_GARO_MASTER),
            CONNECTION(RR_STONE_TOWER_TEMPLE_SPIKED_BAR_ROOM, CAN_KILL_GARO_MASTER)
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_SPIKED_BAR_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_BEFORE_WATER_BRIDGE_CHEST, CAN_USE_EXPLOSIVE),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_1, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_2, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_3, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_4, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_5, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_6, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_7, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_BEFORE_WATER_BRIDGE_8, true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_GARO_MASTER_ROOM, HAS_ITEM(ITEM_HOOKSHOT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_BRIDGE, HAS_ITEM(ITEM_HOOKSHOT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_BRIDGE] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_WATER_BRIDGE_CHEST, CAN_USE_PROJECTILE),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SPIKED_BAR_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_ENTRANCE, CAN_USE_PROJECTILE)
        }
    };

    // END
    Regions[RR_STONE_TOWER_TEMPLE_BOSS_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_BS,
        .checks = {
            // TODO: CAN_KILL_BOSS(Twinmold)?
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_HC, HAS_MAGIC),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_WARP, HAS_MAGIC),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CANYON, 15),                         ONE_WAY_EXIT, true),
        },
        .oneWayEntrances = {
            ENTRANCE(TWINMOLDS_LAIR, 0), // Stone Tower Temple Pre Boss Room
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_HC, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_KEY, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_WARP, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_DEATH_ARMOS_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_LOWER_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_MIDDLE_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_UPPER_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_GIANT_MASK, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_WIZZROBE_CHEST, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_MAZE_SIDE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_MAZE_SIDE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_WIZZROBE_SIDE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_WIZZROBE_SIDE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_5, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_6, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_7, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_8, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_BRIDGE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_BRIDGE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_5, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER_INVERTED, 1),         ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0), true),
            EXIT(ENTRANCE(TWINMOLDS_LAIR, 0),                        ONE_WAY_EXIT, CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, DUNGEON_INDEX_STONE_TOWER_TEMPLE)),
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
            //CHECK(RC_STONE_TOWER_TEMPLE_ENTRANCE_SWITCH_CHEST, true),
            //CHECK(RC_STONE_TOWER_TEMPLE_WATER_SUN_SWITCH_CHEST, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER, 2),                  ENTRANCE(STONE_TOWER_TEMPLE, 0), true),
        },
    };
}, {});
// clang-format on
