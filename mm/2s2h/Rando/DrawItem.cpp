#include "Rando/Rando.h"
#include <libultraship/libultraship.h>
#include "2s2h/Enhancements/FrameInterpolation/FrameInterpolation.h"
#include "2s2h/ShipInit.hpp"

#include "2s2h/Rando/ActorBehavior/Souls.h"

extern "C" {
#include "variables.h"
#include "functions.h"
#include "objects/gameplay_keep/gameplay_keep.h"
#include "objects/object_gi_melody/object_gi_melody.h"
#include "assets/objects/object_gi_key/object_gi_key.h"
#include "assets/objects/object_gi_bosskey/object_gi_bosskey.h"
#include "objects/object_gi_hearts/object_gi_hearts.h"
#include "objects/object_gi_liquid/object_gi_liquid.h"
#include "objects/object_sek/object_sek.h"
#include "objects/object_st/object_st.h"

Gfx* ResourceMgr_LoadGfxByName(const char* path);
}

s32 StrayFairyOverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                               Gfx** gfx) {
    if (limbIndex == STRAY_FAIRY_LIMB_RIGHT_FACING_HEAD) {
        *dList = NULL;
    }

    return false;
}

void DrawStrayFairy(RandoItemId randoItemId) {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    switch (randoItemId) {
        case RI_WOODFALL_STRAY_FAIRY:
            AnimatedMat_Draw(gPlayState, (AnimatedMaterial*)&gStrayFairyWoodfallTexAnim);
            break;
        case RI_SNOWHEAD_STRAY_FAIRY:
            AnimatedMat_Draw(gPlayState, (AnimatedMaterial*)&gStrayFairySnowheadTexAnim);
            break;
        case RI_GREAT_BAY_STRAY_FAIRY:
            AnimatedMat_Draw(gPlayState, (AnimatedMaterial*)&gStrayFairyGreatBayTexAnim);
            break;
        case RI_STONE_TOWER_STRAY_FAIRY:
            AnimatedMat_Draw(gPlayState, (AnimatedMaterial*)&gStrayFairyStoneTowerTexAnim);
            break;
        default: // STRAY_FAIRY_AREA_CLOCK_TOWN
            AnimatedMat_Draw(gPlayState, (AnimatedMaterial*)&gStrayFairyClockTownTexAnim);
            break;
    }

    Matrix_ReplaceRotation(&gPlayState->billboardMtxF);
    Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);

    // Kind of a hack to draw the stray fairy, the drawback of this is that all stray fairies in the scene will animate
    // together, but worse is that the more there are the faster their animation will play (because of the
    // SkelAnime_Update below). This is still better than the previous solution which hand drew the fairy with DL
    // calls...
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[STRAY_FAIRY_LIMB_MAX];
    static u32 lastUpdate = 0;
    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gStrayFairySkel,
                           (AnimationHeader*)&gStrayFairyFlyingAnim, jointTable, jointTable, STRAY_FAIRY_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    POLY_XLU_DISP = SkelAnime_DrawFlex(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount,
                                       StrayFairyOverrideLimbDraw, NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawSong(RandoItemId randoItemId) {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    switch (randoItemId) {
        case RI_SONG_SUN:
            gDPSetEnvColor(POLY_XLU_DISP++, 237, 231, 62, 255);
            break;
        case RI_SONG_TIME:
            gDPSetEnvColor(POLY_XLU_DISP++, 98, 177, 211, 255);
            break;
        case RI_SONG_HEALING:
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 192, 203, 255);
            break;
        case RI_SONG_STORMS:
            gDPSetEnvColor(POLY_XLU_DISP++, 146, 146, 146, 255);
            break;
        case RI_SONG_SONATA:
            gDPSetEnvColor(POLY_XLU_DISP++, 98, 255, 98, 255);
            break;
        case RI_SONG_SOARING:
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 177, 177, 255);
            break;
        case RI_SONG_ELEGY:
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 98, 0, 255);
            break;
        case RI_SONG_LULLABY_INTRO:
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 194, 194, 255);
            break;
        case RI_SONG_LULLABY:
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 100, 100, 255);
            break;
        case RI_SONG_OATH:
            gDPSetEnvColor(POLY_XLU_DISP++, 98, 0, 98, 255);
            break;
        case RI_SONG_EPONA:
            gDPSetEnvColor(POLY_XLU_DISP++, 146, 87, 49, 255);
            break;
        case RI_SONG_NOVA:
            gDPSetEnvColor(POLY_XLU_DISP++, 20, 20, 255, 255);
            break;
        default:
            break;
    }

    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)&gGiSongNoteDL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawDoubleDefense() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetGrayscaleColor(POLY_XLU_DISP++, 255, 255, 255, 255);
    gSPGrayscale(POLY_XLU_DISP++, true);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)&gGiHeartBorderDL);
    gDPSetGrayscaleColor(POLY_XLU_DISP++, 255, 0, 0, 100);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)&gGiHeartContainerDL);
    gSPGrayscale(POLY_XLU_DISP++, false);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawMilkRefill() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08,
               (uintptr_t)Gfx_TwoTexScroll(gPlayState->state.gfxCtx, G_TX_RENDERTILE, -gPlayState->state.frames,
                                           gPlayState->state.frames, 32, 32, 1, -gPlayState->state.frames,
                                           gPlayState->state.frames, 32, 32));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    // Container Color
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 200, 200, 200, 255);
    gSPDisplayList(POLY_OPA_DISP++, (Gfx*)gGiPotionContainerPotDL);
    // Liquid Color
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, 200, 200, 200, 255);
    gSPDisplayList(POLY_OPA_DISP++, (Gfx*)gGiPotionContainerLiquidDL);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    // Pattern Color
    // Milk
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 13, 33, 255, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 100, 100, 255, 255);
    gDPLoadTextureBlock(POLY_XLU_DISP++, gGiPotionContainerBluePatternTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 32, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD);
    // Chateau
    // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 250, 225, 78, 255);
    // gDPSetEnvColor(POLY_XLU_DISP++, 184, 42, 119, 255);
    // gDPLoadTextureBlock(POLY_XLU_DISP++, gGiPotionContainerRedPatternTex, G_IM_FMT_IA, G_IM_SIZ_8b, 16, 32, 0,
    // G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, 4, 5, G_TX_NOLOD, G_TX_NOLOD);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gGiPotionContainerPatternDL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawOwlStatue() {
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -3000, 0, MTXMODE_APPLY);
    Gfx_DrawDListOpa(gPlayState, (Gfx*)gOwlStatueOpenedDL);
}

