#include "Traps.h"
#include "MiscBehavior.h"
#include "2s2h/DeveloperTools/SaveEditor.h"

extern "C" {
#include "variables.h"
#include "functions.h"
void func_80833B18(PlayState* play, Player* thisx, s32 arg2, f32 speed, f32 velocityY, s16 arg5,
                   s32 invincibilityTimer);
}

int roll = TRAP_FREEZE;
const u16 TimeSkipInc = 400;

int RollTrapType() {
    roll = rand() % TRAP_MAX;
    return roll;
}

std::vector<std::string> freezeTrapMessages = {
    "This item is available in the %bRando DLC%w.",
};

std::vector<std::string> blastTrapMessages = {
    "Coming to you live from the %yThunderdome%w!",
    "There was supposed to be an Earth shattering %yKaboom%w!",
};

std::vector<std::string> shockTrapMessages = {
    "We're losing him!\n%gCLEAR%w",
};

std::vector<std::string> timeTrapMessages = {
    "Time flashes before your eyes!",
    "You have played the Sun's Song!",
    "The Goddess of Time smites you!",
    "Spent an hour and a half admiring this fake item.",
};

std::map<TrapTypes, std::vector<std::string>> trapMessageList = {
    { TRAP_FREEZE, freezeTrapMessages },
    { TRAP_BLAST, blastTrapMessages },
    { TRAP_SHOCK, shockTrapMessages },
    { TRAP_TIME, timeTrapMessages },
};

std::string GetTrapMessage() {
    RollTrapType();
    std::vector<std::string> trapMessages = trapMessageList.at((TrapTypes)roll);
    return trapMessages[rand() % trapMessages.size()];
}

void VerifyTimeSkip(u16 gameTime) {
    // Prevents weirdness if multiple time skips are triggered around the same time.
    if (gSaveContext.save.time <= gameTime) {
        UpdateGameTime(gameTime);
    }
}

void Rando::MiscBehavior::OfferTrapItem() {
    if (!gPlayState) {
        return;
    }

    switch (roll) {
        case TRAP_FREEZE:
            GameInteractor::Instance->events.emplace_back(
                GIEventTrap{ .action = []() { func_80833B18(gPlayState, GET_PLAYER(gPlayState), 3, 0, 0, 0, 0); } });
            break;
        case TRAP_BLAST:
            GameInteractor::Instance->events.emplace_back(GIEventTrap{ .action = []() {
                Actor_Spawn(&gPlayState->actorCtx, gPlayState, ACTOR_EN_BOM, GET_PLAYER(gPlayState)->actor.world.pos.x,
                            GET_PLAYER(gPlayState)->actor.world.pos.y, GET_PLAYER(gPlayState)->actor.world.pos.z, 1, 0,
                            0, 0);
            } });
            break;
        case TRAP_SHOCK:
            GameInteractor::Instance->events.emplace_back(
                GIEventTrap{ .action = []() { func_80833B18(gPlayState, GET_PLAYER(gPlayState), 4, 0, 0, 0, 0); } });
            break;
        case TRAP_TIME:
            for (u16 i = gSaveContext.save.time; i <= gSaveContext.save.time + (TimeSkipInc * 10); i += TimeSkipInc) {
                GameInteractor::Instance->events.emplace_back(GIEventTrap{ .action = [i]() { VerifyTimeSkip(i); } });
            }
            break;
        default:
            break;
    }
}