#include "GameInteractor.h"
#include "spdlog/spdlog.h"
#include <libultraship/bridge.h>
#include <variant>
#include "2s2h/Enhancements/FrameInterpolation/FrameInterpolation.h"
#include "2s2h/CustomMessage/CustomMessage.h"

extern "C" {
#include "z64.h"
#include "macros.h"
#include "functions.h"
extern SaveContext gSaveContext;
extern PlayState* gPlayState;
void func_800A6A40(EnItem00* thisx, PlayState* play);
extern Vec3f sPlayerGetItemRefPos;
}

void GameInteractor_ExecuteOnGameStateMainFinish() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnGameStateMainFinish>();
}

void GameInteractor_ExecuteOnGameStateDrawFinish() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnGameStateDrawFinish>();
}

void GameInteractor_ExecuteOnGameStateUpdate() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnGameStateUpdate>();
}

void GameInteractor_ExecuteOnConsoleLogoUpdate() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnConsoleLogoUpdate>();
}

void GameInteractor_ExecuteOnKaleidoUpdate(PauseContext* pauseCtx) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnKaleidoUpdate>(pauseCtx);
}

void GameInteractor_ExecuteBeforeKaleidoDrawPage(PauseContext* pauseCtx, u16 pauseIndex) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::BeforeKaleidoDrawPage>(pauseCtx, pauseIndex);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::BeforeKaleidoDrawPage>(pauseIndex, pauseCtx,
                                                                                       pauseIndex);
}

void GameInteractor_ExecuteAfterKaleidoDrawPage(PauseContext* pauseCtx, u16 pauseIndex) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::AfterKaleidoDrawPage>(pauseCtx, pauseIndex);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::AfterKaleidoDrawPage>(pauseIndex, pauseCtx, pauseIndex);
}

void GameInteractor_ExecuteOnSaveInit(s16 fileNum) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnSaveInit>(fileNum);
}

void GameInteractor_ExecuteOnSaveLoad(s16 fileNum) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnSaveLoad>(fileNum);
}

void GameInteractor_ExecuteBeforeEndOfCycleSave() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::BeforeEndOfCycleSave>();
}

void GameInteractor_ExecuteAfterEndOfCycleSave() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::AfterEndOfCycleSave>();
}

void GameInteractor_ExecuteBeforeMoonCrashSaveReset() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::BeforeMoonCrashSaveReset>();
}

void GameInteractor_ExecuteOnSceneInit(s16 sceneId, s8 spawnNum) {
    SPDLOG_DEBUG("OnSceneInit: sceneId: {}, spawnNum: {}", sceneId, spawnNum);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnSceneInit>(sceneId, spawnNum);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnSceneInit>(sceneId, sceneId, spawnNum);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnSceneInit>(sceneId, spawnNum);
}

void GameInteractor_ExecuteOnRoomInit(s16 sceneId, s8 roomNum) {
    SPDLOG_DEBUG("OnRoomInit: sceneId: {}, roomNum: {}", sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnRoomInit>(sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnRoomInit>(sceneId, sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnRoomInit>(sceneId, roomNum);
}

void GameInteractor_ExecuteAfterRoomSceneCommands(s16 sceneId, s8 roomNum) {
    SPDLOG_DEBUG("AfterRoomSceneCommands: sceneId: {}, roomNum: {}", sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::AfterRoomSceneCommands>(sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::AfterRoomSceneCommands>(sceneId, sceneId, roomNum);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::AfterRoomSceneCommands>(sceneId, roomNum);
}

void GameInteractor_ExecuteOnPlayDrawWorldEnd() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnPlayDrawWorldEnd>();
}

void GameInteractor_ExecuteOnPlayDestroy() {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnPlayDestroy>();
}

bool GameInteractor_ShouldActorInit(Actor* actor) {
    bool result = true;
    GameInteractor::Instance->ExecuteHooks<GameInteractor::ShouldActorInit>(actor, &result);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::ShouldActorInit>(actor->id, actor, &result);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::ShouldActorInit>((uintptr_t)actor, actor, &result);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::ShouldActorInit>(actor, &result);
    return result;
}

void GameInteractor_ExecuteOnActorInit(Actor* actor) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnActorInit>(actor);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnActorInit>(actor->id, actor);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnActorInit>((uintptr_t)actor, actor);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnActorInit>(actor);
}

bool GameInteractor_ShouldActorUpdate(Actor* actor) {
    bool result = true;
    GameInteractor::Instance->ExecuteHooks<GameInteractor::ShouldActorUpdate>(actor, &result);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::ShouldActorUpdate>(actor->id, actor, &result);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::ShouldActorUpdate>((uintptr_t)actor, actor, &result);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::ShouldActorUpdate>(actor, &result);
    return result;
}

void GameInteractor_ExecuteOnActorUpdate(Actor* actor) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnActorUpdate>(actor);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnActorUpdate>(actor->id, actor);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnActorUpdate>((uintptr_t)actor, actor);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnActorUpdate>(actor);
}

bool GameInteractor_ShouldActorDraw(Actor* actor) {
    bool result = true;
    GameInteractor::Instance->ExecuteHooks<GameInteractor::ShouldActorDraw>(actor, &result);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::ShouldActorDraw>(actor->id, actor, &result);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::ShouldActorDraw>((uintptr_t)actor, actor, &result);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::ShouldActorDraw>(actor, &result);
    return result;
}

void GameInteractor_ExecuteOnActorDraw(Actor* actor) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnActorDraw>(actor);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnActorDraw>(actor->id, actor);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnActorDraw>((uintptr_t)actor, actor);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnActorDraw>(actor);
}

void GameInteractor_ExecuteOnActorKill(Actor* actor) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnActorKill>(actor);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnActorKill>(actor->id, actor);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnActorKill>((uintptr_t)actor, actor);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnActorKill>(actor);
}