static Gfx gGiSmallKeyCopyDL[75];

void DrawSmallKey(RandoItemId randoItemId) {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    switch (randoItemId) {
        case RI_WOODFALL_SMALL_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 236, 120, 186, 255);
            break;
        case RI_SNOWHEAD_SMALL_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 129, 173, 70, 255);
            break;
        case RI_GREAT_BAY_SMALL_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 99, 90, 183, 255);
            break;
        case RI_STONE_TOWER_SMALL_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 177, 165, 83, 255);
            break;
        default:
            break;
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGiSmallKeyCopyDL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

static Gfx gGiBossKeyCopyDL[87];

void DrawBossKey(RandoItemId randoItemId) {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    switch (randoItemId) {
        case RI_WOODFALL_BOSS_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 236, 120, 186, 255);
            break;
        case RI_SNOWHEAD_BOSS_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 129, 173, 70, 255);
            break;
        case RI_GREAT_BAY_BOSS_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 99, 90, 183, 255);
            break;
        case RI_STONE_TOWER_BOSS_KEY:
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, 255, 255, 255, 255);
            gDPSetEnvColor(POLY_OPA_DISP++, 177, 165, 83, 255);
            break;
        default:
            break;
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gGiBossKeyCopyDL);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gGiBossKeyGemDL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

