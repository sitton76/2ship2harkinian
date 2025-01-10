#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
}

bool FindSoul(int16_t actorId) {
    bool isFound = false;

    if (gSaveContext.save.shipSaveInfo.rando.enemySouls[actorId] == 1) {
        isFound = true;
    }

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

    COND_HOOK(ShouldActorDraw, IS_RANDO, [](Actor* actor, bool* should) {
        if (actor->category != ACTORCAT_ENEMY) {
            return;
        }

        if (!FindSoul(actor->id)) {
            actor->flags &= ~ACTOR_FLAG_TARGETABLE;
            *should = false;
        } 
    });
    COND_HOOK(ShouldActorUpdate, IS_RANDO, [](Actor* actor, bool* should) {
        if (actor->category != ACTORCAT_ENEMY) {
            return;
        }

        if (!FindSoul(actor->id)) {
            *should = false;
        } 
    });
}