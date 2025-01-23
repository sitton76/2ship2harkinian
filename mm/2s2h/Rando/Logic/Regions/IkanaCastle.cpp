#include <libultraship/libultraship.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_IKANA_CASTLE_BEFORE_THRONE] = RandoRegion{ .name = "Before Throne Room", .sceneId = SCENE_CASTLE,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IGOS_DU_IKANAS_LAIR, 0),          ENTRANCE(IKANA_CASTLE, 6), true)
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_MAIN_ROOM, CAN_USE_MAGIC_ARROW(LIGHT) || (Flags_GetSceneSwitch(SCENE_CASTLE, 0x25) && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)))
        }
    };
    Regions[RR_IKANA_CASTLE_BUBBLE_ROOM] = RandoRegion{ .name = "Bubble Room", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_THIRD_ROOM_POT_01, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_THIRD_ROOM_POT_02, true),
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_SKULLTULA_ROOM, CAN_USE_HUMAN_SWORD),
            CONNECTION(RR_IKANA_CASTLE_OUTER_ROOF, true)
        },
    };
    Regions[RR_IKANA_CASTLE_CEILING_ROOM] = RandoRegion{ .name = "Ceiling Room", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_FIRST_ROOM_POT_01, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_FIRST_ROOM_POT_02, true)
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_MAIN_ROOM, true),
            CONNECTION(RR_IKANA_CASTLE_SKULLTULA_ROOM, CAN_BE_DEKU),
        },
    };
    Regions[RR_IKANA_CASTLE_COURTYARD] = RandoRegion{ .name = "Courtyard", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_EXTERIOR_POT, true)
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(BENEATH_THE_WELL, 1),             ENTRANCE(IKANA_CASTLE, 0), true),
            EXIT(ENTRANCE(IKANA_CASTLE, 3),                 ENTRANCE(IKANA_CASTLE, 2), true),
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_FRONT_ENTRANCE, CAN_USE_MAGIC_ARROW(LIGHT)),
        },
    };
    Regions[RR_IKANA_CASTLE_FLOORMASTER_ROOM] = RandoRegion{ .name = "Floormaster Room", .sceneId = SCENE_CASTLE,
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_MAIN_ROOM, true),
            CONNECTION(RR_IKANA_CASTLE_WIZZROBE_ROOM, CAN_USE_MAGIC_ARROW(LIGHT) || (Flags_GetSceneSwitch(SCENE_CASTLE, 0x27) && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR))),
        },
        .oneWayEntrances = {
            ENTRANCE(IKANA_CASTLE, 5), // From Inner Roof
        }
    };
    Regions[RR_IKANA_CASTLE_FRONT_ENTRANCE] = RandoRegion{ .name = "Front Entrance", .sceneId = SCENE_CASTLE,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CANYON, 8),                 ENTRANCE(IKANA_CASTLE, 1), true),
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_COURTYARD, CAN_USE_MAGIC_ARROW(LIGHT) || ((GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR) && (CAN_USE_HUMAN_SWORD || CAN_BE_GORON || CAN_USE_PROJECTILE))),
        },
    };
    Regions[RR_IKANA_CASTLE_INNER_ROOF] = RandoRegion{ .name = "Inner Roof", .sceneId = SCENE_CASTLE,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CASTLE, 5),                          ONE_WAY_EXIT, Flags_GetSceneSwitch(SCENE_CASTLE, 0x27)),
            EXIT(ENTRANCE(IKANA_CASTLE, 4),                          ONE_WAY_EXIT, Flags_GetSceneSwitch(SCENE_CASTLE, 0x25))
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_REDEAD_WALKWAY, true)
        },
        .events = {
            EVENT( // Opening sunlight in the main room of Ikana Castle
                "Open Sunlight for Ikana Castle Main Room", 
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x25),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x25),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x25), 
                true
            ),
        }
    };
    Regions[RR_IKANA_CASTLE_MAIN_ROOM] = RandoRegion{ .name = "Main Room", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_ENTRANCE_POT_01, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_ENTRANCE_POT_02, true)
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CASTLE, 2),                 ENTRANCE(IKANA_CASTLE, 3), true)
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_CEILING_ROOM, CAN_USE_MAGIC_ARROW(FIRE)),
            CONNECTION(RR_IKANA_CASTLE_FLOORMASTER_ROOM, CAN_USE_MAGIC_ARROW(FIRE)),
            CONNECTION(RR_IKANA_CASTLE_BEFORE_THRONE, CAN_USE_MAGIC_ARROW(LIGHT) || (Flags_GetSceneSwitch(SCENE_CASTLE, 0x25) && (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) >= EQUIP_VALUE_SHIELD_MIRROR)))
        },
        .oneWayEntrances = {
            ENTRANCE(IKANA_CASTLE, 4), // From Inner Roof Powder Keg Hole
        }
    };
    Regions[RR_IKANA_CASTLE_OUTER_ROOF] = RandoRegion{ .name = "Outer Roof", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_PIECE_OF_HEART, CAN_USE_PROJECTILE && CAN_BE_DEKU)
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_BUBBLE_ROOM , true),
            CONNECTION(RR_IKANA_CASTLE_FRONT_ENTRANCE, true)
        },
        .events = {
            EVENT( // Opening sunlight in the right side of Ikana Castle
                "Open Sunlight for Ikana Castle Right Room",
                Flags_GetSceneSwitch(SCENE_CASTLE, 0x27),
                Flags_SetSceneSwitch(SCENE_CASTLE, 0x27),
                Flags_ClearSceneSwitch(SCENE_CASTLE, 0x27),
                true
            ),
        }
    };
    Regions[RR_IKANA_CASTLE_REDEAD_WALKWAY] = RandoRegion{ .name = "Redead Walkway", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_RIGHT_POT_01, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_RIGHT_POT_02, true)
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_WIZZROBE_ROOM, true),
            CONNECTION(RR_IKANA_CASTLE_INNER_ROOF, true)
        },
    };
    Regions[RR_IKANA_CASTLE_SKULLTULA_ROOM] = RandoRegion{ .name = "Skulltula Room", .sceneId = SCENE_CASTLE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_SECOND_ROOM_POT_01, CAN_BE_DEKU || HAS_ITEM(ITEM_BOW)),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_SECOND_ROOM_POT_02, CAN_BE_DEKU || HAS_ITEM(ITEM_BOW)),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_SECOND_ROOM_POT_03, CAN_BE_DEKU || HAS_ITEM(ITEM_BOW)),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_LEFT_SECOND_ROOM_POT_04, CAN_BE_DEKU || HAS_ITEM(ITEM_BOW))
        },
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_CEILING_ROOM, CAN_USE_HUMAN_SWORD || CAN_USE_PROJECTILE),
            CONNECTION(RR_IKANA_CASTLE_BUBBLE_ROOM, HAS_ITEM(ITEM_LENS_OF_TRUTH) && HAS_MAGIC && CAN_BE_DEKU && (HAS_ITEM(ITEM_BOW) || HAS_ITEM(ITEM_BOMB)))
        },
    };
    Regions[RR_IKANA_CASTLE_THRONE_ROOM] = RandoRegion{ .name = "Throne Room", .sceneId = SCENE_IKNINSIDE,
        .checks = {
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS, CanKillEnemy(ACTOR_EN_KNIGHT)),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_01, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_02, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_03, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_04, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_05, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_06, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_07, true),
            CHECK(RC_ANCIENT_CASTLE_OF_IKANA_BOSS_POT_08, true),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(IKANA_CASTLE, 6),                 ENTRANCE(IGOS_DU_IKANAS_LAIR, 0), true)
        }
    };
    Regions[RR_IKANA_CASTLE_WIZZROBE_ROOM] = RandoRegion{ .name = "Wizzrobe Room", .sceneId = SCENE_CASTLE,
        .connections = {
            CONNECTION(RR_IKANA_CASTLE_FLOORMASTER_ROOM, CanKillEnemy(ACTOR_EN_WIZ)),
            CONNECTION(RR_IKANA_CASTLE_REDEAD_WALKWAY, CanKillEnemy(ACTOR_EN_WIZ))
        },
    };
}, {});
// clang-format on
