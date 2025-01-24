#ifndef RANDO_DRAW_FUNCS_H
#define RANDO_DRAW_FUNCS_H

extern "C" {
#include <functions.h>
#include "BenPort.h"
// Enemy Includes
#include "src/overlays/actors/ovl_En_Dodongo/z_en_dodongo.h"
#include "src/overlays/actors/ovl_En_Neo_Reeba/z_en_neo_reeba.h"
#include "src/overlays/actors/ovl_En_Slime/z_en_slime.h"
#include "src/overlays/actors/ovl_En_Rat/z_en_rat.h"
#include "assets/objects/object_bat/object_bat.h"
#include "src/overlays/actors/ovl_En_Wf/z_en_wf.h"
#include "assets/objects/object_ph/object_ph.h"
#include "src/overlays/actors/ovl_En_Jso2/z_en_jso2.h"
#include "src/overlays/actors/ovl_En_Wallmas/z_en_wallmas.h"
#include "src/overlays/actors/ovl_En_Firefly/z_en_firefly.h"
#include "assets/objects/object_tite/object_tite.h"

#include "src/overlays/actors/ovl_En_Bom/z_en_bom.h"

#include "objects/gameplay_keep/gameplay_keep.h"


#include "src/overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"
#include "assets/objects/object_gi_reserve00/object_gi_reserve00.h"

void ResourceMgr_PatchGfxByName(const char* path, const char* patchName, int index, Gfx instruction);
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
    //ResourceMgr_PatchGfxByName(gGiMoonsTearGlowDL, "MoonGlowPrim", 5, gsDPSetPrimColor(0, 0x80, 255, 0, 0, 120));
    //ResourceMgr_PatchGfxByName(gGiMoonsTearGlowDL, "MoonGlowEnv", 6, gsDPSetEnvColor(0, 255, 0, 0, 100));

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

// Limb Override Functions

void DrawEnFirefly_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* firefly) {
    static Color_RGBA8 auraPrimColor[2] = { { 255, 255, 100, 255 }, { 100, 200, 255, 255 } };
    static Color_RGBA8 auraEnvColor[2] = { { 255, 50, 0, 0 }, { 0, 0, 255, 0 } };
    static uint32_t dustUpdate = 0;
    static bool auraColor = false;
    static Vec3f auraVelocity = { 0, 0.5f, 0 };
    static Vec3f auraAccel = { 0, 0.5f, 0 };
    static Vec3f auraPos;
    if (firefly != NULL) {
        auraPos = firefly->world.pos;
    }

    Matrix_MultZero(&auraPos);
    auraPos.x += Rand_ZeroFloat(0.5f);
    auraPos.y += Rand_ZeroFloat(0.5f);
    auraPos.z += Rand_ZeroFloat(0.5f);

    if (gPlayState != NULL && dustUpdate != gPlayState->state.frames) {
        if (dustUpdate == gPlayState->state.frames - 20) {
            dustUpdate = gPlayState->state.frames;
            auraColor = !auraColor;
        }
    }

    if (limbIndex == FIRE_KEESE_LIMB_HEAD) {
        Gfx* gfx = play->state.gfxCtx->polyXlu.p;
        Scene_SetRenderModeXlu(play, 1, 2);
        gSPMatrix(gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gfx++, (Gfx*)&gKeeseRedEyesDL);
    }
    if (limbIndex == FIRE_KEESE_LIMB_LEFT_WING_END || limbIndex == FIRE_KEESE_LIMB_RIGHT_WING_END_ROOT) {
        EffectSsDust_Spawn(gPlayState, 2, &auraPos, &auraVelocity, &auraAccel, &auraPrimColor[auraColor], &auraEnvColor[auraColor], 
        100, -40, 3, 0);
    }
}

void DrawEnRealBombchu_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* rat) {
    //Gfx* gfx = play->state.gfxCtx->polyOpa.p;

    if (limbIndex == REAL_BOMBCHU_LIMB_TAIL_END) {
        OPEN_DISPS(play->state.gfxCtx);
        Matrix_ReplaceRotation(&play->billboardMtxF);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, (Gfx*)&gBombCapDL);
        Matrix_RotateZYX(0x4000, 0, 0, MTXMODE_APPLY);
        gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 80, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 10, 0, 40, 255);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_OPA_DISP++, (Gfx*)&gBombBodyDL);
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

// Enemy Soul Draw Functions

