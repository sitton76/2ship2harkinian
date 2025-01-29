#include "Souls.h"
#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

#include "2s2h/Rando/DrawFuncs.h"

extern "C" {
#include "variables.h"
}

std::unordered_map<RandoItemId, std::pair<std::function<void()>, ActorId>> soulMap = {
    { RI_SOUL_ARMOS, { DrawArmos, ACTOR_EN_AM } },
    { RI_SOUL_BAT, { DrawBat, ACTOR_EN_BAT } },
    { RI_SOUL_BOMBCHU, { DrawRealBombchu, ACTOR_EN_RAT } },
    { RI_SOUL_DEATH_ARMOS, { DrawDeathArmos, ACTOR_EN_FAMOS } },
    { RI_SOUL_DEKU_BABA, { DrawDekuBaba, ACTOR_EN_DEKUBABA } },
    { RI_SOUL_DINOLFOS, { DrawDinolfos, ACTOR_EN_DINOFOS } },
    { RI_SOUL_DODONGO, { DrawDodongo, ACTOR_EN_DODONGO } },
    { RI_SOUL_GARO, { DrawGaroMaster, ACTOR_EN_JSO2 } },
    { RI_SOUL_KEESE, { DrawKeese, ACTOR_EN_FIREFLY } },
    { RI_SOUL_LEEVER, { DrawLeever, ACTOR_EN_NEO_REEBA } },
    { RI_SOUL_MAD_SCRUB, { DrawMadScrub, ACTOR_EN_DEKUNUTS } },
    { RI_SOUL_OCTOROK, { DrawOctorok, ACTOR_EN_OKUTA } },
    { RI_SOUL_PEEHAT, { DrawPeehat, ACTOR_EN_PEEHAT } },
    { RI_SOUL_SKULLTULA, { DrawSkulltula, ACTOR_EN_ST } },
    { RI_SOUL_SLIME, { DrawSlime, ACTOR_EN_SLIME } },
    { RI_SOUL_TEKTITE, { DrawTektite, ACTOR_EN_TITE } },
    { RI_SOUL_WALLMASTER, { DrawWallmaster, ACTOR_EN_WALLMAS } },
    { RI_SOUL_WOLFOS, { DrawWolfos, ACTOR_EN_WF } },
};

bool FindSoul(int16_t actorId) {
    bool isFound = false;

    if (gSaveContext.save.shipSaveInfo.rando.enemySouls[actorId] == 1) {
        isFound = true;
    }

    return isFound;
}

extern void SoulObtained(RandoItemId randoItemId) {
    auto it = soulMap.find(randoItemId);
    if (it != soulMap.end()) {
        gSaveContext.save.shipSaveInfo.rando.enemySouls[it->second.second] = 1;
    }
}

extern void DrawSoul(RandoItemId randoItemId) {
    auto it = soulMap.find(randoItemId);
    if (it != soulMap.end()) {
        it->second.first();
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