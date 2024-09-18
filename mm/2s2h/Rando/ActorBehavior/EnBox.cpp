#include "ActorBehavior.h"
#include <libultraship/libultraship.h>

extern "C" {
#include "variables.h"

s32 func_80832558(PlayState* play, Player* player, PlayerFuncD58 arg2);
void Player_SetAction_PreserveMoveFlags(PlayState* play, Player* player, PlayerActionFunc actionFunc, s32 arg3);
void Player_StopCutscene(Player* player);
void func_80848294(PlayState* play, Player* player);
}

void Player_Action_65_override(Player* player, PlayState* play) {
    if (PlayerAnimation_Update(play, &player->skelAnime)) {
        Player_StopCutscene(player);
        func_80848294(play, player);
    }
}

void func_80837C78_override(PlayState* play, Player* player) {
    Player_SetAction_PreserveMoveFlags(play, player, Player_Action_65_override, 0);
    player->stateFlags1 |= (PLAYER_STATE1_400 | PLAYER_STATE1_20000000);
}

// This simply prevents the player from getting an item from the chest, but still
// plays the chest opening animation and ensure the treasure chest flag is set
void Rando::ActorBehavior::InitEnBoxBehavior(bool isRando) {
    static uint32_t shouldHookId = 0;
    GameInteractor::Instance->UnregisterGameHookForID<GameInteractor::ShouldVanillaBehavior>(shouldHookId);

    shouldHookId = 0;

    if (!isRando) {
        return;
    }

    shouldHookId = REGISTER_VB_SHOULD(GI_VB_GIVE_ITEM_FROM_CHEST, {
        EnBox* enBox = static_cast<EnBox*>(opt);
        Player* player = GET_PLAYER(gPlayState);
        func_80832558(gPlayState, player, func_80837C78_override);
        *should = false;
    });
}
