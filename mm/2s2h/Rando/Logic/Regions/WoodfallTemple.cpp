#include <libultraship/libultraship.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/Logic/Logic.h"

using namespace Rando::Logic;

// clang-format off
static RegisterShipInitFunc initFunc([]() {
    Regions[RR_WOODFALL_TEMPLE_BOSS_KEY_ROOM] = RandoRegion{ .name = "Boss Key Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_BOSS_KEY_CHEST, CanKillEnemy(ACTOR_EN_PAMETFROG)),
            CHECK(RC_WOODFALL_TEMPLE_MINIBOSS_ROOM_POT_01, CanKillEnemy(ACTOR_EN_PAMETFROG)),
            CHECK(RC_WOODFALL_TEMPLE_MINIBOSS_ROOM_POT_02, CanKillEnemy(ACTOR_EN_PAMETFROG)),
            CHECK(RC_WOODFALL_TEMPLE_MINIBOSS_ROOM_POT_03, CanKillEnemy(ACTOR_EN_PAMETFROG)),
            CHECK(RC_WOODFALL_TEMPLE_MINIBOSS_ROOM_POT_04, CanKillEnemy(ACTOR_EN_PAMETFROG)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM_UPPER, true),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_BOSS_ROOM] = RandoRegion{ .sceneId = SCENE_MITURIN_BS,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_BOSS_CONTAINER, CanKillEnemy(ACTOR_BOSS_01)),
            CHECK(RC_WOODFALL_TEMPLE_BOSS_WARP, CanKillEnemy(ACTOR_BOSS_01)),
            CHECK(RC_GIANTS_CHAMBER_OATH_TO_ORDER, CanKillEnemy(ACTOR_BOSS_01)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(WOODFALL_TEMPLE, 1),                       ONE_WAY_EXIT, CanKillEnemy(ACTOR_BOSS_01)),
        },
        .events = {
            EVENT_WEEKEVENTREG("Clear Woodfall Temple", WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE, CanKillEnemy(ACTOR_BOSS_01)),
        },
        .oneWayEntrances = {
            ENTRANCE(ODOLWAS_LAIR, 0), // From Woodfall Temple Pre-Boss Room
        },
    };
    Regions[RR_WOODFALL_TEMPLE_BOW_ROOM] = RandoRegion{ .name = "Bow Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_BOW_CHEST, CanKillEnemy(ACTOR_EN_DINOFOS)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM_UPPER, true),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_COMPASS_ROOM] = RandoRegion{ .name = "Compass Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_COMPASS_CHEST, CanKillEnemy(ACTOR_EN_GRASSHOPPER)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAZE_ROOM, true),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_DARK_ROOM] = RandoRegion{ .name = "Dark Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_DARK_CHEST,  CanKillEnemy(ACTOR_EN_MKK)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM_UPPER, CAN_BE_DEKU && CAN_LIGHT_TORCH_NEAR_ANOTHER),
            CONNECTION(RR_WOODFALL_TEMPLE_MAZE_ROOM, CAN_LIGHT_TORCH_NEAR_ANOTHER),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_DEKU_PRINCESS_ROOM] = RandoRegion{ .name = "Deku Princess Room", .sceneId = SCENE_MITURIN,
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(WOODFALL, 3),                     ENTRANCE(WOODFALL_TEMPLE, 2), true),
        },
        .events = {
            EVENT_ACCESS(RANDO_ACCESS_DEKU_PRINCESS, true),
        },
        .oneWayEntrances = {
            ENTRANCE(WOODFALL_TEMPLE, 1), // From boss room
        },
    };
    Regions[RR_WOODFALL_TEMPLE_ENTRANCE] = RandoRegion{ .name = "Entrance", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_ENTRANCE_CHEST, CAN_BE_DEKU || HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_WOODFALL_TEMPLE_SF_ENTRANCE, CAN_USE_PROJECTILE),
            CHECK(RC_WOODFALL_TEMPLE_ENTRANCE_POT, CAN_BE_DEKU),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(WOODFALL, 1),                     ENTRANCE(WOODFALL_TEMPLE, 0), true),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM, CAN_BE_DEKU || HAS_ITEM(ITEM_HOOKSHOT)),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_MAIN_ROOM_UPPER] = RandoRegion{ .name = "Main Room Upper", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_UPPER_POT_01, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_UPPER_POT_02, true),
            CHECK(RC_WOODFALL_TEMPLE_SF_MAIN_BUBBLE, true),
            CHECK(RC_WOODFALL_TEMPLE_CENTER_CHEST, CAN_BE_DEKU),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM_UPPER, true),
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM, true),
            CONNECTION(RR_WOODFALL_TEMPLE_PRE_BOSS_ROOM, CHECK_WEEKEVENTREG(WEEKEVENTREG_12_01)),
            CONNECTION(RR_WOODFALL_TEMPLE_DARK_ROOM, true),
        },
        .events = {
            EVENT(
                "Light Corner Torch", 
                Flags_GetSceneSwitch(SCENE_MITURIN, 0x09), 
                Flags_SetSceneSwitch(SCENE_MITURIN, 0x09), 
                Flags_ClearSceneSwitch(SCENE_MITURIN, 0x09), 
                HAS_ITEM(ITEM_BOW) && CHECK_WEEKEVENTREG(WEEKEVENTREG_12_01)
            ),
            EVENT_WEEKEVENTREG("Light Middle Torch", WEEKEVENTREG_12_01, HAS_ITEM(ITEM_BOW)),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_MAIN_ROOM] = RandoRegion{ .name = "Main Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_01, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_02, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_03, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_04, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_05, true),
            CHECK(RC_WOODFALL_TEMPLE_MAIN_ROOM_LOWER_POT_06, true),
            CHECK(RC_WOODFALL_TEMPLE_SF_MAIN_POT, true),
            CHECK(RC_WOODFALL_TEMPLE_SF_MAIN_DEKU_BABA, CanKillEnemy(ACTOR_EN_DEKUBABA)),
            CHECK(RC_WOODFALL_TEMPLE_SF_MAIN_BUBBLE, (HAS_ITEM(ITEM_BOW) || HAS_ITEM(ITEM_HOOKSHOT))),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_ENTRANCE, true),
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM, true), // It's a little tight for goron but possible
            CONNECTION(RR_WOODFALL_TEMPLE_MAZE_ROOM, KEY_COUNT(WOODFALL_TEMPLE) >= 1),
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM_UPPER, Flags_GetSceneSwitch(SCENE_MITURIN, 0x09) || HAS_ITEM(ITEM_HOOKSHOT)),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_MAP_ROOM] = RandoRegion{ .name = "Map Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_MAP_CHEST, CanKillEnemy(ACTOR_EN_BIGPAMET)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM, true),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_MAZE_ROOM] = RandoRegion{ .name = "Maze Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_MAZE_POT_01, true),
            CHECK(RC_WOODFALL_TEMPLE_MAZE_POT_02, true),
            // TODO: Trick for bombs & chus here - Doesn't need a trick. Bomb can break it from above
            CHECK(RC_WOODFALL_TEMPLE_SF_MAZE_BEEHIVE, CAN_USE_PROJECTILE ||  CAN_USE_EXPLOSIVE),
            // TODO: Maybe add a health check here later
            CHECK(RC_WOODFALL_TEMPLE_SF_MAZE_BUBBLE, true),
            // CanKillEnemy(ACTOR_EN_ST)? - DONE
            CHECK(RC_WOODFALL_TEMPLE_SF_MAZE_SKULLTULA, CanKillEnemy(ACTOR_EN_ST)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM, KEY_COUNT(WOODFALL_TEMPLE) >= 1),
            CONNECTION(RR_WOODFALL_TEMPLE_DARK_ROOM, CAN_LIGHT_TORCH_NEAR_ANOTHER),
            CONNECTION(RR_WOODFALL_TEMPLE_COMPASS_ROOM, CAN_LIGHT_TORCH_NEAR_ANOTHER),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_PRE_BOSS_ROOM] = RandoRegion{ .name = "Pre Boss Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_SF_PRE_BOSS_BOTTOM_RIGHT, CAN_BE_DEKU),
            CHECK(RC_WOODFALL_TEMPLE_SF_PRE_BOSS_LEFT, CAN_BE_DEKU),
            CHECK(RC_WOODFALL_TEMPLE_SF_PRE_BOSS_TOP_RIGHT, CAN_BE_DEKU),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_POT_01, CAN_BE_DEKU && HAS_ITEM(ITEM_BOW)),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_POT_02, CAN_BE_DEKU && HAS_ITEM(ITEM_BOW)),
            CHECK(RC_WOODFALL_TEMPLE_SF_PRE_BOSS_PILLAR, CAN_BE_DEKU && HAS_ITEM(ITEM_BOW)),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_01, CAN_BE_DEKU || CAN_BE_ZORA),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_02, CAN_BE_DEKU || CAN_BE_ZORA),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_03, CAN_BE_DEKU || CAN_BE_ZORA),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_04, CAN_BE_DEKU || CAN_BE_ZORA),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_05, CAN_BE_DEKU && HAS_ITEM(ITEM_BOW) || CAN_BE_ZORA || CAN_USE_EXPLOSIVE),
            CHECK(RC_WOODFALL_TEMPLE_PRE_BOSS_FREESTANDING_RUPEE_06, CAN_BE_DEKU && (CAN_BE_ZORA || CAN_USE_EXPLOSIVE)),
        },
        .exits = { //     TO                                         FROM
            EXIT(ENTRANCE(ODOLWAS_LAIR, 0),                          ONE_WAY_EXIT, CAN_BE_DEKU && HAS_ITEM(ITEM_BOW) && CHECK_DUNGEON_ITEM(DUNGEON_BOSS_KEY, DUNGEON_INDEX_WOODFALL_TEMPLE)),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM_UPPER, CAN_BE_DEKU),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_WATER_ROOM_UPPER] = RandoRegion{ .name = "Water Room Upper", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_WATER_ROOM_POT_01, true),
            CHECK(RC_WOODFALL_TEMPLE_WATER_ROOM_POT_02, true),
            CHECK(RC_WOODFALL_TEMPLE_WATER_ROOM_POT_03, true),
            CHECK(RC_WOODFALL_TEMPLE_WATER_ROOM_POT_04, true),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM, true),
            CONNECTION(RR_WOODFALL_TEMPLE_BOW_ROOM, true),
            CONNECTION(RR_WOODFALL_TEMPLE_BOSS_KEY_ROOM, HAS_ITEM(ITEM_BOW) && CAN_BE_DEKU),
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM_UPPER, true),
        },
    };
    Regions[RR_WOODFALL_TEMPLE_WATER_ROOM] = RandoRegion{ .name = "Water Room", .sceneId = SCENE_MITURIN,
        .checks = {
            CHECK(RC_WOODFALL_TEMPLE_WATER_CHEST, CAN_BE_DEKU || HAS_ITEM(ITEM_HOOKSHOT)),
            CHECK(RC_WOODFALL_TEMPLE_SF_WATER_ROOM_BEEHIVE, (
                // Can they break it, leaving the fairy up high? // TODO: Trick for bombs & chus here
                ((HAS_ITEM(ITEM_HOOKSHOT) || CAN_BE_ZORA) && HAS_ITEM(ITEM_MASK_GREAT_FAIRY)) ||
                // Can they break it, making it drop into the water? --- Only if you make the item drop if not it will float
                (HAS_ITEM(ITEM_BOW) || (CAN_BE_DEKU && HAS_MAGIC))
            )),
        },
        .connections = {
            CONNECTION(RR_WOODFALL_TEMPLE_MAIN_ROOM, true),
            CONNECTION(RR_WOODFALL_TEMPLE_MAP_ROOM, CAN_BE_DEKU),
            CONNECTION(RR_WOODFALL_TEMPLE_WATER_ROOM_UPPER, HAS_ITEM(ITEM_BOW) && CAN_BE_DEKU),
        },
    };
}, {});
// clang-format on
