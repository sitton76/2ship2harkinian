#include <libultraship/bridge.h>
#include "2s2h/GameInteractor/GameInteractor.h"
#include "2s2h/ShipInit.hpp"

extern "C" {
#include "variables.h"
#include "overlays/actors/ovl_En_Test6/z_en_test6.h"

void EnTest6_DoubleSoTCutscene(EnTest6* thisx, PlayState* play);
void EnTest6_InvertedSoTCutscene(EnTest6* thisx, PlayState* play);
void EnTest6_StopDoubleSoTCutscene(EnTest6* thisx, PlayState* play);
void EnTest6_StopInvertedSoTCutscene(EnTest6* thisx, PlayState* play);
}

#define CVAR_NAME "gEnhancements.Songs.SkipSoTCutscenes"
#define CVAR CVarGetInteger(CVAR_NAME, 0)

void RegisterSkipSoTCutscenes() {
    // Song of Time cutscene plays as part of an entrance, so set the time and
    // hijack the entrance to the destination
    COND_VB_SHOULD(VB_PLAY_TRANSITION_CS, CVAR, {
        if ((gSaveContext.save.entrance == ENTRANCE(CUTSCENE, 0) ||
             gSaveContext.save.entrance == ENTRANCE(CUTSCENE, 1)) &&
            gSaveContext.save.cutsceneIndex == 0xFFF7) {
            gSaveContext.save.eventDayCount = 0;
            gSaveContext.save.day = 0;
            gSaveContext.save.time = CLOCK_TIME(6, 0) - 1;

            if (gSaveContext.save.entrance == ENTRANCE(CUTSCENE, 1)) {
                // Loads to flash back montage before going to Dawn of... in clock town
                // Use ENTRANCE(SOUTH_CLOCK_TOWN, 10) if we ever add a story cutscene skip for the flash backs
                gSaveContext.save.entrance = ENTRANCE(LOST_WOODS, 1);
            } else {
                // Directly to Dawn of... in clock town
                gSaveContext.save.entrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
            }

            gSaveContext.save.cutsceneIndex = 0;
        }
    });

    // Skips inverted/double time cutscenes by forcing the actor to stop
    COND_ID_HOOK(OnActorUpdate, ACTOR_EN_TEST6, CVAR, [](Actor* actor) {
        EnTest6* enTest6 = (EnTest6*)actor;

        if (enTest6->actionFunc == EnTest6_DoubleSoTCutscene) {
            enTest6->actionFunc = EnTest6_StopDoubleSoTCutscene;
            Player* player = GET_PLAYER(gPlayState);

            // Respawns the player out when going from day -> night
            // copied from last cutscene cue SOTCS_CUEID_DOUBLE_END in EnTest6_SharedSoTCutscene
            if (gSaveContext.save.time > CLOCK_TIME(12, 0)) {
                Play_SetRespawnData(&gPlayState->state, RESPAWN_MODE_RETURN, gSaveContext.save.entrance,
                                    player->unk_3CE, PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B), &player->unk_3C0,
                                    player->unk_3CC);
                gPlayState->transitionTrigger = TRANS_TRIGGER_START;
                gPlayState->nextEntrance = gSaveContext.respawn[RESPAWN_MODE_RETURN].entrance;
                gPlayState->transitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.respawnFlag = 2;
                gPlayState->msgCtx.ocarinaMode = OCARINA_MODE_END;
            }
        } else if (enTest6->actionFunc == EnTest6_InvertedSoTCutscene) {
            enTest6->actionFunc = EnTest6_StopInvertedSoTCutscene;
        }
    });
}

static RegisterShipInitFunc initFunc(RegisterSkipSoTCutscenes, { CVAR_NAME });