void GameInteractor_ExecuteOnActorDestroy(Actor* actor) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnActorDestroy>(actor);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnActorDestroy>(actor->id, actor);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnActorDestroy>((uintptr_t)actor, actor);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnActorDestroy>(actor);
}

void GameInteractor_ExecuteOnPlayerPostLimbDraw(Player* player, s32 limbIndex) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnPlayerPostLimbDraw>(player, limbIndex);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnPlayerPostLimbDraw>(limbIndex, player, limbIndex);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnPlayerPostLimbDraw>(player, limbIndex);
}

void GameInteractor_ExecuteOnSceneFlagSet(s16 sceneId, FlagType flagType, u32 flag) {
    SPDLOG_DEBUG("OnSceneFlagSet: sceneId: {}, flagType: {}, flag: {}", sceneId, (u32)flagType, flag);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnSceneFlagSet>(sceneId, flagType, flag);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnSceneFlagSet>(sceneId, flagType, flag);
}

void GameInteractor_ExecuteOnSceneFlagUnset(s16 sceneId, FlagType flagType, u32 flag) {
    SPDLOG_DEBUG("OnSceneFlagUnset: sceneId: {}, flagType: {}, flag: {}", sceneId, (u32)flagType, flag);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnSceneFlagUnset>(sceneId, flagType, flag);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnSceneFlagUnset>(sceneId, flagType, flag);
}

void GameInteractor_ExecuteOnFlagSet(FlagType flagType, u32 flag) {
    // This flag in particular is very spammy, so we'll suppress it
    if (!(flagType == FLAG_WEEK_EVENT_REG && flag == WEEKEVENTREG_92_80)) {
        SPDLOG_DEBUG("OnFlagSet: flagType: {}, flag: {}", (u32)flagType, flag);
    }
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnFlagSet>(flagType, flag);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnFlagSet>(flagType, flag);
}

void GameInteractor_ExecuteOnFlagUnset(FlagType flagType, u32 flag) {
    // This flag in particular is very spammy, so we'll suppress it
    if (!(flagType == FLAG_WEEK_EVENT_REG && flag == WEEKEVENTREG_92_80)) {
        SPDLOG_DEBUG("OnFlagUnset: flagType: {}, flag: {}", (u32)flagType, flag);
    }
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnFlagUnset>(flagType, flag);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnFlagUnset>(flagType, flag);
}

void GameInteractor_ExecuteOnCameraChangeModeFlags(Camera* camera) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnCameraChangeModeFlags>(camera);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnCameraChangeModeFlags>(camera->uid, camera);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnCameraChangeModeFlags>((uintptr_t)camera, camera);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnCameraChangeModeFlags>(camera);
}

void GameInteractor_ExecuteAfterCameraUpdate(Camera* camera) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::AfterCameraUpdate>(camera);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::AfterCameraUpdate>(camera->uid, camera);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::AfterCameraUpdate>((uintptr_t)camera, camera);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::AfterCameraUpdate>(camera);
}

void GameInteractor_ExecuteOnCameraChangeSettingsFlags(Camera* camera) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnCameraChangeSettingsFlags>(camera);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnCameraChangeSettingsFlags>(camera->uid, camera);
    GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::OnCameraChangeSettingsFlags>((uintptr_t)camera,
                                                                                              camera);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnCameraChangeSettingsFlags>(camera);
}

void GameInteractor_ExecuteOnPassPlayerInputs(Input* input) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnPassPlayerInputs>(input);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnPassPlayerInputs>(input);
}

void GameInteractor_ExecuteOnOpenText(u16* textId) {
    SPDLOG_DEBUG("OnOpenText: textId: {}", *textId);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnOpenText>(textId);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnOpenText>(*textId, textId);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnOpenText>(textId);
}