void DrawBat() {
    static u32 lastUpdate = 0;
    static u32 wingAnim = 0;
    
    OPEN_DISPS(gPlayState->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);

    static Gfx* sWingsDLs[] = {
    (Gfx*)&gBadBatWingsFrame0DL, (Gfx*)&gBadBatWingsFrame1DL, (Gfx*)&gBadBatWingsFrame2DL, (Gfx*)&gBadBatWingsFrame3DL, 
    (Gfx*)&gBadBatWingsFrame4DL, (Gfx*)&gBadBatWingsFrame5DL, (Gfx*)&gBadBatWingsFrame6DL, (Gfx*)&gBadBatWingsFrame7DL, 
    (Gfx*)&gBadBatWingsFrame8DL,
    };

    Gfx* gfx = POLY_OPA_DISP;

    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        if (wingAnim == 8) {
            wingAnim = 0;
        } else {
            wingAnim++;
        }
    }

    gSPDisplayList(&gfx[0], gSetupDLs[SETUPDL_25]);
    gSPMatrix(&gfx[1], Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(&gfx[2], (Gfx*)&gBadBatSetupDL);
    gSPDisplayList(&gfx[3], (Gfx*)&gBadBatBodyDL);
    gSPDisplayList(&gfx[4], sWingsDLs[wingAnim]);

    POLY_OPA_DISP = &gfx[5];

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(2.0f, 0.5f, 2.0f, -3000.0f);
}

void DrawRealBombchu() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[REAL_BOMBCHU_LIMB_MAX];
    static Vec3s morphTable[REAL_BOMBCHU_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Gfx_SetupDL60_XluNoCD(gPlayState->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);
    Matrix_Translate(0, -1500.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gRealBombchuSkel, 
            (AnimationHeader*)&gRealBombchuRunAnim, jointTable, morphTable, REAL_BOMBCHU_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }

    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount,
                          NULL, DrawEnRealBombchu_PostLimbDraw, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(2.0f, 0.5f, 2.0f, -200.0f);
}

void DrawDodongo() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[OBJECT_DODONGO_LIMB_MAX];
    static Vec3s morphTable[OBJECT_DODONGO_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);
    Matrix_Translate(0, -1500.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_Init(gPlayState, &skelAnime, (SkeletonHeader*)&object_dodongo_Skel_008318, 
            (AnimationHeader*)&object_dodongo_Anim_004C20, jointTable, morphTable, OBJECT_DODONGO_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }

    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(4.0f, 0.5f, 4.0f, -200.0f);
}

void DrawGaroMaster() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[GARO_MASTER_LIMB_MAX];
    static Vec3s morphTable[GARO_MASTER_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);
    Matrix_Translate(0, -50.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gGaroMasterSkel, (AnimationHeader*)&gGaroLookAroundAnim, 
            jointTable, morphTable, GARO_MASTER_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    Scene_SetRenderModeXlu(gPlayState, 0, 1);
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount,
                              NULL, NULL, NULL);
    

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(1.0f, 0.3f, 1.0f, -3200.0f);
}

void DrawKeese(Actor* actor) {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[FIRE_KEESE_LIMB_MAX];
    static Vec3s morphTable[FIRE_KEESE_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -700.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_Init(gPlayState, &skelAnime, (SkeletonHeader*)&gFireKeeseSkel, (AnimationHeader*)&gFireKeeseFlyAnim, 
            jointTable, morphTable, FIRE_KEESE_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
   
    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, DrawEnFirefly_PostLimbDraw, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(3.0f, 0.5f, 3.0f, -5200.0f);
}

void DrawLeever() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[LEEVER_LIMB_MAX];
    static Vec3s morphTable[LEEVER_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.05f, 0.05f, 0.05f, MTXMODE_APPLY);
    Matrix_Translate(0, -700.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_Init(gPlayState, &skelAnime, (SkeletonHeader*)&gLeeverSkel, (AnimationHeader*)&gLeeverSpinAnim, 
            jointTable, morphTable, LEEVER_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }

    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x01, 255, 255, 255, 255);
    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(1.0f, 0.3f, 1.0f, -200.0f);
}

void DrawPeehat() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[24];
    static Vec3s morphTable[24];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_Init(gPlayState, &skelAnime, (SkeletonHeader*)&object_ph_Skel_001C80, (AnimationHeader*)&object_ph_Anim_0009C4, 
            jointTable, morphTable, 24);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }

    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(3.0f, 1.0f, 3.0f, -2800.0f);
}