static Gfx gSkulltulaTokenFlameCopyDL[76];

void DrawSkulltulaToken(RandoItemId randoItemId, Actor* actor) {
    // It is not known why this happens, but the eyes on the skulltula tokens disappear if they are are perfectly
    // parallel with the camera. This most likely a problem in our Fast3D (maybe z-index stuff?).
    // Tilting the token down by 16 units seems to be enough to get it to always render the eyes without being
    // noticeable that it is tilted. This issue was most prevalent for tokens in shops.
    Matrix_RotateXS(16, MTXMODE_APPLY);

    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, (Gfx*)gSkulltulaTokenDL);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);

    if (randoItemId == RI_GS_TOKEN_OCEAN) {
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 0, 255, 255, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 255, 255);
    } else {
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, 0, 255, 170, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 0, 255, 0, 255);
    }

    gSPSegment(POLY_XLU_DISP++, 0x08,
               (uintptr_t)Gfx_TwoTexScroll(gPlayState->state.gfxCtx, G_TX_RENDERTILE, gPlayState->state.frames * 0,
                                           -(gPlayState->state.frames * 5), 32, 32, 1, gPlayState->state.frames * 0,
                                           gPlayState->state.frames * 0, 32, 64));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gSkulltulaTokenFlameCopyDL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawSparkles(RandoItemId randoItemId, Actor* actor) {
    if (actor == NULL) {
        return;
    }

    if (gGameState->frames % 2 == 0) {
        return;
    }

    static Vec3f sVelocity = { 0.0f, 0.0f, 0.0f };
    static Vec3f sAccel = { 0.0f, 0.0f, 0.0f };
    static Color_RGBA8 sPrimColor = { 255, 255, 255, 255 };
    static Color_RGBA8 sEnvColor = { 255, 128, 0, 255 };
    Vec3f newPos;

    newPos.x = Rand_CenteredFloat(10.0f) + actor->world.pos.x;
    newPos.y = (Rand_ZeroOne() * 10.0f) + actor->world.pos.y;
    newPos.z = Rand_CenteredFloat(10.0f) + actor->world.pos.z;

    if (actor->id == ACTOR_EN_SI) {
        newPos.y = (Rand_ZeroOne() * 10.0f) + actor->world.pos.y - 5.0f;
    } else if (actor->id == ACTOR_EN_ITEM00) {
        newPos.x = Rand_CenteredFloat(20.0f) + actor->world.pos.x;
        newPos.y = (Rand_ZeroOne() * 10.0f) + actor->world.pos.y + 10.0f;
        newPos.z = Rand_CenteredFloat(20.0f) + actor->world.pos.z;
    }

    EffectSsKirakira_SpawnDispersed(gPlayState, &newPos, &sVelocity, &sAccel, &sPrimColor, &sEnvColor, 2000, 16);
}