void GameInteractor_ExecuteOnHandleCustomMessage(s32 modId, s32 textId, std::string* msg) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnHandleCustomMessage>(modId, textId, msg);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnHandleCustomMessage>(modId, textId, msg);
}

bool GameInteractor_ShouldItemGive(u8 item) {
    bool result = true;
    GameInteractor::Instance->ExecuteHooks<GameInteractor::ShouldItemGive>(item, &result);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::ShouldItemGive>(item, item, &result);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::ShouldItemGive>(item, &result);
    return result;
}

void GameInteractor_ExecuteOnItemGive(u8 item) {
    SPDLOG_DEBUG("OnItemGive: item: {}", item);
    GameInteractor::Instance->ExecuteHooks<GameInteractor::OnItemGive>(item);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::OnItemGive>(item, item);
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::OnItemGive>(item);
}

bool GameInteractor_Should(GIVanillaBehavior flag, bool result, void* opt) {
    GameInteractor::Instance->ExecuteHooks<GameInteractor::ShouldVanillaBehavior>(flag, &result, opt);
    GameInteractor::Instance->ExecuteHooksForID<GameInteractor::ShouldVanillaBehavior>(flag, flag, &result, opt);
    if (opt != nullptr) {
        GameInteractor::Instance->ExecuteHooksForPtr<GameInteractor::ShouldVanillaBehavior>((uintptr_t)opt, flag,
                                                                                            &result, opt);
    }
    GameInteractor::Instance->ExecuteHooksForFilter<GameInteractor::ShouldVanillaBehavior>(flag, &result, opt);
    return result;
}

// Returns 1 or -1 based on a number of factors like CVars or other game states.
int GameInteractor_InvertControl(GIInvertType type) {
    int result = 1;

    switch (type) {
        case GI_INVERT_CAMERA_RIGHT_STICK_X:
            if (CVarGetInteger("gEnhancements.Camera.RightStick.InvertXAxis", 0)) {
                result *= -1;
            }
            break;
        case GI_INVERT_CAMERA_RIGHT_STICK_Y:
            if (CVarGetInteger("gEnhancements.Camera.RightStick.InvertYAxis", 1)) {
                result *= -1;
            }
            break;
    }

    /*
    // Invert all X axis inputs if the Mirrored World mode is enabled
    if (CVarGetInteger("gModes.MirroredWorld.State", 0)) {
        switch (type) {
            case GI_INVERT_CAMERA_RIGHT_STICK_X:
                result *= -1;
                break;
        }
    }
    */

    /*
    if (CrowdControl::State::InvertedInputs) {
        result *= -1;
    }
    */

    return result;
}

uint32_t GameInteractor_Dpad(GIDpadType type, uint32_t buttonCombo) {
    uint32_t result = 0;

    switch (type) {
        case GI_DPAD_OCARINA:
            if (CVarGetInteger("gEnhancements.Playback.DpadOcarina", 0)) {
                result = buttonCombo;
            }
            break;
        case GI_DPAD_EQUIP:
            if (CVarGetInteger("gEnhancements.Dpad.DpadEquips", 0)) {
                result = buttonCombo;
            }
            break;
    }

    return result;
}

void EnItem00_DrawCustomForOverHead(Actor* thisx, PlayState* play) {
    EnItem00* enItem00 = (EnItem00*)thisx;
    thisx->shape.rot.y += 0x3C0;
    Matrix_Scale(30.0f, 30.0f, 30.0f, MTXMODE_APPLY);
    Matrix_Translate(0.0f, -50.0f, 0.0f, MTXMODE_APPLY);
    GetItem_Draw(play, GID_SHIP);
}