void DrawSlime() {
    static int16_t timer = 25;
    f32 timerFactor = sqrtf(timer) * 0.2f;
    AnimatedMaterial* sSlimeTexAnim = (AnimatedMaterial*)Lib_SegmentedToVirtual((void*)gChuchuSlimeFlowTexAnim);

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Matrix_Scale(0.01f, (((Math_CosF(timer * (2.0f * M_PI / 5.0f)) * (0.07f * timerFactor)) + 1.0f) * 0.01f), 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -2700.0f, 0, MTXMODE_APPLY);

    Gfx_SetupDL25_Xlu(gPlayState->state.gfxCtx);
    AnimatedMat_Draw(gPlayState, sSlimeTexAnim);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 100, 255, 255, 200, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, 255, 180, 0, 255);

    if (timer == 0) {
        timer = 25;
    }

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gPlayState->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Scene_SetRenderModeXlu(gPlayState, 1, 2);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gChuchuBodyDL);
    gSPSegment(POLY_XLU_DISP++, 9, (uintptr_t)gChuchuEyeOpenTex);
    gSPDisplayList(POLY_XLU_DISP++, (Gfx*)gChuchuEyesDL);
    
    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(5.0f, 1.0f, 5.0f, -200.0f);
    timer--;
}

void DrawTektite() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[25];
    static Vec3s morphTable[25];
    static u32 lastUpdate = 0;

    static TexturePtr D_80896B24[2][3] = {
    { (TexturePtr*)&object_tite_Tex_001300, (TexturePtr*)&object_tite_Tex_001700, (TexturePtr*)&object_tite_Tex_001900 },
    { (TexturePtr*)&object_tite_Tex_001B00, (TexturePtr*)&object_tite_Tex_001F00, (TexturePtr*)&object_tite_Tex_002100 },
    };

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -2900.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_Init(gPlayState, &skelAnime, (SkeletonHeader*)&object_tite_Skel_003A20, 
            (AnimationHeader*)&object_tite_Anim_0012E4, jointTable, morphTable, 25);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    Gfx* gfx = POLY_OPA_DISP;

    gSPDisplayList(&gfx[0], gSetupDLs[SETUPDL_25]);

    gSPSegment(&gfx[1], 0x08, (uintptr_t)D_80896B24[0][0]);
    gSPSegment(&gfx[2], 0x09, (uintptr_t)D_80896B24[0][1]);
    gSPSegment(&gfx[3], 0x0A, (uintptr_t)D_80896B24[0][2]);

    POLY_OPA_DISP = &gfx[4];

    SkelAnime_DrawOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(4.0f, 0.5f, 4.0f, -200.0f);
}

void DrawWallmaster() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[WALLMASTER_LIMB_MAX];
    static Vec3s morphTable[WALLMASTER_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);
        
    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -3500.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gWallmasterSkel, (AnimationHeader*)&gWallmasterIdleAnim, 
            jointTable, morphTable, WALLMASTER_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount,
                          NULL, NULL, NULL);
    

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(7.0f, 1.0f, 7.0f, -200.0f);
}

void DrawWolfos() {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static Vec3s jointTable[WOLFOS_NORMAL_LIMB_MAX];
    static Vec3s morphTable[WOLFOS_NORMAL_LIMB_MAX];
    static u32 lastUpdate = 0;

    OPEN_DISPS(gPlayState->state.gfxCtx);
    Gfx_SetupDL25_Opa(gPlayState->state.gfxCtx);

    Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
    Matrix_Translate(0, -3000.0f, 0, MTXMODE_APPLY);

    if (!initialized) {
        initialized = true;
        SkelAnime_InitFlex(gPlayState, &skelAnime, (FlexSkeletonHeader*)&gWolfosNormalSkel, (AnimationHeader*)&gWolfosWaitAnim, 
            jointTable, morphTable, WOLFOS_NORMAL_LIMB_MAX);
    }
    if (gPlayState != NULL && lastUpdate != gPlayState->state.frames) {
        lastUpdate = gPlayState->state.frames;
        SkelAnime_Update(&skelAnime);
    }
    
    gSPSegment(POLY_OPA_DISP++, 0x08, (uintptr_t)&gWolfosNormalEyeOpenTex);
    SkelAnime_DrawFlexOpa(gPlayState, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount,
                              NULL, NULL, NULL);

    CLOSE_DISPS(gPlayState->state.gfxCtx);
    DrawFireRing(5.0f, 1.0f, 5.0f, -200.0f);
}

#endif
