#include "DrawFuncs.h"
#include <libultraship/libultraship.h>
#include "2s2h/Enhancements/FrameInterpolation/FrameInterpolation.h"
#include "BenPort.h"

extern "C" {
#include <functions.h>
#include "objects/gameplay_keep/gameplay_keep.h"

// clang-format off
// Boss Includes
/* Goht */      #include "objects/object_boss_hakugin/object_boss_hakugin.h"
/* Gyorg */     #include "objects/object_boss03/object_boss03.h"
/* Odolwa */    #include "objects/object_boss01/object_boss01.h"
/* Twinmold */  #include "objects/object_boss02/object_boss02.h"
// clang-format on

// Soul Effects
#include "src/overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"
#include "assets/objects/object_gi_reserve00/object_gi_reserve00.h"

    void
    ResourceMgr_PatchGfxByName(const char* path, const char* patchName, int index, Gfx instruction);
}

void DrawSmoke(f32 x, f32 y, f32 z, f32 tY) {
    AnimatedMaterial* sMoonTexAnim = (AnimatedMaterial*)Lib_SegmentedToVirtual((void*)gGiMoonsTearTexAnim);
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    AnimatedMat_Draw(gPlayState, sMoonTexAnim);
    Matrix_Scale(x, y, z, MTXMODE_APPLY);
    Matrix_Translate(0, tY, 0, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&gPlayState->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)&gGiMoonsTearGlowDL);
    // ResourceMgr_PatchGfxByName(gGiMoonsTearGlowDL, "MoonGlowPrim", 5, gsDPSetPrimColor(0, 0x80, 255, 0, 0, 120));
    // ResourceMgr_PatchGfxByName(gGiMoonsTearGlowDL, "MoonGlowEnv", 6, gsDPSetEnvColor(0, 255, 0, 0, 100));

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

void DrawFireRing(f32 x, f32 y, f32 z, f32 tY) {
    static u32 lastUpdate = 0;
    static uint32_t unk_1A4 = Rand_ZeroOne() * 128.0f;

    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    Matrix_Scale(x, y, z, MTXMODE_APPLY);
    Matrix_Translate(0, tY, 0, MTXMODE_APPLY);

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 220, 0, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
    gSPSegment(POLY_XLU_DISP++, 0x08,
               (uintptr_t)Gfx_TwoTexScroll(gPlayState->state.gfxCtx, 0, unk_1A4 & 0x7F, 0, 0x20, 0x40, 1, 0,
                                           (unk_1A4 * -15) & 0xFF, 0x20, 0x40));
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gameplay_keep_DL_02E510);

    CLOSE_DISPS(gPlayState->state.gfxCtx);

    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        unk_1A4++;
    }
}

// Boss Souls
extern void DrawGoht() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    Matrix_Translate(0.0f, -20.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(0.005f, 0.005f, 0.005f, MTXMODE_APPLY);

    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[33];
    static Vec3s otherTable[33];
    static u32 lastUpdate = 0;
    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gGohtSkel, (AnimationHeader*)&gGohtRunAnim,
                           jointTable, otherTable, 33);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

extern void DrawGyorg() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    Matrix_Translate(0.0f, -20.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(0.05f, 0.05f, 0.05f, MTXMODE_APPLY);

    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[15];
    static Vec3s otherTable[15];
    static u32 lastUpdate = 0;
    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gGyorgSkel,
                           (AnimationHeader*)&gGyorgCrawlingAnim, jointTable, otherTable, 15);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

extern void DrawOdolwa() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    Matrix_Translate(0.0f, -20.0f, 0.0f, MTXMODE_APPLY);
    Matrix_Scale(0.005f, 0.005f, 0.005f, MTXMODE_APPLY);

    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[52];
    static Vec3s otherTable[52];
    static u32 lastUpdate = 0;
    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gOdolwaSkel,
                           (AnimationHeader*)&gOdolwaReadyAnim, jointTable, otherTable, 52);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}

extern void DrawTwinmold() {
    OPEN_DISPS(gPlayState->state.gfxCtx);

    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.05f, 0.05f, 0.05f, MTXMODE_APPLY);

    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[13];
    static Vec3s otherTable[13];
    static u32 lastUpdate = 0;
    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gTwinmoldHeadSkel,
                           (AnimationHeader*)&gTwinmoldHeadFlyAnim, jointTable, otherTable, 13);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }

    gSPSegment(POLY_OPA_DISP++, 0x08, (uintptr_t)gTwinmoldBlueSkinTex);
    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
}