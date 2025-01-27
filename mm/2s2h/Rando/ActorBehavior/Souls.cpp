#include "Souls.h"
#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
}

std::unordered_map<RandoItemId, ActorId> soulToIdMap = {
    { RI_SOUL_BAT, ACTOR_EN_BAT },
    { RI_SOUL_BOMBCHU, ACTOR_EN_RAT },
    { RI_SOUL_DINOLFOS, ACTOR_EN_DINOFOS },
    { RI_SOUL_DODONGO, ACTOR_EN_DODONGO },
    { RI_SOUL_GARO, ACTOR_EN_JSO2 },
    { RI_SOUL_KEESE, ACTOR_EN_FIREFLY },
    { RI_SOUL_LEEVER, ACTOR_EN_NEO_REEBA },
    { RI_SOUL_OCTOROK, ACTOR_EN_OKUTA },
    { RI_SOUL_PEEHAT, ACTOR_EN_PEEHAT },
    { RI_SOUL_SKULLTULA, ACTOR_EN_ST },
    { RI_SOUL_SLIME, ACTOR_EN_SLIME },
    { RI_SOUL_TEKTITE, ACTOR_EN_TITE },
    { RI_SOUL_WALLMASTER, ACTOR_EN_WALLMAS },
    { RI_SOUL_WOLFOS, ACTOR_EN_WF },
};

bool FindSoul(int16_t actorId) {
    bool isFound = false;

    if (gSaveContext.save.shipSaveInfo.rando.enemySouls[actorId] == 1) {
        isFound = true;
    }

    return isFound;
}

extern void SoulObtained(RandoItemId randoItemId) {
    auto it = soulToIdMap.find(randoItemId);
    if (it != soulToIdMap.end()) {
        gSaveContext.save.shipSaveInfo.rando.enemySouls[it->second] = 1;
    }
}

void Rando::ActorBehavior::InitSoulsBehavior() {
    COND_HOOK(ShouldActorDraw, IS_RANDO, [](Actor* actor, bool* should) {
        if (actor->category != ACTORCAT_ENEMY) {
            return;
        }
    
        if (!FindSoul(actor->id)) {
            actor->flags &= ~ACTOR_FLAG_TARGETABLE;
            *should = false;
        } else {
            actor->flags |= ACTOR_FLAG_TARGETABLE;
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