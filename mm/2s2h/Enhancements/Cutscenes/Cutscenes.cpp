#include "Cutscenes.h"

void RegisterCutscenes() {
    // MiscInteractions
    RegisterSkipDekuSalesman();
    RegisterSkipFallingMoonsTear();
    RegisterSkipGreatFairyCutscene();
    RegisterSkipKafeiReveal();
    RegisterSkipScarecrowDance();
    RegisterSkipTatlInterrupts();
    RegisterSkipMayorsOfficeCutscene();

    // StoryCutscenes
    RegisterSkipClockTowerOpen();
    SkipClockTowerSkullKidEncounter();
    RegisterSkipLearningSongOfHealing();
    RegisterSkipLearningSongOfTime();
    RegisterSkipSongOfTimeCutscene();
    RegisterSkipMilkRunCutscenes();

    RegisterHideTitleCards();
    RegisterSkipEntranceCutscenes();
    RegisterSkipIntroSequence();
    RegisterSkipToFileSelect();
}