void Rando::DrawItem(RandoItemId randoItemId, Actor* actor) {
    // Apply hilites with actor world pos before drawing
    if (actor != NULL) {
        func_800B8118(actor, gPlayState, 0);
        func_800B8050(actor, gPlayState, 0);
    }

    if (randoItemId >= RI_SOUL_GOHT && randoItemId <= RI_SOUL_WART) {
        DrawSoul(randoItemId);
        return;
    }

    switch (randoItemId) {
        case RI_JUNK:
            Rando::DrawItem(Rando::CurrentJunkItem(), actor);
            break;
        case RI_GREAT_BAY_SMALL_KEY:
        case RI_SNOWHEAD_SMALL_KEY:
        case RI_STONE_TOWER_SMALL_KEY:
        case RI_WOODFALL_SMALL_KEY:
            DrawSmallKey(randoItemId);
            break;
        case RI_GREAT_BAY_BOSS_KEY:
        case RI_SNOWHEAD_BOSS_KEY:
        case RI_STONE_TOWER_BOSS_KEY:
        case RI_WOODFALL_BOSS_KEY:
            DrawBossKey(randoItemId);
            break;
        case RI_SONG_TIME:
        case RI_SONG_STORMS:
        case RI_SONG_SUN:
        case RI_SONG_HEALING:
        case RI_SONG_SOARING:
        case RI_SONG_SONATA:
        case RI_SONG_ELEGY:
        case RI_SONG_LULLABY_INTRO:
        case RI_SONG_LULLABY:
        case RI_SONG_OATH:
        case RI_SONG_EPONA:
        case RI_SONG_NOVA:
            DrawSong(randoItemId);
            break;
        case RI_CLOCK_TOWN_STRAY_FAIRY:
        case RI_WOODFALL_STRAY_FAIRY:
        case RI_SNOWHEAD_STRAY_FAIRY:
        case RI_GREAT_BAY_STRAY_FAIRY:
        case RI_STONE_TOWER_STRAY_FAIRY:
            DrawStrayFairy(randoItemId);
            break;
        case RI_DOUBLE_DEFENSE:
            DrawDoubleDefense();
            break;
        case RI_MILK_REFILL:
            DrawMilkRefill();
            break;
        case RI_GS_TOKEN_SWAMP:
        case RI_GS_TOKEN_OCEAN:
            DrawSkulltulaToken(randoItemId, actor);
            break;
        case RI_OWL_CLOCK_TOWN_SOUTH:
        case RI_OWL_GREAT_BAY_COAST:
        case RI_OWL_IKANA_CANYON:
        case RI_OWL_MILK_ROAD:
        case RI_OWL_MOUNTAIN_VILLAGE:
        case RI_OWL_SNOWHEAD:
        case RI_OWL_SOUTHERN_SWAMP:
        case RI_OWL_STONE_TOWER:
        case RI_OWL_WOODFALL:
        case RI_OWL_ZORA_CAPE:
            DrawOwlStatue();
            break;
        case RI_PROGRESSIVE_LULLABY:
        case RI_PROGRESSIVE_MAGIC:
        case RI_PROGRESSIVE_BOW:
        case RI_PROGRESSIVE_BOMB_BAG:
        case RI_PROGRESSIVE_SWORD:
        case RI_PROGRESSIVE_WALLET:
            Rando::DrawItem(Rando::ConvertItem(randoItemId), actor);
            break;
        case RI_NONE:
        case RI_UNKNOWN:
            break;
        default:
            GetItem_Draw(gPlayState, Rando::StaticData::Items[randoItemId].drawId);
            break;
    }

    switch (randoItemId) {
        case RI_NONE:
        case RI_PROGRESSIVE_MAGIC:
        case RI_SINGLE_MAGIC:
        case RI_DOUBLE_MAGIC:
            DrawSparkles(randoItemId, actor);
            break;
        default:
            break;
    }
}

static RegisterShipInitFunc initializeGICopyDLs(
    []() {
        // Small keys
        Gfx* baseDL = ResourceMgr_LoadGfxByName(gGiSmallKeyDL);
        memcpy(gGiSmallKeyCopyDL, baseDL, sizeof(gGiSmallKeyCopyDL));
        gGiSmallKeyCopyDL[5] = gsDPNoOp();
        gGiSmallKeyCopyDL[6] = gsDPNoOp();

        // Boss keys
        baseDL = ResourceMgr_LoadGfxByName(gGiBossKeyDL);
        memcpy(gGiBossKeyCopyDL, baseDL, sizeof(gGiBossKeyCopyDL));
        gGiBossKeyCopyDL[5] = gsDPNoOp();
        gGiBossKeyCopyDL[6] = gsDPNoOp();

        // Token Flame
        baseDL = ResourceMgr_LoadGfxByName(gSkulltulaTokenFlameDL);
        memcpy(gSkulltulaTokenFlameCopyDL, baseDL, sizeof(gSkulltulaTokenFlameCopyDL));
        gSkulltulaTokenFlameCopyDL[5] = gsDPNoOp();
        gSkulltulaTokenFlameCopyDL[6] = gsDPNoOp();
    },
    {});
