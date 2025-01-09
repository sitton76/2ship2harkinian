#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
}

bool FindSoul(ActorId actorId) {
    bool isFound = false;
    s8* soulsFound = gSaveContext.save.shipSaveInfo.rando.enemySouls;

    if (soulsFound[actorId] == 1) {
        isFound = true;
    }
    
    //for (auto& soul : gSaveContext.save.shipSaveInfo.rando.enemySouls) {
    //    if (soul == (int8_t)actorId) {
    //        isFound = true;
    //    }
    //}

    return isFound;
}

void Rando::ActorBehavior::InitSoulsBehavior() {
    //COND_ID_HOOK(ShouldActorInit, ACTOR_BOSS_01, IS_RANDO, [](Actor* actor, bool* should) {
    //    if (gPlayState->sceneId == SCENE_MITURIN_BS &&
    //        gSaveContext.save.shipSaveInfo.rando.bossSouls[DUNGEON_INDEX_WOODFALL_TEMPLE] != 1) {
    //        *should = false;
    //    }
    //});
    //COND_ID_HOOK(ShouldActorInit, ACTOR_BOSS_02, IS_RANDO, [](Actor* actor, bool* should) {
    //    if (gPlayState->sceneId == SCENE_INISIE_BS &&
    //        gSaveContext.save.shipSaveInfo.rando.bossSouls[DUNGEON_INDEX_STONE_TOWER_TEMPLE] != 1) {
    //        *should = false;
    //    }
    //});
    //COND_ID_HOOK(ShouldActorInit, ACTOR_BOSS_03, IS_RANDO, [](Actor* actor, bool* should) {
    //    if (gPlayState->sceneId == SCENE_SEA_BS &&
    //        gSaveContext.save.shipSaveInfo.rando.bossSouls[DUNGEON_INDEX_GREAT_BAY_TEMPLE] != 1) {
    //        *should = false;
    //    }
    //});
    //COND_ID_HOOK(ShouldActorInit, ACTOR_BOSS_HAKUGIN, IS_RANDO, [](Actor* actor, bool* should) {
    //    if (gPlayState->sceneId == SCENE_HAKUGIN_BS &&
    //        gSaveContext.save.shipSaveInfo.rando.bossSouls[DUNGEON_INDEX_SNOWHEAD_TEMPLE] != 1) {
    //        *should = false;
    //    }
    //});

    COND_ID_HOOK(ShouldActorInit, ACTOR_EN_SLIME, IS_RANDO, [](Actor* actor, bool* should) {
        if (!FindSoul(ACTOR_EN_SLIME)) {
            *should = false;
        }
    });
}