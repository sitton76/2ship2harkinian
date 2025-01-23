#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"
#include "src/overlays/actors/ovl_En_Trt/z_en_trt.h"
#include "src/overlays/actors/ovl_En_Trt2/z_en_trt2.h"
void Player_TalkWithPlayer(PlayState* play, Actor* actor);
void EnTrt_ItemGiven(EnTrt* enTrt, PlayState* play);
}

void EnTrt_CompleteDialogue(Actor* actor) {
    EnTrt* refActor = (EnTrt*)actor;
    Player_TalkWithPlayer(gPlayState, &refActor->actor);
    refActor->actionFunc = EnTrt_ItemGiven;
}

void EnTrt2_CompleteDialogue(Actor* actor) {
    EnTrt2* refActor = (EnTrt2*)actor;
    refActor->unk_3B2 = 13;
    Player_TalkWithPlayer(gPlayState, &refActor->actor);
}

void Rando::ActorBehavior::InitEnTrtBehavior() {
    COND_VB_SHOULD(VB_GIVE_ITEM_FROM_OFFER, IS_RANDO, {
        GetItemId* item = va_arg(args, GetItemId*);
        Actor* actor = va_arg(args, Actor*);
        Player* player = GET_PLAYER(gPlayState);

        if (actor->id != ACTOR_EN_TRT && actor->id != ACTOR_EN_TRT2) {
            return;
        }
        if (!RANDO_SAVE_CHECKS[RC_HAGS_POTION_SHOP_KOTAKE].shuffled ||
            RANDO_SAVE_CHECKS[RC_HAGS_POTION_SHOP_KOTAKE].cycleObtained) {
            return;
        }

        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE);
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME);
        *should = false;

        actor->parent = &player->actor;
        player->talkActor = actor;
        player->talkActorDistance = actor->xzDistToPlayer;
        player->exchangeItemAction = PLAYER_IA_MINUS1;

        if (actor->id == ACTOR_EN_TRT) {
            EnTrt_CompleteDialogue(actor);
        } else {
            EnTrt2_CompleteDialogue(actor);
        }
    });
}
