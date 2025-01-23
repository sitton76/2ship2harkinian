#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
void func_80837B60(PlayState* play, Player* player);
s32 func_80832558(PlayState* play, Player* player, PlayerFuncD58 arg2);
}

static std::vector<u8> skipCmds = {};

void Rando::ActorBehavior::InitEnAnBehavior() {

    COND_ID_HOOK(OnActorInit, ACTOR_EN_AN, IS_RANDO, [](Actor* actor) { skipCmds.clear(); });

    COND_VB_SHOULD(VB_EXEC_MSG_EVENT, IS_RANDO, {
        u32 cmdId = va_arg(args, u32);
        Actor* actor = va_arg(args, Actor*);
        if (actor->id == ACTOR_EN_AN) { // Anju
            MsgScript* script = va_arg(args, MsgScript*);
            Player* player = GET_PLAYER(gPlayState);

            if (cmdId == MSCRIPT_CMD_06) { // MSCRIPT_OFFER_ITEM
                func_80832558(gPlayState, player, func_80837B60);
                *should = false;
                skipCmds.clear();
                skipCmds.push_back(MSCRIPT_CMD_12); // Have to skip this to prevent a crash
                skipCmds.push_back(MSCRIPT_CMD_07); // And have to skip this to prevent a softlock on repeats
                GetItemId getItemId = (GetItemId)MSCRIPT_GET_16(script, 1);
                /*
                 * If the player has the Bombers' Notebook and this is the Letter to Kafei check, the game will crash
                 * unless player->talkActor is set AND these skipped notebook events are queued. This does not happen
                 * with the Room Key check.
                 */
                if (CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK) && getItemId == GI_LETTER_TO_KAFEI) {
                    player->talkActor = actor;
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
