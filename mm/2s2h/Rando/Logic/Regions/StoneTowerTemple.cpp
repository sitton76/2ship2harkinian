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
            CHECK(RC_STONE_TOWER_TEMPLE_ENTRANCE_CHEST, HAS_ITEM(ITEM_BOW)),
            CHECK(RC_STONE_TOWER_TEMPLE_ENTRANCE_SWITCH_CHEST, (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x0c) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0c))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_ENTRANCE_1, true),
            //CHECK(RC_STONE_TOWER_TEMPLE_POT_ENTRANCE_2, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER, 2),                  ENTRANCE(STONE_TOWER_TEMPLE, 0), true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SWITCH_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_DEEP_POOL_ROOM, (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x03) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x03)) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_BRIDGE, false) //This is one way, not sure how to go about doing this with a connection.
        },
        .events = {
            EVENT( // Spawns Stray Fairy chest in inverted
                "Spawn Stray Fairy chest in Inverted Entrance",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x05),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x05),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x05),
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
            CONNECTION(RR_STONE_TOWER_TEMPLE_SHALLOW_POOL_ROOM, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1) // TODO: Key is not working in check tracker logic atm
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_ARMOS_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_N,
        .checks = {
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
            CHECK(RC_STONE_TOWER_TEMPLE_COMPASS, (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x03) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x03))),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_BRIDGE_1, true),
            // CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_BRIDGE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_1, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_2, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_LOWER_3, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_UPPER_1, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_POT_WATER_ROOM_UNDERWATER_UPPER_2, CAN_BE_ZORA),
            CHECK(RC_STONE_TOWER_TEMPLE_WATER_SUN_SWITCH_CHEST, CAN_BE_ZORA && (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x0b) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0b)))
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_SHALLOW_POOL_ROOM, CAN_BE_ZORA),
            CONNECTION(RR_STONE_TOWER_TEMPLE_ENTRANCE, Flags_GetSceneSwitch(SCENE_INISIE_R, 0x03) || CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_MIRROR_PILLAR_ROOM, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1) // TODO: Key is not working in check tracker logic atm
        },
        .events = {
            EVENT( // Spawns Stray Fairy chest in ISTT Wind Room
                "Spawn Stray Fairy chest #1 in ISTT Wind Room",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x07),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x07),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x07),
                CAN_BE_ZORA
            ),
            EVENT( // Spawns Stray Fairy chest in ISTT Wind Room
                "Spawn Stray Fairy chest #2 in ISTT Wind Room",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x1d),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x1d),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x1d),
                CAN_USE_MAGIC_ARROW(FIRE)
            ),
            EVENT( // Gets rid of Sun Block
                "Remove the Sun Block",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x03),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x03),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x03),
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
            CHECK(RC_STONE_TOWER_TEMPLE_BEFORE_WATER_BRIDGE_CHEST, CAN_USE_EXPLOSIVE || (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x16) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x16))),
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

    // Inverted Temple
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_CHEST, (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x05) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x05))),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER_INVERTED, 1),         ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0), true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_WIND_ROOM, CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_TOP, HAS_ITEM(ITEM_HOOKSHOT) && (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x0c) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0c)))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_WIND_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_UPPER_CHEST, CAN_BE_DEKU && CAN_PLAY_SONG(ELEGY) && (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x1d) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x1d))),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_MIDDLE_CHEST, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_EAST_LOWER_CHEST, CAN_BE_DEKU && (Flags_GetSceneSwitch(SCENE_INISIE_R, 0x1D) || CAN_USE_MAGIC_ARROW(FIRE))),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_BRIDGE_1, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_BRIDGE_2, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_1, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_2, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_3, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_UPDRAFTS_LEDGE_4, CAN_BE_DEKU)
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_LAVA_FLIP_ROOM, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1 && CAN_BE_DEKU) // TODO: Key is not working in check tracker logic atm
        },
        .events = {
                EVENT( // Spawns Stray Fairy chest in STT Deep Water Room
                "Spawn Stray Fairy chest in STT Deep Water Room",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0b),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x0b),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x0b),
                true
            ),
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_LAVA_FLIP_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_WIND_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_BLOCK_FLIP_ROOM, CAN_BE_GORON && CAN_USE_MAGIC_ARROW(LIGHT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_BLOCK_FLIP_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_LAVA_FLIP_ROOM, CAN_USE_MAGIC_ARROW(LIGHT)),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_WIZZROBE_ROOM, CAN_USE_MAGIC_ARROW(LIGHT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_WIZZROBE_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_WIZZROBE_CHEST, CAN_KILL_WIZZROBE && HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_1, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_2, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_3, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_4, HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_WIZZROBE_5, HAS_ITEM(ITEM_HOOKSHOT)),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_BLOCK_FLIP_ROOM, CAN_KILL_WIZZROBE),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_POE_ROOM, CAN_KILL_WIZZROBE && HAS_ITEM(ITEM_HOOKSHOT))
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_POE_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_WIZZROBE_SIDE_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_WIZZROBE_SIDE_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_MAZE_SIDE_1, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_POE_MAZE_SIDE_2, CAN_BE_DEKU),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_DEATH_ARMOS_CHEST, CAN_BE_DEKU && CAN_PLAY_SONG(ELEGY)),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_WIZZROBE_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_UNDER_BRIDGE, CAN_BE_DEKU)
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_UNDER_BRIDGE] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_POE_ROOM, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_PATH_TO_GOMESS, CAN_BE_DEKU && (HAS_ITEM(ITEM_HOOKSHOT) || HAS_ITEM(ITEM_BOW) || CAN_BE_ZORA)), // Deku bubbles can work with TIGHT aim, prob better to not consider that in logic
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_SIDE_OF_ENTRANCE, true)
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_PATH_TO_GOMESS] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_UNDER_BRIDGE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_GOMESS_ROOM, true)
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_GOMESS_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_KEY, CAN_USE_HUMAN_SWORD && CAN_USE_MAGIC_ARROW(LIGHT)), // Check this properly later
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_GOMESS_4, true),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_PATH_TO_GOMESS, true),
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_SIDE_OF_ENTRANCE] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_UNDER_BRIDGE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_TOP, HAS_ITEM(ITEM_HOOKSHOT) && (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x0c) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0c)))
        },
        .events = {
                EVENT( // Spawns Stray Fairy chest in STT
                "Spawn Stray Fairy chest in STT Entrance",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x0c),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x0c),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x0c),
                true
            ),
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_TOP] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_SIDE_OF_ENTRANCE, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_BRIDGE, KEY_COUNT(STONE_TOWER_TEMPLE) >= 1) // TODO: Key is not working in check tracker logic atm
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_BRIDGE] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_GIANT_MASK, CAN_USE_PROJECTILE),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_ENTRANCE_TOP, true),
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_SPIKED_BAR_ROOM_UPPER, CAN_USE_PROJECTILE)
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_SPIKED_BAR_ROOM_UPPER] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_SPIKED_BAR_ROOM_LOWER, HAS_ITEM(ITEM_HOOKSHOT) || (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x16) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x16)))
        },
        .events = {
                EVENT( // Spawns Stray Fairy chest in STT Spiked Bar Room
                "Spawn Stray Fairy chest in STT Spiked Bar Room",
                Flags_GetSceneSwitch(SCENE_INISIE_R, 0x16),
                Flags_SetSceneSwitch(SCENE_INISIE_R, 0x16),
                Flags_ClearSceneSwitch(SCENE_INISIE_R, 0x16),
                true
            ),
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_SPIKED_BAR_ROOM_LOWER] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_1, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_2, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_3, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_4, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_5, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_6, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_7, true),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_POT_PRE_BOSS_8, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 1),           ONE_WAY_EXIT, CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, DUNGEON_INDEX_STONE_TOWER_TEMPLE)),
        },
        .connections = {
            CONNECTION(RR_STONE_TOWER_TEMPLE_INVERTED_SPIKED_BAR_ROOM_UPPER, HAS_ITEM(ITEM_HOOKSHOT) || (Flags_GetSceneSwitch(SCENE_INISIE_N, 0x16) || Flags_GetSceneSwitch(SCENE_INISIE_R, 0x16))),
        },
    };
    Regions[RR_STONE_TOWER_TEMPLE_INVERTED_TWINMOLD_BOSS_ENTRANCE] = RandoRegion{ .sceneId = SCENE_INISIE_R,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(TWINMOLDS_LAIR, 0),                        ONE_WAY_EXIT, true),
        },
        .oneWayEntrances = {
            ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 1), // Hole to before boss arena
        }
    };
    Regions[RR_STONE_TOWER_TEMPLE_BOSS_ROOM] = RandoRegion{ .sceneId = SCENE_INISIE_BS,
        .checks = {
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_HC, CAN_KILL_TWINMOLD),
            CHECK(RC_STONE_TOWER_TEMPLE_INVERTED_BOSS_WARP, CAN_KILL_TWINMOLD)
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CANYON, 15),                        ONE_WAY_EXIT, true),
        },
        .oneWayEntrances = {
            ENTRANCE(TWINMOLDS_LAIR, 0), // Blue warp exit
        }
    };
}, {});
// clang-format on
