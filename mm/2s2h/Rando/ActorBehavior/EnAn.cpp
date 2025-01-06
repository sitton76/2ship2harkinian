#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
void func_80837B60(PlayState* play, Player* player);
s32 func_80832558(PlayState* play, Player* player, PlayerFuncD58 arg2);
}

void Rando::ActorBehavior::InitEnAnBehavior() {
    COND_VB_SHOULD(VB_EXEC_MSG_EVENT, IS_RANDO, {
        u32 cmdId = va_arg(args, u32);
        Actor* actor = va_arg(args, Actor*);
        if (actor->id == ACTOR_EN_AN) { // Anju
            MsgScript* script = va_arg(args, MsgScript*);
            Player* player = GET_PLAYER(gPlayState);
            static std::vector<u8> skipCmds = {};

            if (cmdId == MSCRIPT_CMD_06) { // MSCRIPT_OFFER_ITEM
                func_80832558(gPlayState, player, func_80837B60);
                player->talkActor = actor;
                *should = false;
                skipCmds.clear();
                skipCmds.push_back(MSCRIPT_CMD_12); // Have to skip this to prevent a crash
                skipCmds.push_back(MSCRIPT_CMD_07); // And have to skip this to prevent a softlock on repeats
                GetItemId getItemId = (GetItemId)MSCRIPT_GET_16(script, 1);
                // If these are skipped but not manually queued, the game will crash for some reason. This does not
                // happen with the room key check.
                if (getItemId == GI_LETTER_TO_KAFEI) {
                    Message_BombersNotebookQueueEvent(gPlayState, BOMBERS_NOTEBOOK_EVENT_PROMISED_TO_MEET_KAFEI);
                    Message_BombersNotebookQueueEvent(gPlayState, BOMBERS_NOTEBOOK_EVENT_RECEIVED_LETTER_TO_KAFEI);
                }
                return;
            }

            if (skipCmds.empty()) {
                return;
            }

            if (cmdId == skipCmds[0]) {
                skipCmds.erase(skipCmds.begin());
                *should = false;
            }
        }
    });
}