void GameInteractor_ProcessEvents(Actor* actor) {
    Player* player = GET_PLAYER(gPlayState);

    // If the player has a message active, stop
    if (gPlayState->msgCtx.msgMode != 0) {
        return;
    }

    // If the player is in a blocking cutscene, stop
    if (Player_InBlockingCsMode(gPlayState, player)) {
        return;
    }

    // If the player is not on the solid ground, stop
    if (!(player->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        return;
    }

    // If there is an event active, stop
    const auto& currentEvent = GameInteractor::Instance->currentEvent;
    bool shouldReturn = false;
    std::visit(
        [&](auto&& e) {
            using T = std::decay_t<decltype(e)>;
            if constexpr (!std::is_same_v<T, GIEventNone>) {
                shouldReturn = true;
            }
            // additionally if the active event is a give item event with a cutscene, clear the current event
            if constexpr (std::is_same_v<T, GIEventGiveItem>) {
                if (e.showGetItemCutscene) {
                    GameInteractor::Instance->currentEvent = GIEventNone{};
                }
            }
        },
        currentEvent);
    if (shouldReturn) {
        return;
    }

    // If there are no events that need to happen, stop
    if (GameInteractor::Instance->events.empty()) {
        return;
    }

    GameInteractor::Instance->currentEvent = GameInteractor::Instance->events.front();
    const auto& nextEvent = GameInteractor::Instance->currentEvent;
    std::visit(
        [&](auto&& e) {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, GIEventGiveItem>) {
                if (e.showGetItemCutscene) {
                    EnItem00* enItem00 = (EnItem00*)Item_DropCollectible(gPlayState, &actor->world.pos, ITEM00_NOTHING);
                    enItem00->getItemId = GI_SHIP;
                    enItem00->actionFunc = func_800A6A40;
                    CustomMessage_SetActiveMessage(MOD_ID_SHIP, SHIP_TEXT_GIVE_ITEM);
                    Actor_OfferGetItem(&enItem00->actor, gPlayState, GI_SHIP, 50.0f, 20.0f);
                    enItem00->actionFunc(enItem00, gPlayState);
                    e.giveItem();
                } else {
                    EnItem00* enItem00 =
                        (EnItem00*)Item_DropCollectible(gPlayState, &actor->world.pos, ITEM00_NOTHING | 0x8000);
                    Audio_PlaySfx(NA_SE_SY_GET_ITEM);
                    CustomMessage_StartTextbox(gPlayState, MOD_ID_SHIP, SHIP_TEXT_GIVE_ITEM_NO_STOP, &enItem00->actor);
                    e.giveItem();
                    enItem00->actor.draw = EnItem00_DrawCustomForOverHead;
                }
            } else if constexpr (std::is_same_v<T, GIEventTransition>) {
                gPlayState->nextEntrance = e.entrance;
                gSaveContext.nextCutsceneIndex = e.cutsceneIndex;
                gPlayState->transitionTrigger = e.transitionTrigger;
                gPlayState->transitionType = e.transitionType;
            }
        },
        nextEvent);

    GameInteractor::Instance->events.erase(GameInteractor::Instance->events.begin());
}

void GameInteractor::Init() {
    GameInteractor::Instance->RegisterGameHookForID<GameInteractor::OnActorUpdate>(ACTOR_PLAYER,
                                                                                   GameInteractor_ProcessEvents);

    GameInteractor::Instance->RegisterGameHookForID<GameInteractor::ShouldVanillaBehavior>(
        GI_VB_GIVE_ITEM_FROM_ITEM00, [](GIVanillaBehavior _, bool* should, void* opt) {
            EnItem00* item00 = static_cast<EnItem00*>(opt);
            if (item00->actor.params == ITEM00_NOTHING || item00->actor.params == (ITEM00_NOTHING | 0x8000)) {
                *should = false;
            }
        });

    // If it's a give item event without a cutscene, clear the current event
    GameInteractor::Instance->RegisterGameHookForID<GameInteractor::OnActorKill>(ACTOR_EN_ITEM00, [](Actor* actor) {
        if (actor->params == 24) {
            const auto& currentEvent = GameInteractor::Instance->currentEvent;
            std::visit(
                [&](auto&& e) {
                    using T = std::decay_t<decltype(e)>;
                    if constexpr (std::is_same_v<T, GIEventGiveItem>) {
                        if (!e.showGetItemCutscene) {
                            GameInteractor::Instance->currentEvent = GIEventNone{};
                        }
                    }
                },
                currentEvent);
        }
    });

    // If there is a transition event active, clear it
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnSceneInit>([](s8 sceneId, s8 spawnNum) {
        const auto& currentEvent = GameInteractor::Instance->currentEvent;
        std::visit(
            [&](auto&& e) {
                using T = std::decay_t<decltype(e)>;
                if constexpr (std::is_same_v<T, GIEventTransition>) {
                    GameInteractor::Instance->currentEvent = GIEventNone{};
                }
            },
            currentEvent);
    });

    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnHandleCustomMessage>(
        [](s32 modId, s32 textId, std::string* msg) {
            // Replaces {{item}} in msg arg with currentEvent getItemText
            const auto& currentEvent = GameInteractor::Instance->currentEvent;
            std::visit(
                [&](auto&& e) {
                    using T = std::decay_t<decltype(e)>;
                    if constexpr (std::is_same_v<T, GIEventGiveItem>) {
                        CustomMessage_Replace(msg, "{{item}}", e.getItemText);
                    }
                },
                currentEvent);
        });
}

void GameInteractor_GetItemDraw(PlayState* play, s16 drawId) {
    const auto& currentEvent = GameInteractor::Instance->currentEvent;
    std::visit(
        [&](auto&& e) {
            using T = std::decay_t<decltype(e)>;
            if constexpr (std::is_same_v<T, GIEventGiveItem>) {
                e.drawItem();
            }
        },
        currentEvent);
}
