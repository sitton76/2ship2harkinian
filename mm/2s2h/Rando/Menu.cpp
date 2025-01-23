#include "Rando/Rando.h"
#include "Rando/Spoiler/Spoiler.h"
#include <libultraship/libultraship.h>
#include "2s2h/BenGui/UIWidgets.hpp"
#include "Rando/CheckTracker/CheckTracker.h"
#include "BenPort.h"
#include "build.h"

// TODO: This block should come from elsewhere, tied to data in Rando::StaticData::Options
std::vector<std::string> logicOptions = { "Glitchless", "No Logic", "Nearly No Logic", "French Vanilla", "Vanilla" };

namespace BenGui {
extern std::shared_ptr<Rando::CheckTracker::CheckTrackerWindow> mRandoCheckTrackerWindow;
extern std::shared_ptr<Rando::CheckTracker::SettingsWindow> mRandoCheckTrackerSettingsWindow;
} // namespace BenGui

extern "C" {
#include "archives/icon_item_24_static/icon_item_24_static_yar.h"
}

void Rando::DrawMenu() {
    ImGui::BeginChild("randoSettings", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
    ImGui::SeparatorText("Seed Generation");
    UIWidgets::CVarCheckbox("Enable Rando (Randomizes new files upon creation)", "gRando.Enabled");

    if (UIWidgets::CVarCombobox("Seed", "gRando.SpoilerFileIndex", Rando::Spoiler::spoilerOptions)) {
        if (CVarGetInteger("gRando.SpoilerFileIndex", 0) == 0) {
            CVarSetString("gRando.SpoilerFile", "");
        } else {
            CVarSetString("gRando.SpoilerFile",
                          Rando::Spoiler::spoilerOptions[CVarGetInteger("gRando.SpoilerFileIndex", 0)].c_str());
        }
    }

    if (CVarGetInteger("gRando.SpoilerFileIndex", 0) == 0) {
        UIWidgets::PushStyleSlider();
        static char seed[256];
        std::string stringSeed = CVarGetString("gRando.InputSeed", "");
        strcpy(seed, stringSeed.c_str());
        ImGui::InputText("##Seed", seed, sizeof(seed), ImGuiInputTextFlags_CallbackAlways,
                         [](ImGuiInputTextCallbackData* data) {
                             CVarSetString("gRando.InputSeed", data->Buf);
                             return 0;
                         });
        if (stringSeed.length() < 1) {
            ImGui::SameLine(17.0f);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.4f), "Leave blank for random seed");
        }
        UIWidgets::PopStyleSlider();

        UIWidgets::CVarCheckbox("Generate Spoiler File", "gRando.GenerateSpoiler");

        UIWidgets::CVarCombobox(
            "Logic", Rando::StaticData::Options[RO_LOGIC].cvar, logicOptions,
            { .tooltip =
                  "Glitchless - The items are shuffled in a way that guarantees the seed is beatable without "
                  "glitches\n\n"
                  "No Logic - The items are shuffled completely randomly, this can result in unbeatable seeds, and "
                  "will require heavy use of glitches\n\n"
                  "Nearly No Logic - The items are shuffled completely randomly, with the following exceptions:\n"
                  "- Oath to Order and Remains cannot be placed on the Moon\n"
                  "- Deku Mask, Zora Mask, Sonata, and Bossa Nova cannot be placed in their respective Temples or on "
                  "the Moon\n\n"
                  "French Vanilla - This is an alternative variant to Glitchless, but the items are biased to be "
                  "closer to their vanilla locations. Tends to be an more beginner friendly experience.\n\n"
                  "Vanilla - The items are not shuffled." });

        if (CVarGetInteger(Rando::StaticData::Options[RO_LOGIC].cvar, RO_LOGIC_NO_LOGIC) != RO_LOGIC_VANILLA) {
            UIWidgets::CVarCheckbox("Shuffle Gold Skulltula Tokens",
                                    Rando::StaticData::Options[RO_SHUFFLE_GOLD_SKULLTULAS].cvar);
            UIWidgets::CVarCheckbox("Shuffle Owl Statues", Rando::StaticData::Options[RO_SHUFFLE_OWL_STATUES].cvar);
            UIWidgets::CVarCheckbox("Shuffle Mundane Checks", Rando::StaticData::Options[RO_SHUFFLE_MUNDANE].cvar);
            UIWidgets::Tooltip(
                "This will shuffle freestanding rupees and drops from pots, crates, etc. Not everything is covered "
                "here yet, consult the check tracker for more detailed information.");
            UIWidgets::CVarCheckbox("Shuffle Shops", Rando::StaticData::Options[RO_SHUFFLE_SHOPS].cvar);
            UIWidgets::CVarCheckbox("Shuffle Boss Remains", Rando::StaticData::Options[RO_SHUFFLE_BOSS_REMAINS].cvar);

            ImGui::SeparatorText("Untested");
            UIWidgets::CVarCheckbox("Shuffle Enemy Souls", Rando::StaticData::Options[RO_SHUFFLE_ENEMY_SOULS].cvar);
        }
    }
    ImGui::SeparatorText("Enhancements");
    UIWidgets::CVarCheckbox("Container Style Matches Contents", "gRando.CSMC");
    UIWidgets::Tooltip("This will make the contents of a container match the container itself. This currently only "
                       "applies to chests and pots");
    UIWidgets::WindowButton("Check Tracker", "gWindows.CheckTracker", BenGui::mRandoCheckTrackerWindow,
                            { .size = ImVec2((ImGui::GetContentRegionAvail().x - 48.0f), 40.0f) });
    ImGui::SameLine();
    if (UIWidgets::Button(ICON_FA_COG, { .size = ImVec2(40.0f, 40.0f) })) {
        BenGui::mRandoCheckTrackerSettingsWindow->ToggleVisibility();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("randoDisclaimer");
    ImGui::PushStyleColor(ImGuiCol_Text, UIWidgets::Colors::Gray);
    if (gGitCommitTag[0] == 0) {
        ImGui::Text("%s | %s", (char*)gGitBranch, (char*)gGitCommitHash);
    } else {
        ImGui::Text("%s", (char*)gBuildVersion);
    }
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Text, UIWidgets::Colors::Yellow);
    ImGui::SeparatorText("Disclaimer");
    ImGui::PopStyleColor();
    ImGui::TextWrapped(
        "This is an Alpha. Please make note of any odd or unexpected behavior while you are playing. While we are in "
        "the earlier phases of this project, some things you may encounter are:\n- X Check is not shuffled\n- X "
        "Cutscene is not skipped\n- Soft lock when interacting with X\n- Unbeatable seed (glitchless logic)\n\nWe are "
        "aware of some of these, but likely not all. Please compare your findings to our list of known issues, which "
        "is available in the pins of the Rando Alpha Discord thread, or on the Github Issue #211, and let us know if "
        "you encounter any new issues.\n\nExplore the menus for various enhancements and time savers, they are not "
        "enabled by default in Rando.\n\n");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
    ImGui::SeparatorText("Thank You");
    ImGui::PopStyleColor();
    ImGui::TextWrapped("Special thanks to BalloonDude, Eblo, Caladius, Sitton, Dana, our playtesters, everyone who "
                       "contributed to the SoH randomizer, and the creators of the various other randomizer "
                       "implementations that inspired this project. I hope you enjoy it.\n\n");
    ImTextureID swordTextureId = Ship::Context::GetInstance()->GetWindow()->GetGui()->GetTextureByName(
        (const char*)gQuestIconHeartContainer2Tex);
    ImGui::Image(swordTextureId, ImVec2(25.0f, 25.0f));
    ImGui::SameLine();
    ImGui::Text("ProxySaw");
    ImGui::EndChild();
}
