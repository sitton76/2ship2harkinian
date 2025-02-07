#include "BenMenu.h"
#include "UIWidgets.hpp"
#include "BenPort.h"
#include "BenInputEditorWindow.h"
#include "window/gui/GuiMenuBar.h"
#include "window/gui/GuiElement.h"
#include "DeveloperTools/SaveEditor.h"
#include "DeveloperTools/ActorViewer.h"
#include "DeveloperTools/CollisionViewer.h"
#include "DeveloperTools/EventLog.h"
#include "2s2h/Enhancements/GfxPatcher/AuthenticGfxPatches.h"
#include "HudEditor.h"
#include "Notification.h"
#include <variant>
#include "StringHelper.h"
#include <spdlog/fmt/fmt.h>
#include "variables.h"
#include <variant>
#include <tuple>
#include "ResolutionEditor.h"
#include "2s2h/Rando/Rando.h"

extern "C" {
#include "z64.h"
#include "functions.h"
extern PlayState* gPlayState;
}
extern std::unordered_map<s16, const char*> warpPointSceneList;
extern void Warp();

namespace BenGui {
extern std::shared_ptr<BenMenu> mBenMenu;
void FreeLookPitchMinMax() {
    f32 maxY = CVarGetFloat("gEnhancements.Camera.FreeLook.MaxPitch", 72.0f);
    f32 minY = CVarGetFloat("gEnhancements.Camera.FreeLook.MinPitch", -49.0f);
    CVarSetFloat("gEnhancements.Camera.FreeLook.MaxPitch", std::max(maxY, minY));
    CVarSetFloat("gEnhancements.Camera.FreeLook.MinPitch", std::min(maxY, minY));
}

using namespace UIWidgets;

void BenMenu::AddSidebarEntry(std::string sectionName, std::string sidebarName, uint32_t columnCount) {
    if (sectionName == "Settings") {
        settingsSidebar.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
        settingsOrder.push_back(sidebarName);
    } else if (sectionName == "Enhancements") {
        enhancementsSidebar.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
        enhancementsOrder.push_back(sidebarName);
    } else if (sectionName == "Dev Tools") {
        devToolsSidebar.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
        devToolsOrder.push_back(sidebarName);
    } else if (sectionName == "Rando") {
        randoSidebar.emplace(sidebarName, SidebarEntry{ .columnCount = columnCount });
        randoOrder.push_back(sidebarName);
    }
}

WidgetInfo& BenMenu::AddWidget(WidgetPath& pathInfo, std::string widgetName, WidgetType widgetType) {
    assert(widgetName != ""); // Must be unique
    std::unordered_map<std::string, SidebarEntry>* sidebar;
    if (pathInfo.sectionName == "Settings") {
        sidebar = &settingsSidebar;
    } else if (pathInfo.sectionName == "Enhancements") {
        sidebar = &enhancementsSidebar;
    } else if (pathInfo.sectionName == "Dev Tools") {
        sidebar = &devToolsSidebar;
    } else if (pathInfo.sectionName == "Rando") {
        sidebar = &randoSidebar;
    }
    uint8_t column = pathInfo.column - 1;
    if (sidebar->contains(pathInfo.sidebarName)) {
        while (sidebar->at(pathInfo.sidebarName).columnWidgets.size() < column + 1) {
            sidebar->at(pathInfo.sidebarName).columnWidgets.push_back({});
        }
    }
    SidebarEntry& entry = sidebar->at(pathInfo.sidebarName);
    entry.columnWidgets.at(column).push_back({ .name = widgetName, .type = widgetType });
    WidgetInfo& widget = entry.columnWidgets.at(column).back();
    switch (widgetType) {
        case WIDGET_CHECKBOX:
        case WIDGET_CVAR_CHECKBOX:
            widget.options = std::make_shared<CheckboxOptions>();
            break;
        case WIDGET_SLIDER_FLOAT:
        case WIDGET_CVAR_SLIDER_FLOAT:
            widget.options = std::make_shared<FloatSliderOptions>();
            break;
        case WIDGET_SLIDER_INT:
        case WIDGET_CVAR_SLIDER_INT:
            widget.options = std::make_shared<IntSliderOptions>();
            break;
        case WIDGET_COMBOBOX:
        case WIDGET_CVAR_COMBOBOX:
        case WIDGET_AUDIO_BACKEND:
        case WIDGET_VIDEO_BACKEND:
            widget.options = std::make_shared<ComboboxOptions>();
            break;
        case WIDGET_BUTTON:
            widget.options = std::make_shared<ButtonOptions>();
            break;
        case WIDGET_WINDOW_BUTTON:
            widget.options = std::make_shared<ButtonOptions>(ButtonOptions{ .size = Sizes::Inline });
            break;
        case WIDGET_COLOR_24:
        case WIDGET_COLOR_32:
            break;
        case WIDGET_SEARCH:
        case WIDGET_SEPARATOR:
        case WIDGET_SEPARATOR_TEXT:
        case WIDGET_TEXT:
        default:
            widget.options = std::make_shared<WidgetOptions>();
    }
    return widget;
}

void BenMenu::AddSettings() {

    // General Settings
    AddSidebarEntry("Settings", "General", 3);
    WidgetPath path = { "Settings", "General", 1 };
    AddWidget(path, "Menu Theme", WIDGET_CVAR_COMBOBOX)
        .CVar("gSettings.Menu.Theme")
        .Options(ComboboxOptions()
                     .Tooltip("Changes the Theme of the Menu Widgets.")
                     .ComboMap(menuThemeOptions)
                     .DefaultIndex(Colors::LightBlue));
#if not defined(__SWITCH__) and not defined(__WIIU__)
    AddWidget(path, "Menu Controller Navigation", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_IMGUI_CONTROLLER_NAV)
        .Options(CheckboxOptions().Tooltip(
            "Allows controller navigation of the 2Ship menu (Settings, Enhancements,...)\nCAUTION: "
            "This will disable game inputs while the menu is visible.\n\nD-pad to move between "
            "items, A to select, B to move up in scope."));
    AddWidget(path, "Cursor Always Visible", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.CursorVisibility")
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetForceCursorVisibility(
                CVarGetInteger("gSettings.CursorVisibility", 0));
        })
        .Options(CheckboxOptions().Tooltip("Makes the cursor always visible, even in full screen."));
#endif
    AddWidget(path, "Search In Sidebar", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SidebarSearch")
        .Callback([](WidgetInfo& info) {
            if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
                mBenMenu->InsertSidebarSearch();
            } else {
                mBenMenu->RemoveSidebarSearch();
            }
        })
        .Options(CheckboxOptions().Tooltip(
            "Displays the Search menu as a sidebar entry in Settings instead of in the header."));
    AddWidget(path, "Search Input Autofocus", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.SearchAutofocus")
        .Options(CheckboxOptions().Tooltip(
            "Search input box gets autofocus when visible. Does not affect using other widgets."));
    AddWidget(path, "Alt Assets Tab hotkey", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Mods.AlternateAssetsHotkey")
        .Options(
            CheckboxOptions().Tooltip("Allows pressing the Tab key to toggle alternate assets").DefaultValue(true));
    AddWidget(path, "Open App Files Folder", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            std::string filesPath = Ship::Context::GetInstance()->GetAppDirectoryPath();
            SDL_OpenURL(std::string("file:///" + std::filesystem::absolute(filesPath).string()).c_str());
        })
        .Options(ButtonOptions().Tooltip("Opens the folder that contains the save and mods folders, etc."));

    // Audio Settings
    path.sidebarName = "Audio";
    AddSidebarEntry("Settings", "Audio", 3);
    AddWidget(path, "Master Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.MasterVolume")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the overall sound volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Main Music Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.MainMusicVolume")
        .Callback([](WidgetInfo& info) {
            AudioSeq_SetPortVolumeScale(SEQ_PLAYER_BGM_MAIN, CVarGetFloat("gSettings.Audio.MainMusicVolume", 1.0f));
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the background music volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Sub Music Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.SubMusicVolume")
        .Callback([](WidgetInfo& info) {
            AudioSeq_SetPortVolumeScale(SEQ_PLAYER_BGM_SUB, CVarGetFloat("gSettings.Audio.SubMusicVolume", 1.0f));
        })
        .Options(
            FloatSliderOptions().Tooltip("Adjust the sub music volume.").ShowButtons(false).Format("").IsPercentage());
    AddWidget(path, "Sound Effects Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.SoundEffectsVolume")
        .Callback([](WidgetInfo& info) {
            AudioSeq_SetPortVolumeScale(SEQ_PLAYER_SFX, CVarGetFloat("gSettings.Audio.SoundEffectsVolume", 1.0f));
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the sound effects volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Fanfare Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.FanfareVolume")
        .Callback([](WidgetInfo& info) {
            AudioSeq_SetPortVolumeScale(SEQ_PLAYER_FANFARE, CVarGetFloat("gSettings.Audio.FanfareVolume", 1.0f));
        })
        .Options(
            FloatSliderOptions().Tooltip("Adjust the fanfare volume.").ShowButtons(false).Format("").IsPercentage());
    AddWidget(path, "Ambience Volume: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gSettings.Audio.AmbienceVolume")
        .Callback([](WidgetInfo& info) {
            AudioSeq_SetPortVolumeScale(SEQ_PLAYER_AMBIENCE, CVarGetFloat("gSettings.Audio.AmbienceVolume", 1.0f));
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the ambient sound volume.")
                     .ShowButtons(false)
                     .Format("")
                     .IsPercentage());
    AddWidget(path, "Audio API", WIDGET_AUDIO_BACKEND);

    // Graphics Settings
    static int32_t maxFps;
    const char* tooltip = "";
    if (Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() == Ship::WindowBackend::FAST3D_DXGI_DX11) {
        maxFps = 360;
        tooltip = "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. This is "
                  "purely visual and does not impact game logic, execution of glitches etc.\n\nA higher target "
                  "FPS than your monitor's refresh rate will waste resources, and might give a worse result.";
    } else {
        maxFps = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
        tooltip = "Uses Matrix Interpolation to create extra frames, resulting in smoother graphics. This is "
                  "purely visual and does not impact game logic, execution of glitches etc.";
    }
    path.sidebarName = "Graphics";
    AddSidebarEntry("Settings", "Graphics", 3);
    AddWidget(path, "Toggle Fullscreen", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Fullscreen")
        .Callback([](WidgetInfo& info) { Ship::Context::GetInstance()->GetWindow()->ToggleFullscreen(); })
        .Options(CheckboxOptions().Tooltip("Toggles Fullscreen On/Off."));
#ifndef __APPLE__
    AddWidget(path, "Internal Resolution: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar(CVAR_INTERNAL_RESOLUTION)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetResolutionMultiplier(
                CVarGetFloat(CVAR_INTERNAL_RESOLUTION, 1));
        })
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_ADVANCED_RESOLUTION_ON).active &&
                mBenMenu->disabledMap.at(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_ADVANCED_RESOLUTION_ON);
                info.activeDisables.push_back(DISABLE_FOR_VERTICAL_RES_TOGGLE_ON);
            } else if (mBenMenu->disabledMap.at(DISABLE_FOR_LOW_RES_MODE_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_LOW_RES_MODE_ON);
            }
        })
        .Options(
            FloatSliderOptions()
                .Tooltip("Multiplies your output resolution by the value inputted, as a more intensive but effective "
                         "form of anti-aliasing.")
                .ShowButtons(false)
                .IsPercentage()
                .Format("")
                .Min(0.5f)
                .Max(2.0f));
#endif
#ifndef __WIIU__
    AddWidget(path, "Anti-aliasing (MSAA): %d", WIDGET_CVAR_SLIDER_INT)
        .CVar(CVAR_MSAA_VALUE)
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetWindow()->SetMsaaLevel(CVarGetInteger(CVAR_MSAA_VALUE, 1));
        })
        .Options(
            IntSliderOptions()
                .Tooltip("Activates MSAA (multi-sample anti-aliasing) from 2x up to 8x, to smooth the edges of "
                         "rendered geometry.\n"
                         "Higher sample count will result in smoother edges on models, but may reduce performance.")
                .Min(1)
                .Max(8)
                .DefaultValue(1));
#endif

    AddWidget(path, "Current FPS: %d", WIDGET_CVAR_SLIDER_INT)
        .CVar("gInterpolationFPS")
        .Callback([](WidgetInfo& info) {
            int32_t defaultValue = std::static_pointer_cast<IntSliderOptions>(info.options)->defaultValue;
            if (CVarGetInteger(info.cVar, defaultValue) == defaultValue) {
                info.name = "Current FPS: Original (%d)";
            } else {
                info.name = "Current FPS: %d";
            }
        })
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_MATCH_REFRESH_RATE_ON).active)
                info.activeDisables.push_back(DISABLE_FOR_MATCH_REFRESH_RATE_ON);
        })
        .Options(IntSliderOptions().Tooltip(tooltip).Min(20).Max(maxFps).DefaultValue(20));
    AddWidget(path, "Match Refresh Rate", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            int hz = Ship::Context::GetInstance()->GetWindow()->GetCurrentRefreshRate();
            if (hz >= 20 && hz <= 360) {
                CVarSetInteger("gInterpolationFPS", hz);
                Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
            }
        })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NOT_DIRECTX).active; })
        .Options(ButtonOptions().Tooltip("Matches interpolation value to the current game's window refresh rate."));
    AddWidget(path, "Match Refresh Rate", WIDGET_CVAR_CHECKBOX)
        .CVar("gMatchRefreshRate")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DIRECTX).active; })
        .Options(CheckboxOptions().Tooltip("Matches interpolation value to the current game's window refresh rate."));
    AddWidget(path, "Jitter fix : >= % d FPS", WIDGET_CVAR_SLIDER_INT)
        .CVar("gExtraLatencyThreshold")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NOT_DIRECTX).active; })
        .Options(IntSliderOptions()
                     .Tooltip("When Interpolation FPS setting is at least this threshold, add one frame of input "
                              "lag (e.g. 16.6 ms for 60 FPS) in order to avoid jitter. This setting allows the "
                              "CPU to work on one frame while GPU works on the previous frame.\nThis setting "
                              "should be used when your computer is too slow to do CPU + GPU work in time.")
                     .Min(0)
                     .Max(360)
                     .DefaultValue(80));
    AddWidget(path, "Renderer API (Needs reload)", WIDGET_VIDEO_BACKEND);
    AddWidget(path, "Enable Vsync", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_VSYNC_ENABLED)
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NO_VSYNC).active; })
        .Options(CheckboxOptions().Tooltip("Enables Vsync."));
    AddWidget(path, "Windowed Fullscreen", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_SDL_WINDOWED_FULLSCREEN)
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NO_WINDOWED_FULLSCREEN).active;
        })
        .Options(CheckboxOptions().Tooltip("Enables Windowed Fullscreen Mode."));
    AddWidget(path, "Allow multi-windows", WIDGET_CVAR_CHECKBOX)
        .CVar(CVAR_ENABLE_MULTI_VIEWPORTS)
        .PreFunc(
            [](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NO_MULTI_VIEWPORT).active; })
        .Options(CheckboxOptions().Tooltip(
            "Allows multiple windows to be opened at once. Requires a reload to take effect."));
    AddWidget(path, "Texture Filter (Needs reload)", WIDGET_CVAR_COMBOBOX)
        .CVar(CVAR_TEXTURE_FILTER)
        .Options(ComboboxOptions().Tooltip("Sets the applied Texture Filtering.").ComboMap(textureFilteringMap));

    path.sidebarName = "Controls";
    AddSidebarEntry("Settings", "Controls", 1);
    AddWidget(path, "Popout Bindings Window", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.BenInputEditor")
        .WindowName("2S2H Input Editor")
        .Options(ButtonOptions().Tooltip("Enables the separate Bindings Window.").Size(Sizes::Inline));

    path.sidebarName = "Overlay";
    AddSidebarEntry("Settings", "Overlay", 2);
    AddWidget(path, "Notifications", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Position", WIDGET_CVAR_COMBOBOX)
        .CVar("gNotifications.Position")
        .Options(ComboboxOptions()
                     .Tooltip("Which corner of the screen notifications appear in.")
                     .ComboMap(notificationPosition)
                     .DefaultIndex(3));
    AddWidget(path, "Duration: %.0f seconds", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.Duration")
        .Options(FloatSliderOptions()
                     .Tooltip("How long notifications are displayed for.")
                     .Format("%.1f")
                     .Step(0.1f)
                     .Min(3.0f)
                     .Max(30.0f)
                     .DefaultValue(10.0f));
    AddWidget(path, "Background Opacity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.BgOpacity")
        .Options(FloatSliderOptions()
                     .Tooltip("How opaque the background of notifications is.")
                     .DefaultValue(0.5f)
                     .IsPercentage());
    AddWidget(path, "Size %.1f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gNotifications.Size")
        .Options(FloatSliderOptions()
                     .Tooltip("How large notifications are.")
                     .Format("%.1f")
                     .Step(0.1f)
                     .Min(1.0f)
                     .Max(5.0f)
                     .DefaultValue(1.8f));
    AddWidget(path, "Test Notification", WIDGET_BUTTON)
        .Callback([](WidgetInfo& info) {
            Notification::Emit({
                .itemIcon = "__OTR__icon_item_24_static_yar/gQuestIconGoldSkulltulaTex",
                .prefix = "This",
                .message = "is a",
                .suffix = "test.",
            });
        })
        .Options(ButtonOptions().Tooltip("Displays a test notification."));
    path.column = 2;
    AddWidget(path, "In-Game Timer", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Toggle Display Overlay", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.DisplayOverlay")
        .WindowName("Display Overlay")
        .Options(ButtonOptions().Tooltip("Toggles the Display Overlay window for In-game Timers."));
    AddWidget(path, "Hide Window Background", WIDGET_CVAR_CHECKBOX)
        .CVar("gDisplayOverlay.Background")
        .Options(CheckboxOptions().Tooltip("Hides the background of the Display Overlay window."));
    AddWidget(path, "Scale: %.0fx", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gDisplayOverlay.Scale")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the Scale for the Display Overlay window.")
                     .Min(1.0f)
                     .Max(5.0f)
                     .DefaultValue(1.0f)
                     .Format("%.1f")
                     .Step(0.1f));
}
int32_t motionBlurStrength;

void BenMenu::AddEnhancements() {
    WidgetPath path = { "Enhancements", "Camera", 1 };
    AddSidebarEntry("Enhancements", "Camera", 3);
    // Camera Snap Fix
    AddWidget(path, "Fixes", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Fix Targeting Camera Snap", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FixTargettingCameraSnap")
        .Options(CheckboxOptions().Tooltip(
            "Fixes the camera snap that occurs when you are moving and press the targeting button."));
    AddWidget(path, "First Person", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Disable Auto-Centering", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.DisableFirstPersonAutoCenterView")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_GYRO_ON).active)
                info.activeDisables.push_back(DISABLE_FOR_GYRO_ON);
        })
        .Options(CheckboxOptions().Tooltip("Disables the auto-centering of the camera in first person mode."));
    AddWidget(path, "Invert X Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.InvertX")
        .Options(CheckboxOptions().Tooltip("Inverts the X Axis of the Camera in First Person Mode."));
    AddWidget(path, "Invert Y Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.InvertY")
        .Options(
            CheckboxOptions().Tooltip("Inverts the Y Axis of the Camera in First Person Mode.").DefaultValue(true));
    AddWidget(path, "X Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.SensitivityX")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the X Axis in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());
    AddWidget(path, "Y Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.SensitivityY")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the Y Axis in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());
    AddWidget(path, "Gyro Aiming", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.GyroEnabled")
        .Options(CheckboxOptions().Tooltip("Enables Gyro Aiming in First Person Mode."));
    AddWidget(path, "Invert Gyro X Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.GyroInvertX")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_GYRO_OFF).active; })
        .Options(CheckboxOptions().Tooltip("Inverts the X Axis of the Gyro in First Person Mode."));
    AddWidget(path, "Invert Gyro Y Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.GyroInvertY")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_GYRO_OFF).active; })
        .Options(CheckboxOptions().Tooltip("Inverts the Y Axis of the Gyro in First Person Mode."));
    AddWidget(path, "Gyro X Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.GyroSensitivityX")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the X Axis of the Gyro in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage())
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_GYRO_OFF).active; });
    AddWidget(path, "Gyro Y Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.GyroSensitivityY")
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the Y Axis of the Gyro in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage())
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_GYRO_OFF).active; });
    AddWidget(path, "Right Stick Aiming", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.RightStickEnabled")
        .Options(CheckboxOptions().Tooltip("Enables Right Stick Aiming in First Person Mode."));
    AddWidget(path, "Move while aiming", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.MoveInFirstPerson")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_RIGHT_STICK_OFF).active)
                info.activeDisables.push_back(DISABLE_FOR_RIGHT_STICK_OFF);
        })
        .Options(CheckboxOptions().Tooltip("Allows movement with the left stick while in first person mode."));
    AddWidget(path, "Invert Right Stick X Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.RightStickInvertX")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_RIGHT_STICK_OFF).active; })
        .Options(CheckboxOptions().Tooltip("Inverts the X Axis of the Right Stick in First Person Mode."));
    AddWidget(path, "Invert Right Stick Y Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FirstPerson.RightStickInvertY")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_RIGHT_STICK_OFF).active; })
        .Options(CheckboxOptions()
                     .Tooltip("Inverts the Y Axis of the Right Stick in First Person Mode.")
                     .DefaultValue(true));
    AddWidget(path, "Right Stick X Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.RightStickSensitivityX")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_RIGHT_STICK_OFF).active; })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the X Axis of the Right Stick in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());
    AddWidget(path, "Right Stick Y Axis Sensitivity: %.0f%%", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FirstPerson.RightStickSensitivityY")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_RIGHT_STICK_OFF).active; })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the Sensitivity of the Y Axis of the Right Stick in First Person Mode.")
                     .Min(0.01f)
                     .Max(2.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());

    path.column = 2;
    AddWidget(path, "Cameras", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Free Look", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.FreeLook.Enable")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_CAM_ON).active)
                info.activeDisables.push_back(DISABLE_FOR_DEBUG_CAM_ON);
        })
        .Options(CheckboxOptions().Tooltip(
            "Enables free look camera control\nNote: You must remap C buttons off of the right "
            "stick in the controller config menu, and map the camera stick to the right stick."));
    AddWidget(path, "Camera Distance: %d", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Camera.FreeLook.MaxCameraDistance")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FREE_LOOK_OFF).active; })
        .Options(
            IntSliderOptions().Tooltip("Maximum Camera Distance for Free Look.").Min(100).Max(900).DefaultValue(185));
    AddWidget(path, "Camera Transition Speed: %d", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Camera.FreeLook.TransitionSpeed")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FREE_LOOK_OFF).active; })
        .Options(IntSliderOptions().Tooltip("Can someone help me?").Min(1).Max(900).DefaultValue(25));
    AddWidget(path, "Max Camera Height Angle: %.0f\xC2\xB0", WIDGET_CVAR_SLIDER_FLOAT)
        .Callback([](WidgetInfo& info) { FreeLookPitchMinMax(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FREE_LOOK_OFF).active; })
        .CVar("gEnhancements.Camera.FreeLook.MaxPitch")
        .Options(FloatSliderOptions()
                     .Tooltip("Maximum Height of the Camera.")
                     .Format("%.0f\xC2\xB0")
                     .Min(-89.0f)
                     .Max(89.0f)
                     .DefaultValue(72.0f));
    AddWidget(path, "Min Camera Height Angle: %.0f\xC2\xB0", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.FreeLook.MinPitch")
        .Callback([](WidgetInfo& info) { FreeLookPitchMinMax(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FREE_LOOK_OFF).active; })
        .Options(FloatSliderOptions()
                     .Tooltip("Minimum Height of the Camera.")
                     .Format("%.0f\xC2\xB0")
                     .Min(-89.0f)
                     .Max(89.0f)
                     .DefaultValue(-49.0f));
    AddWidget(path, "Debug Camera", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.DebugCam.Enable")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_FREE_LOOK_ON).active) {
                info.activeDisables.push_back(DISABLE_FOR_FREE_LOOK_ON);
            }
        })
        .Options(CheckboxOptions().Tooltip("Enables debug camera control."));
    AddWidget(path, "Invert Camera X Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.RightStick.InvertXAxis")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_CAMERAS_OFF).active) {
                info.activeDisables.push_back(DISABLE_FOR_CAMERAS_OFF);
            }
        })
        .Options(CheckboxOptions().Tooltip("Inverts the Camera X Axis"));
    AddWidget(path, "Invert Camera Y Axis", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.RightStick.InvertYAxis")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_CAMERAS_OFF).active) {
                info.activeDisables.push_back(DISABLE_FOR_CAMERAS_OFF);
            }
        })
        .Options(CheckboxOptions().Tooltip("Inverts the Camera Y Axis").DefaultValue(true));
    AddWidget(path, "Third-Person Horizontal Sensitivity: %.0f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.RightStick.CameraSensitivity.X")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_CAMERAS_OFF).active) {
                info.activeDisables.push_back(DISABLE_FOR_CAMERAS_OFF);
            }
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the Sensitivity of the x axis when in Third Person.")
                     .Format("%.0f%%")
                     .Min(0.01f)
                     .Max(5.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());
    AddWidget(path, "Third-Person Vertical Sensitivity: %.0f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.RightStick.CameraSensitivity.Y")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_CAMERAS_OFF).active) {
                info.activeDisables.push_back(DISABLE_FOR_CAMERAS_OFF);
            }
        })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjust the Sensitivity of the x axis when in Third Person.")
                     .Format("%.0f%%")
                     .Min(0.01f)
                     .Max(5.0f)
                     .DefaultValue(1.0f)
                     .IsPercentage());
    AddWidget(path, "Enable Roll (6\xC2\xB0 of Freedom)", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Camera.DebugCam.6DOF")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_CAM_OFF).active; })
        .Options(CheckboxOptions().Tooltip(
            "This allows for all six degrees of movement with the camera, NOTE: Yaw will work "
            "differently in this system, instead rotating around the focal point, rather than a polar axis."));
    AddWidget(path, "Camera Speed: %.0f", WIDGET_CVAR_SLIDER_FLOAT)
        .CVar("gEnhancements.Camera.DebugCam.CameraSpeed")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_CAM_OFF).active; })
        .Options(FloatSliderOptions()
                     .Tooltip("Adjusts the speed of the Camera.")
                     .Format("%.0f%%")
                     .Min(0.1f)
                     .Max(3.0f)
                     .DefaultValue(0.5f)
                     .IsPercentage());

    path = { "Enhancements", "Cheats", 1 };
    AddSidebarEntry("Enhancements", "Cheats", 3);
    AddWidget(path, "Infinite Health", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.InfiniteHealth")
        .Options(CheckboxOptions().Tooltip("Always have full Hearts."));
    AddWidget(path, "Infinite Magic", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.InfiniteMagic")
        .Options(CheckboxOptions().Tooltip("Always have full Magic."));
    AddWidget(path, "Infinite Rupees", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.InfiniteRupees")
        .Options(CheckboxOptions().Tooltip("Always have a full Wallet."));
    AddWidget(path, "Infinite Consumables", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.InfiniteConsumables")
        .Options(
            CheckboxOptions().Tooltip("Always have max Consumables, you must have collected the consumables first."));
    AddWidget(path, "Easy Frame Advance", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.EasyFrameAdvance")
        .Options(CheckboxOptions().Tooltip(
            "Continue holding START button when unpausing to only advance a single frame and then re-pause"));
    AddWidget(path, "Longer Deku Flower Glide", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.LongerFlowerGlide")
        .Options(CheckboxOptions().Tooltip(
            "Allows Deku Link to glide longer, no longer dropping after a certain distance."));
    AddWidget(path, "No Clip", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.NoClip")
        .Options(CheckboxOptions().Tooltip("Allows Link to phase through collision."));
    AddWidget(path, "Unbreakable Razor Sword", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.UnbreakableRazorSword")
        .Options(CheckboxOptions().Tooltip("Allows to Razor Sword to be used indefinitely without dulling its blade."));
    AddWidget(path, "Unrestricted Items", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.UnrestrictedItems")
        .Options(CheckboxOptions().Tooltip("Allows all Forms to use all Items."));
    AddWidget(path, "Hookshot Anywhere", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.HookshotAnywhere")
        .Options(CheckboxOptions().Tooltip("Allows most surfaces to be hookshot-able"));
    AddWidget(path, "Moon Jump on L", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.MoonJumpOnL")
        .Options(CheckboxOptions().Tooltip("Holding L makes you float into the air."));
    AddWidget(path, "Elegy of Emptiness Anywhere", WIDGET_CVAR_CHECKBOX)
        .CVar("gCheats.ElegyAnywhere")
        .Options(CheckboxOptions().Tooltip("Allows Elegy of Emptiness outside of Ikana"));
    AddWidget(path, "Stop Time in Dungeons", WIDGET_CVAR_COMBOBOX)
        .CVar("gCheats.TempleTimeStop")
        .Options(
            ComboboxOptions()
                .Tooltip("Stops time from advancing in selected areas. Requires a room change to update.\n\n"
                         "- Off: Vanilla behaviour.\n"
                         "- Temples: Stops time in Woodfall, Snowhead, Great Bay, and Stone Tower Temples.\n"
                         "- Temples + Mini Dungeons: In addition to the above temples, stops time in both Spider "
                         "Houses, Pirate's Fortress, Beneath the Well, Ancient Castle of Ikana, and Secret Shrine.")
                .ComboMap(timeStopOptions));

    //// Gameplay Enhancements
    path = { "Enhancements", "Gameplay", 1 };
    AddSidebarEntry("Enhancements", "Gameplay", 3);
    AddWidget(path, "Player", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Fast Deku Flower Launch", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.FastFlowerLaunch")
        .Options(CheckboxOptions().Tooltip("Speeds up the time it takes to be able to get maximum height from"
                                           "launching out of a deku flower"));
    AddWidget(path, "Infinite Deku Hopping", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.InfiniteDekuHopping")
        .Options(CheckboxOptions().Tooltip("Allows Deku Link to hop indefinitely in water without drowning. This also "
                                           "prevents the velocity loss while in the air."));
    AddWidget(path, "Instant Putaway", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.InstantPutaway")
        .Options(CheckboxOptions().Tooltip("Allows Link to instantly puts away held item without waiting."));
    AddWidget(path, "Fierce Deity Putaway", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.FierceDeityPutaway")
        .Options(CheckboxOptions().Tooltip("Allows Fierce Deity Link to put away his sword."));
    AddWidget(path, "Climb speed", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Player.ClimbSpeed")
        .Options(IntSliderOptions()
                     .Tooltip("Increases the speed at which Link climbs vines and ladders.")
                     .Min(1)
                     .Max(5)
                     .DefaultValue(1));
    AddWidget(path, "Faster Push/Pull", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.FasterPushAndPull")
        .Options(CheckboxOptions().Tooltip("Speeds up the time it takes to push/pull various objects."));
    AddWidget(path, "Prevent Diving Over Water", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Player.PreventDiveOverWater")
        .Options(CheckboxOptions().Tooltip("Prevents Link from automatically diving over bodies of water."));
    AddWidget(path, "Dpad Equips", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Dpad.DpadEquips")
        .Options(CheckboxOptions().Tooltip("Allows you to equip items to your d-pad"));
    AddWidget(path, "Fast Magic Arrow Equip Animation", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Equipment.MagicArrowEquipSpeed")
        .Options(CheckboxOptions().Tooltip("Removes the animation for equipping Magic Arrows."));
    AddWidget(path, "Instant Fin Boomerangs Recall", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.PlayerActions.InstantRecall")
        .Options(CheckboxOptions().Tooltip(
            "Pressing B will instantly recall the fin boomerang back to Zora Link after they are thrown."));
    AddWidget(path, "Two-Handed Sword Spin Attack", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Equipment.TwoHandedSwordSpinAttack")
        .Options(CheckboxOptions().Tooltip(
            "Enables magic spin attacks for the Fierce Deity Sword and Great Fairy's Sword."));
    AddWidget(path, "Better Picto Message", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Equipment.BetterPictoMessage")
        .Options(
            CheckboxOptions().Tooltip("Inform the player what target if any is being captured in the pictograph."));
    AddWidget(path, "Arrow Type Cycling", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.PlayerActions.ArrowCycle")
        .Options(CheckboxOptions().Tooltip(
            "While aiming the bow, use L to cycle between Normal, Fire, Ice and Light arrows."));
    AddWidget(path, "Bombchu Drops", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Equipment.ChuDrops")
        .Options(
            CheckboxOptions().Tooltip("When a bomb drop is spawned, it has a 50% chance to be a bombchu instead."));

    path.column = 2;
    AddWidget(path, "Modes", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Play as Kafei", WIDGET_CVAR_CHECKBOX)
        .CVar("gModes.PlayAsKafei")
        .Options(CheckboxOptions().Tooltip("Requires scene reload to take effect."));
    AddWidget(path, "Hyrule Warriors Styled Link", WIDGET_CVAR_CHECKBOX)
        .CVar("gModes.HyruleWarriorsStyledLink")
        .Options(CheckboxOptions().Tooltip(
            "When acquired, places the Keaton and Fierce Deity masks on Link similarly to how he "
            "wears them in Hyrule Warriors"));
    AddWidget(path, "Time Moves when you Move", WIDGET_CVAR_CHECKBOX)
        .CVar("gModes.TimeMovesWhenYouMove")
        .Options(CheckboxOptions().Tooltip("Time only moves when Link is not standing still."));
    AddWidget(path, "Mirrored World", WIDGET_CVAR_CHECKBOX)
        .CVar("gModes.MirroredWorld.Mode")
        .Callback([](WidgetInfo& info) {
            if (CVarGetInteger("gModes.MirroredWorld.Mode", 0)) {
                CVarSetInteger("gModes.MirroredWorld.State", 1);
            } else {
                CVarClear("gModes.MirroredWorld.State");
            }
        })
        .Options(CheckboxOptions().Tooltip("Mirrors the world horizontally."));
    AddWidget(path, "Minigames", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Always Win Doggy Race", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.Minigames.AlwaysWinDoggyRace")
        .Options(ComboboxOptions().Tooltip("Makes the Doggy Race easier to win.").ComboMap(alwaysWinDoggyraceOptions));
    AddWidget(path, "Milk Run Reward Options", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.Minigames.CremiaHugs")
        .Options(ComboboxOptions()
                     .Tooltip("Choose what reward you get for winning the Milk Run minigame after the first time. \n"
                              "-Vanilla: Reward is Random\n"
                              "-Hug: Get the hugging cutscene\n"
                              "-Rupee: Get the rupee reward")
                     .ComboMap(cremiaRewardOptions));
    AddWidget(path, "Cucco Shack Cucco Count", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.CuccoShackCuccoCount")
        .Options(IntSliderOptions()
                     .Tooltip("Choose how many cuccos you need to raise to make Grog happy.")
                     .Min(1)
                     .Max(10)
                     .DefaultValue(10));
    AddWidget(path, "Swordsman School Winning Score", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.SwordsmanSchoolScore")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Swordsman School.")
                     .Min(1)
                     .Max(30)
                     .DefaultValue(30));
    AddWidget(path, "Beaver Race Rings Collected", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.BeaverRaceRingsCollected")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the number of rings required for both Beavers. If the slider is set to 20, the "
                              "first Beaver will require 20 rings, and the second Beaver will require 25 rings, which "
                              "are their vanilla values.")
                     .Min(1)
                     .Max(20)
                     .DefaultValue(20));
    AddWidget(path, "Swamp Archery Perfect Score", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.SwampArcheryScore")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Swamp Archery minigame, if this is changed it also "
                              "speeds up the final score counting.")
                     .Min(1000)
                     .Max(2180)
                     .DefaultValue(2180));
    AddWidget(path, "Town Archery Perfect Score", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.TownArcheryScore")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Town Archery minigame. Reaching this score will end "
                              "the minigame.")
                     .Min(1)
                     .Max(50)
                     .DefaultValue(50));
    AddWidget(path, "Honey & Darling Day 1 (Bombchus)", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.HoneyAndDarlingDay1")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Honey & Darling minigame on Day 1.")
                     .Min(1)
                     .Max(8)
                     .DefaultValue(8));
    AddWidget(path, "Honey & Darling Day 2 (Bombs)", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.HoneyAndDarlingDay2")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Honey & Darling minigame on Day 2.")
                     .Min(1)
                     .Max(8)
                     .DefaultValue(8));
    AddWidget(path, "Honey & Darling Day 3 (Bow)", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.HoneyAndDarlingDay3")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win the Honey & Darling minigame on Day 3.")
                     .Min(1)
                     .Max(16)
                     .DefaultValue(16));
    AddWidget(path, "Skip Powder Keg Certification", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Minigames.PowderKegCertification")
        .Options(CheckboxOptions().Tooltip(
            "Skips requiring to take the Powder Keg Test before being given the Certification."));
    AddWidget(path, "Malon Target Practice Winning Score", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Minigames.MalonTargetPractice")
        .Options(IntSliderOptions()
                     .Tooltip("Sets the score required to win Malon's Target Practice.")
                     .Min(1)
                     .Max(10)
                     .DefaultValue(10));
    AddWidget(path, "Skip Gorman Horse Race", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Minigames.SkipHorseRace")
        .Options(CheckboxOptions().Tooltip("Instantly win the Gorman Horse Race"));
    AddWidget(path, "Skip Ballad of Windfish", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Minigames.SkipBalladOfWindfish")
        .Options(CheckboxOptions().Tooltip(
            "Play the complete Ballad after playing in one form if you have all three transformation masks."));

    path.column = 3;
    AddWidget(path, "Saving", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "3rd Save File Slot", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Saving.FileSlot3")
        .Options(CheckboxOptions().Tooltip("Adds a 3rd file slot that can be used for saves").DefaultValue(true));
    AddWidget(path, "Persistent Owl Saves", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Saving.PersistentOwlSaves")
        .Options(CheckboxOptions().Tooltip("Continuing a save will not remove the owl save. Playing Song of "
                                           "Time, allowing the moon to crash or finishing the "
                                           "game will remove the owl save and become the new last save."));
    AddWidget(path, "Pause Menu Save", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Saving.PauseSave")
        .Options(CheckboxOptions().Tooltip(
            "Re-introduce the pause menu save system. Pressing B in the pause menu will give you the "
            "option to create a persistent Owl Save from your current location.\n\nWhen loading back "
            "into the game, you will be placed either at the entrance of the dungeon you saved in, or "
            "in South Clock Town."));
    AddWidget(path, "Autosave", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Saving.Autosave")
        .Callback([](WidgetInfo& info) { RegisterAutosave(); })
        .Options(CheckboxOptions().Tooltip(
            "Automatically create a persistent Owl Save on the chosen interval.\n\nWhen loading "
            "back into the game, you will be placed either at the entrance of the dungeon you "
            "saved in, or in South Clock Town."));
    AddWidget(path, "Autosave Interval: %d minutes", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Saving.AutosaveInterval")
        .PreFunc([](WidgetInfo& info) {
            if (mBenMenu->disabledMap.at(DISABLE_FOR_AUTO_SAVE_OFF).active) {
                info.activeDisables.push_back(DISABLE_FOR_AUTO_SAVE_OFF);
            }
        })
        .Options(IntSliderOptions().Tooltip("Sets the interval between Autosaves.").Min(1).Max(60).DefaultValue(5));
    AddWidget(path, "Time Cycle", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Do not reset Bottle content", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.DoNotResetBottleContent")
        .Options(CheckboxOptions().Tooltip("Playing the Song Of Time will not reset the bottles' content."));
    AddWidget(path, "Do not reset Consumables", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.DoNotResetConsumables")
        .Options(CheckboxOptions().Tooltip("Playing the Song Of Time will not reset the consumables."));
    AddWidget(path, "Do not reset Razor Sword", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.DoNotResetRazorSword")
        .Options(CheckboxOptions().Tooltip("Playing the Song Of Time will not reset the Sword back to Kokiri Sword."));
    AddWidget(path, "Do not reset Rupees", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.DoNotResetRupees")
        .Options(CheckboxOptions().Tooltip("Playing the Song Of Time will not reset the your rupees."));
    AddWidget(path, "Do not reset Time Speed", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.DoNotResetTimeSpeed")
        .Options(CheckboxOptions().Tooltip(
            "Playing the Song Of Time will not reset the current time speed set by Inverted Song of Time."));
    AddWidget(path, "Keep Express Mail", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cycle.KeepExpressMail")
        .Options(CheckboxOptions().Tooltip(
            "Allows the player to keep the Express Mail in their inventory after delivering it "
            "the first time, so that both deliveries can be done within one cycle"));
    AddWidget(path, "Unstable", WIDGET_SEPARATOR_TEXT).Options(WidgetOptions().Color(Colors::Orange));
    AddWidget(path, "Disable Save Delay", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Saving.DisableSaveDelay")
        .Options(CheckboxOptions().Tooltip(
            "Removes the arbitrary 2 second timer for saving from the original game. This is known to "
            "cause issues when attempting the 0th Day Glitch"));

    //// Graphics Enhancements
    path = { "Enhancements", "Graphics", 1 };
    AddSidebarEntry("Enhancements", "Graphics", 3);
    AddWidget(path, "Clock", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Clock Type", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.Graphics.ClockType")
        .Options(
            ComboboxOptions().Tooltip("Swaps between Graphical and Text only Clock types.").ComboMap(clockTypeOptions));
    AddWidget(path, "24 Hours Clock", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.24HoursClock")
        .Options(CheckboxOptions().Tooltip("Changes from a 12 Hour to a 24 Hour Clock"));
    AddWidget(path, "Mods", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Use Alternate Assets", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Mods.AlternateAssets")
        .Options(CheckboxOptions().Tooltip(
            "Toggle between standard assets and alternate assets. Usually mods will indicate if "
            "this setting has to be used or not."));
    AddWidget(path, "Motion Blur", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Motion Blur Mode", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.Graphics.MotionBlur.Mode")
        .Options(ComboboxOptions()
                     .Tooltip("Selects the Mode for Motion Blur.")
                     .LabelPosition(LabelPosition::None)
                     .ComboMap(motionBlurOptions));
    AddWidget(path, "Interpolate", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.MotionBlur.Interpolate")
        .PreFunc(
            [](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_MOTION_BLUR_MODE).value == 1; })
        .Options(CheckboxOptions().Tooltip(
            "Change motion blur capture to also happen on interpolated frames instead of only on game frames.\n"
            "This notably reduces the overall motion blur strength but smooths out the trails."));
    AddWidget(path, "On/Off", WIDGET_CHECKBOX)
        .ValuePointer((bool*)&R_MOTION_BLUR_ENABLED)
        .PreFunc([](WidgetInfo& info) {
            info.valuePointer = (bool*)&R_MOTION_BLUR_ENABLED;
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_MOTION_BLUR_MODE).value != 0;
        });
    AddWidget(path, "Strength", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Graphics.MotionBlur.Strength")
        .Options(IntSliderOptions().Tooltip("Motion Blur strength.").Min(0).Max(255).DefaultValue(180))
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_MOTION_BLUR_MODE).value != 2;
        });
    AddWidget(path, "Strength", WIDGET_SLIDER_INT)
        .Options(IntSliderOptions().Tooltip("Motion Blur strength.").Min(0).Max(255).DefaultValue(180))
        .ValuePointer(&motionBlurStrength)
        .Callback([](WidgetInfo& info) { R_MOTION_BLUR_ALPHA = motionBlurStrength; })
        .PreFunc([](WidgetInfo& info) {
            motionBlurStrength = R_MOTION_BLUR_ALPHA;
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_MOTION_BLUR_MODE).value != 0 ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_MOTION_BLUR_OFF).active;
        });

    path.column = 2;
    AddWidget(path, "Other", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "3D Item Drops", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.3DItemDrops")
        .Options(CheckboxOptions().Tooltip("Makes item drops 3D"));
    AddWidget(path, "Authentic Logo", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.AuthenticLogo")
        .Options(CheckboxOptions().Tooltip("Hide the game version and build details and display the authentic "
                                           "model and texture on the boot logo start screen"));
    AddWidget(path, "Bow Reticle", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.BowReticle")
        .Options(CheckboxOptions().Tooltip("Gives the bow a reticle when you draw an arrow."));
    AddWidget(path, "Disable Black Bar Letterboxes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.DisableBlackBars")
        .Options(CheckboxOptions().Tooltip(
            "Disables Black Bar Letterboxes during cutscenes and Z-targeting\nNote: there may be "
            "minor visual glitches that were covered up by the black bars\nPlease disable this "
            "setting before reporting a bug."));
    AddWidget(path, "Unstable", WIDGET_SEPARATOR_TEXT).Options(WidgetOptions{ .color = Colors::Orange });
    AddWidget(path, "Disable Scene Geometry Distance Check", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.DisableSceneGeometryDistanceCheck")
        .Callback([](WidgetInfo& info) { GfxPatcher_ApplyGeometryIssuePatches(); })
        .Options(CheckboxOptions().Tooltip(
            "Disables the distance check for scene geometry, allowing it to be drawn no matter how far "
            "away it is from the player. This may have unintended side effects."));
    AddWidget(path, "Widescreen Actor Culling", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Graphics.ActorCullingAccountsForWidescreen")
        .Options(CheckboxOptions().Tooltip("Adjusts the culling planes to account for widescreen resolutions. "
                                           "This may have unintended side effects."));
    AddWidget(path, "Increase Actor Draw Distance: %dx", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Graphics.IncreaseActorDrawDistance")
        .Options(IntSliderOptions()
                     .Tooltip("Increase the range in which Actors are drawn. This may have unintended side effects.")
                     .Min(1)
                     .Max(5)
                     .DefaultValue(1));

    path = { "Enhancements", "Items/Songs", 1 };
    AddSidebarEntry("Enhancements", "Items/Songs", 3);
    // Mask Enhancements
    AddWidget(path, "Masks", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Blast Mask has Powder Keg Force", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.BlastMaskKeg")
        .Options(CheckboxOptions().Tooltip("Blast Mask can also destroy objects only the Powder Keg can."));
    AddWidget(path, "Fast Transformation", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.FastTransformation")
        .Options(CheckboxOptions().Tooltip("Removes the delay when using transformation masks."));
    AddWidget(path, "Fierce Deity's Mask Anywhere", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.FierceDeitysAnywhere")
        .Options(CheckboxOptions().Tooltip("Allow using Fierce Deity's mask outside of boss rooms."));
    AddWidget(path, "Persistent Bunny Hood", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.PersistentBunnyHood.Enabled")
        .Options(CheckboxOptions().Tooltip(
            "Permanently toggle a speed boost from the bunny hood by pressing 'A' on it in the mask menu."));
    AddWidget(path, "No Blast Mask Cooldown", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.NoBlastMaskCooldown")
        .Options(CheckboxOptions().Tooltip("Eliminates the Cooldown between Blast Mask usage."));
    AddWidget(path, "Goron Rolling Ignores Magic", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.GoronRollingIgnoresMagic")
        .Options(CheckboxOptions().Tooltip(
            "Goron rolling will use spikes even when Link doesn't have magic, and doesn't consume any."));
    AddWidget(path, "Goron Rolling Fast Spikes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Masks.GoronRollingFastSpikes")
        .Options(CheckboxOptions().Tooltip("Speeds up the wind-up towards spiky rolling to be near instant."));

    // Song Enhancements
    path.column = 2;
    AddWidget(path, "Ocarina", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Better Song of Double Time", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Songs.BetterSongOfDoubleTime")
        .Options(CheckboxOptions().Tooltip(
            "When playing the Song of Double Time, you can now choose the exact time you want to go "
            "to, similar to the 3DS version.\n\n"
            "Holding Z allows decreasing the time adjustment factor, while holding R will increase the factor"));
    AddWidget(path, "Enable Sun's Song", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Songs.EnableSunsSong")
        .Options(CheckboxOptions().Tooltip(
            "Enables the partially implemented Sun's Song. RIGHT-DOWN-UP-RIGHT-DOWN-UP to play it. "
            "This song will make time move very fast until either Link moves to a different scene, "
            "or when the time switches to a new time period."));
    AddWidget(path, "Dpad Ocarina", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Playback.DpadOcarina")
        .Options(CheckboxOptions().Tooltip("Enables using the Dpad for Ocarina playback."));
    AddWidget(path, "Pause Owl Warp", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Songs.PauseOwlWarp")
        .Options(CheckboxOptions().Tooltip(
            "Allows warping to registered Owl Statues from the pause menu map screen. "
            "Requires that you can play Song of Soaring normally.\n\n"
            "Accounts for Index-Warp being active, by presenting all valid warps for the registered "
            "map points. Great Bay Coast warp is always given for index 0 warp as a convenience."));
    AddWidget(path, "Zora Eggs For Bossa Nova", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Songs.ZoraEggCount")
        .Options(IntSliderOptions()
                     .Tooltip("The number of eggs required to unlock new wave bossa nova.")
                     .Min(1)
                     .Max(7)
                     .DefaultValue(7));
    AddWidget(path, "Prevent Dropped Ocarina Inputs", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Playback.NoDropOcarinaInput")
        .Options(CheckboxOptions().Tooltip("Prevent dropping inputs when playing the ocarina quickly."));
    AddWidget(path, "Skip Scarecrow Song", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Playback.SkipScarecrowSong")
        .Options(CheckboxOptions().Tooltip("Pierre appears when the Ocarina is pulled out."));
    AddWidget(path, "Faster Song Playback", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Songs.FasterSongPlayback")
        .Options(CheckboxOptions().Tooltip("Speeds up the playback of songs."));
    AddWidget(path, "Skip Song of Time cutscenes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Songs.SkipSoTCutscenes")
        .Options(CheckboxOptions().Tooltip("Skips the cutscenes when playing any of the Song of Time songs"));

    // Time Savers
    path = { "Enhancements", "Time Savers", 1 };
    AddSidebarEntry("Enhancements", "Time Savers", 3);
    // Cutscene Skips
    AddWidget(path, "Cutscenes", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Hide Title Cards", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.HideTitleCards")
        .Options(CheckboxOptions().Tooltip("Hides Title Cards when entering areas."));
    AddWidget(path, "Skip One Point Cutscenes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipOnePointCutscenes")
        .Options(CheckboxOptions().Tooltip(
            "Skips freezing Link to focus on various events like chest spawning, door unlocking, switch pressed, etc"));
    AddWidget(path, "Skip Entrance Cutscenes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipEntranceCutscenes")
        .Options(CheckboxOptions().Tooltip("Skip cutscenes that occur when first entering a new area."));
    AddWidget(path, "Skip to File Select", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipToFileSelect")
        .Options(CheckboxOptions().Tooltip(
            "Skip the opening title sequence and go straight to the file select menu after boot."));
    AddWidget(path, "Skip Intro Sequence", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipIntroSequence")
        .Options(CheckboxOptions().Tooltip(
            "When starting a game you will be taken straight to South Clock Town as Deku Link."));
    AddWidget(path, "Skip First Cycle", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipFirstCycle")
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_INTRO_SKIP_OFF).active; })
        .Options(CheckboxOptions().Tooltip(
            "When starting a game you will be taken straight to South Clock Town as Human Link "
            "with Deku Mask, Ocarina, Song of Time, and Song of Healing."));
    AddWidget(path, "Skip Story Cutscenes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipStoryCutscenes")
        .Options(CheckboxOptions().Tooltip(
            "Disclaimer: This doesn't do much yet, we will be progressively adding more skips over time."));
    AddWidget(path, "Skip Misc Interactions", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Cutscenes.SkipMiscInteractions")
        .Options(CheckboxOptions().Tooltip(
            "Disclaimer: This doesn't do much yet, we will be progressively adding more skips over time."));
    AddWidget(path, "Skip Item Get Cutscene", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.Cutscenes.SkipGetItemCutscenes")
        .Options(ComboboxOptions()
                     .Tooltip("Note: This only works in Randomizer currently")
                     .ComboMap(skipGetItemCutscenesOptions));

    // Dialogue Enhancements
    path.column = 2;
    AddWidget(path, "Dialogue", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Fast Bank Selection", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Dialogue.FastBankSelection")
        .Options(CheckboxOptions().Tooltip(
            "Pressing the Z or R buttons while the Deposit/Withdrawal Rupees dialogue is open will set "
            "the Rupees to Links current Rupees or 0 respectively."));
    AddWidget(path, "Fast Text", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Dialogue.FastText")
        .Options(
            CheckboxOptions().Tooltip("Speeds up text rendering, and enables holding of B progress to next message."));
    path.column = 3;
    AddWidget(path, "Other", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Swamp Boat Timesaver", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Timesavers.SwampBoatSpeed")
        .Options(CheckboxOptions().Tooltip("Hold Z to speed up the boat ride in through the Swamp."));
    AddWidget(path, "Shooting Gallery Both Rewards", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Timesavers.GalleryTwofer")
        .Options(CheckboxOptions().Tooltip("When getting a perfect score at the Shooting Gallery, receive both rewards "
                                           "back to back instead of having to play twice."));
    AddWidget(path, "Fast Marine Lab Fish", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Timesavers.MarineLabHP")
        .Options(CheckboxOptions().Tooltip("Only requires a single fish to be fed for the Piece of Heart to spawn. "
                                           "Requires a Scene Reload to take effect."));

    // Fixes
    path = { "Enhancements", "Fixes", 1 };
    AddSidebarEntry("Enhancements", "Fixes", 3);
    AddWidget(path, "Fixes", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Fix Console Crashes", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Fixes.ConsoleCrashes")
        .Options(CheckboxOptions()
                     .Tooltip("Fixes crashes that would typically happen on Console. "
                              "Disabling this option will simply soft reset 2Ship when encountering these "
                              "crashes instead of actually crashing the program.\n\n"
                              "Includes the following:\n"
                              "- HESS/Weirdshot crashes\n"
                              "- Action Swap crash without arrow ammo\n"
                              "- Owl Warp menu crash when moving the cursor with Index-Warp active\n"
                              "- Remote Hookshot Hookslide crashes when over voids in Great Bay Temple")
                     .DefaultValue(true));
    AddWidget(path, "Fix Ammo Count Color", WIDGET_CVAR_CHECKBOX)
        .CVar("gFixes.FixAmmoCountEnvColor")
        .Options(CheckboxOptions().Tooltip("Fixes a missing gDPSetEnvColor, which causes the ammo count to be "
                                           "the wrong color prior to obtaining magic or other conditions."));
    AddWidget(path, "Fix Epona stealing Sword", WIDGET_CVAR_CHECKBOX)
        .CVar("gFixes.FixEponaStealingSword")
        .Options(CheckboxOptions().Tooltip(
            "This fixes a bug where Epona can steal your sword when you mount her without a bow in your inventory."));
    AddWidget(path, "Fix Fierce Deity Z-Target movement", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Fixes.FierceDeityZTargetMovement")
        .Options(CheckboxOptions().Tooltip("Fixes Fierce Deity movement being choppy when Z-targeting"));
    AddWidget(path, "Fix Text Control Characters", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Fixes.ControlCharacters")
        .Options(CheckboxOptions().Tooltip("Fixes certain control characters not functioning properly "
                                           "depending on their position within the text."));
    AddWidget(path, "Fix Ikana Great Fairy Fountain Color", WIDGET_CVAR_CHECKBOX)
        .CVar("gFixes.FixIkanaGreatFairyFountainColor")
        .Options(CheckboxOptions().Tooltip(
            "Fixes a bug that results in the Ikana Great Fairy fountain looking green instead of "
            "yellow, this was fixed in the EU version"));
    AddWidget(path, "Fix Texture overflow OOB", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Fixes.FixTexturesOOB")
        .Options(CheckboxOptions()
                     .Tooltip("Fixes textures that normally overflow to be patched with the correct size or format")
                     .DefaultValue(true))
        .Callback([](WidgetInfo& info) { GfxPatcher_ApplyOverflowTexturePatches(); });
    AddWidget(path, "Fix Completed Heart Container Audio", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Fixes.CompletedHeartContainerAudio")
        .Options(CheckboxOptions().Tooltip(
            "Fixes a bug that results in the wrong audio playing upon receiving a 4th piece of heart to "
            "fill a new heart container."));

    // Restorations
    path = { "Enhancements", "Restorations", 1 };
    AddSidebarEntry("Enhancements", "Restorations", 3);
    AddWidget(path, "Restorations", WIDGET_SEPARATOR_TEXT);
    AddWidget(path, "Constant Distance Backflips and Sidehops", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.ConstantFlipsHops")
        .Options(CheckboxOptions().Tooltip("Backflips and Sidehops travel a constant distance as they did in OoT."));
    AddWidget(path, "Power Crouch Stab", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.PowerCrouchStab")
        .Options(CheckboxOptions().Tooltip(
            "Crouch stabs will use the power of Link's previous melee attack, as is in MM JP 1.0 and OoT."));
    AddWidget(path, "Side Rolls", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.SideRoll")
        .Options(CheckboxOptions().Tooltip("Restores side rolling from OoT."));
    AddWidget(path, "Faster Swim", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.OoTFasterSwim")
        .Options(CheckboxOptions().Tooltip("Restores the ability to swim faster by spamming the B button, as in OoT."));
    AddWidget(path, "Tatl ISG", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.TatlISG")
        .Options(CheckboxOptions().Tooltip("Restores Navi ISG from OoT, but now with Tatl."));
    AddWidget(path, "Woodfall Mountain Appearance", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.Restorations.WoodfallMountainAppearance")
        .Options(CheckboxOptions().Tooltip("Restores the appearance of Woodfall mountain to not look poisoned "
                                           "when viewed from Termina Field after clearing Woodfall Temple\n\n"
                                           "Requires a scene reload to take effect"));
    AddWidget(path, "Simulated Input Lag", WIDGET_CVAR_SLIDER_INT)
        .CVar(CVAR_SIMULATED_INPUT_LAG)
        .Options(IntSliderOptions()
                     .Tooltip("Buffers your inputs to be executed a specified amount of frames later")
                     .Min(0)
                     .Max(6)
                     .DefaultValue(0));
    AddWidget(path, "Pause Buffer Input Window", WIDGET_CVAR_SLIDER_INT)
        .CVar("gEnhancements.Restorations.PauseBufferWindow")
        .Options(IntSliderOptions()
                     .Tooltip("Amount of time in frames you have to buffer an input while unpausing the game. Original "
                              "hardware is around 20")
                     .Min(0)
                     .Max(40)
                     .DefaultValue(0));

    // Difficulty Options
    path = { "Enhancements", "Difficulty Options", 1 };
    AddSidebarEntry("Enhancements", "Difficulty Options", 3);
    AddWidget(path, "Disable Takkuri Steal", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.DifficultyOptions.DisableTakkuriSteal")
        .Options(CheckboxOptions().Tooltip(
            "Prevents the Takkuri from stealing key items like bottles and swords. It may still steal "
            "other items."));
    AddWidget(path, "Deku Guard Search Balls", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.DifficultyOptions.DekuGuardSearchBalls")
        .Options(
            ComboboxOptions()
                .Tooltip("Choose when to show the Deku Palace Guards' search balls\n"
                         "- Never: Never show the search balls. This matches Majora's Mask 3D behaviour\n"
                         "- Night Only: Only show the search balls at night. This matches original N64 behaviour.\n"
                         "- Always: Always show the search balls.")
                .DefaultIndex(DekuGuardSearchBallsOptions::DEKU_GUARD_SEARCH_BALLS_NIGHT_ONLY)
                .ComboMap(dekuGuardSearchBallsOptions));
    AddWidget(path, "Lower Bank Reward Thresholds", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.DifficultyOptions.LowerBankRewardThresholds")
        .Options(
            CheckboxOptions().Tooltip("Reduces the amount of rupees required to receive the rewards from the bank.\n"
                                      "From: 200 -> 1000 -> 5000\n"
                                      "To:   100 ->  500 -> 1000"));
    AddWidget(path, "Gibdo Trade Sequence Options", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.DifficultyOptions.GibdoTradeSequence")
        .Options(
            ComboboxOptions()
                .Tooltip(
                    "Changes the way the Gibdo Trade Sequence works\n"
                    "-Vanilla: Works normally\n"
                    "-MM3D: Gibdos will only take one quantity of the item they request, as they do in MM3D. The Gibdo "
                    "requesting a blue potion will also accept a red potion.\n"
                    "-No trade: Gibdos will vanish without taking items")
                .DefaultIndex(GibdoTradeSequenceOptions::GIBDO_TRADE_SEQUENCE_VANILLA)
                .ComboMap(gibdoTradeSequenceOptions));

    path.column = 2;
    AddWidget(path, "Damage Multiplier", WIDGET_CVAR_COMBOBOX)
        .CVar("gEnhancements.DifficultyOptions.DamageMultiplier")
        .Options(ComboboxOptions()
                     .Tooltip("Adjusts the amount of damage Link takes from all sources.")
                     .ComboMap(damageMultiplierOptions));
    AddWidget(path, "Permanent Heart Loss", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.DifficultyOptions.PermanentHeartLoss")
        .Options(CheckboxOptions().Tooltip(
            "When you lose 4 quarters of a heart you will permanently lose that heart container.\n\nDisabling this "
            "after the fact will not restore any received heart containers."));
    AddWidget(path, "Delete File on Death", WIDGET_CVAR_CHECKBOX)
        .CVar("gEnhancements.DifficultyOptions.DeleteFileOnDeath")
        .Options(CheckboxOptions().Tooltip("Dying will delete your file\n\n     " ICON_FA_EXCLAMATION_TRIANGLE
                                           " WARNING " ICON_FA_EXCLAMATION_TRIANGLE
                                           "\nTHIS IS NOT REVERSABLE\nUSE AT YOUR OWN RISK!"));

    // HUD Editor
    path = { "Enhancements", "HUD Editor", 1 };
    AddSidebarEntry("Enhancements", "HUD Editor", 1);
    AddWidget(path, "Popout HUD Editor", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.HudEditor")
        .WindowName("HUD Editor")
        .Options(ButtonOptions()
                     .Tooltip("Enables the HUD Editor window, allowing you to modify your HUD")
                     .Size(Sizes::Inline));

    // Item Tracker Settings
    path = { "Enhancements", "Item Tracker", 1 };
    AddSidebarEntry("Enhancements", "Item Tracker", 1);
    AddWidget(path, "Popout Settings", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.ItemTrackerSettings")
        .WindowName("Item Tracker Settings");
}

void BenMenu::AddDevTools() {
    WidgetPath path = { "Dev Tools", "General", 1 };
    AddSidebarEntry("Dev Tools", "General", 3);
    AddWidget(path, "Popout Menu", WIDGET_CVAR_CHECKBOX)
        .CVar("gSettings.Menu.Popout")
        .Options(CheckboxOptions().Tooltip("Changes the menu display from overlay to windowed."));
    AddWidget(path, "Set Warp Point", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Creates warp point that you can teleport to later"))
        .Callback([](WidgetInfo& info) {
            Player* player = GET_PLAYER(gPlayState);

            CVarSetInteger(WARP_POINT_CVAR "Entrance", gSaveContext.save.entrance);
            CVarSetInteger(WARP_POINT_CVAR "Room", gPlayState->roomCtx.curRoom.num);
            CVarSetFloat(WARP_POINT_CVAR "X", player->actor.world.pos.x);
            CVarSetFloat(WARP_POINT_CVAR "Y", player->actor.world.pos.y);
            CVarSetFloat(WARP_POINT_CVAR "Z", player->actor.world.pos.z);
            CVarSetFloat(WARP_POINT_CVAR "Rotation", player->actor.shape.rot.y);
            CVarSetInteger(WARP_POINT_CVAR "Saved", 1);
            Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
        })
        .PreFunc(
            [](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NULL_PLAY_STATE).active; });
    AddWidget(path, "Scene Room ID", WIDGET_TEXT).PreFunc([](WidgetInfo& info) {
        u32 sceneId =
            Entrance_GetSceneIdAbsolute(CVarGetInteger(WARP_POINT_CVAR "Entrance", ENTRANCE(SOUTH_CLOCK_TOWN, 0)));
        info.name = fmt::format("{} Room {}", warpPointSceneList[sceneId], CVarGetInteger(WARP_POINT_CVAR "Room", 0));
        info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NULL_PLAY_STATE).active ||
                        mBenMenu->disabledMap.at(DISABLE_FOR_WARP_POINT_NOT_SET).active;
    });
    AddWidget(path, ICON_FA_TIMES, WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Clear warp point").Size(Sizes::Inline))
        .Callback([](WidgetInfo& info) {
            CVarClear(WARP_POINT_CVAR "Entrance");
            CVarClear(WARP_POINT_CVAR "Room");
            CVarClear(WARP_POINT_CVAR "X");
            CVarClear(WARP_POINT_CVAR "Y");
            CVarClear(WARP_POINT_CVAR "Z");
            CVarClear(WARP_POINT_CVAR "Rotation");
            CVarClear(WARP_POINT_CVAR "Saved");
            Ship::Context::GetInstance()->GetWindow()->GetGui()->SaveConsoleVariablesNextFrame();
        })
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NULL_PLAY_STATE).active ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_WARP_POINT_NOT_SET).active;
        })
        .SameLine(true);
    AddWidget(path, "Warp", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Teleport to the set warp point").Size(Sizes::Inline))
        .Callback([](WidgetInfo& info) { Warp(); })
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NULL_PLAY_STATE).active ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_WARP_POINT_NOT_SET).active;
        })
        .SameLine(true);
    AddWidget(path, "Debug Mode", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.DebugEnabled")
        .Options(CheckboxOptions().Tooltip("Enables Debug Mode, allowing you to select maps with L + R + Z."))
        .Callback([](WidgetInfo& info) {
            if (!CVarGetInteger("gDeveloperTools.DebugEnabled", 0)) {
                CVarClear("gDeveloperTools.DebugSaveFileMode");
                CVarClear("gDeveloperTools.PreventActorUpdate");
                CVarClear("gDeveloperTools.PreventActorDraw");
                CVarClear("gDeveloperTools.PreventActorInit");
                CVarClear("gDeveloperTools.DisableObjectDependency");
                if (gPlayState != NULL) {
                    gPlayState->frameAdvCtx.enabled = false;
                }
                RegisterDebugSaveCreate();
                RegisterPreventActorUpdateHooks();
                RegisterPreventActorDrawHooks();
                RegisterPreventActorInitHooks();
            }
        });
    AddWidget(path, "Better Map Select", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.BetterMapSelect.Enabled")
        .Options(CheckboxOptions().Tooltip(
            "Overrides the original map select with a translated, more user-friendly version."))
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Debug Save File Mode", WIDGET_CVAR_COMBOBOX)
        .CVar("gDeveloperTools.DebugSaveFileMode")
        .Options(ComboboxOptions()
                     .Tooltip("Change the behavior of creating saves while debug mode is enabled:\n\n"
                              "- Empty Save: The default 3 heart save file in first cycle\n"
                              "- Vanilla Debug Save: Uses the title screen save info (8 hearts, all items and masks)\n"
                              "- 100\% Save: All items, equipment, mask, quest status and bombers notebook complete")
                     .ComboMap(debugSaveOptions))
        .Callback([](WidgetInfo& info) { RegisterDebugSaveCreate(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Prevent Actor Update", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.PreventActorUpdate")
        .Options(CheckboxOptions().Tooltip("Prevents Actors from updating."))
        .Callback([](WidgetInfo& info) { RegisterPreventActorUpdateHooks(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Prevent Actor Draw", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.PreventActorDraw")
        .Options(CheckboxOptions().Tooltip("Prevents Actors from drawing."))
        .Callback([](WidgetInfo& info) { RegisterPreventActorDrawHooks(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Prevent Actor Init", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.PreventActorInit")
        .Options(CheckboxOptions().Tooltip("Prevents Actors from initializing."))
        .Callback([](WidgetInfo& info) { RegisterPreventActorInitHooks(); })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Disable Object Dependency", WIDGET_CVAR_CHECKBOX)
        .CVar("gDeveloperTools.DisableObjectDependency")
        .Options(CheckboxOptions().Tooltip("Disables dependencies when loading objects."))
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Log Level", WIDGET_CVAR_COMBOBOX)
        .CVar("gDeveloperTools.LogLevel")
        .Options(ComboboxOptions()
                     .Tooltip("The log level determines which messages are printed to the "
                              "console. This does not affect the log file output")
                     .ComboMap(logLevels))
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetLogger()->set_level(
                (spdlog::level::level_enum)CVarGetInteger("gDeveloperTools.LogLevel", 1));
        })
        .PreFunc([](WidgetInfo& info) { info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active; });
    AddWidget(path, "Frame Advance", WIDGET_CHECKBOX)
        .Options(CheckboxOptions().Tooltip(
            "This allows you to advance through the game one frame at a time on command. "
            "To advance a frame, hold Z and tap R on the second controller. Holding Z "
            "and R will advance a frame every half second. You can also use the buttons below."))
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_NULL_PLAY_STATE).active ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active;
            if (gPlayState != nullptr) {
                info.valuePointer = (bool*)&gPlayState->frameAdvCtx.enabled;
            } else {
                info.valuePointer = (bool*)nullptr;
            }
        });
    AddWidget(path, "Advance 1", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Advance 1 frame.").Size(Sizes::Inline))
        .Callback([](WidgetInfo& info) { CVarSetInteger("gDeveloperTools.FrameAdvanceTick", 1); })
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FRAME_ADVANCE_OFF).active ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active;
        });
    AddWidget(path, "Advance (Hold)", WIDGET_BUTTON)
        .Options(ButtonOptions().Tooltip("Advance frames while the button is held.").Size(Sizes::Inline))
        .PreFunc([](WidgetInfo& info) {
            info.isHidden = mBenMenu->disabledMap.at(DISABLE_FOR_FRAME_ADVANCE_OFF).active ||
                            mBenMenu->disabledMap.at(DISABLE_FOR_DEBUG_MODE_OFF).active;
        })
        .PostFunc([](WidgetInfo& info) {
            if (ImGui::IsItemActive()) {
                CVarSetInteger("gDeveloperTools.FrameAdvanceTick", 1);
            }
        })
        .SameLine(true);

    // dev tools windows
    path = { "Dev Tools", "Collision Viewer", 1 };
    AddSidebarEntry("Dev Tools", "Collision Viewer", 1);
    AddWidget(path, "Popout Collision Viewer", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.CollisionViewer")
        .Options(ButtonOptions().Tooltip("Makes collision visible on screen").Size(Sizes::Inline))
        .WindowName("Collision Viewer");

    path = { "Dev Tools", "Stats", 1 };
    AddSidebarEntry("Dev Tools", "Stats", 1);
    AddWidget(path, "Popout Stats", WIDGET_WINDOW_BUTTON)
        .CVar("gOpenWindows.Stats")
        .Options(ButtonOptions().Tooltip(
            "Shows the stats window, with your FPS and frametimes, and the OS you're playing on"))
        .WindowName("Stats");

    path = { "Dev Tools", "Console", 1 };
    AddSidebarEntry("Dev Tools", "Console", 1);
    AddWidget(path, "Popout Console", WIDGET_WINDOW_BUTTON)
        .CVar("gOpenWindows.Console")
        .Options(ButtonOptions().Tooltip(
            "Enables the console window, allowing you to input commands. Type help for some examples"))
        .WindowName("Console");

    path = { "Dev Tools", "Gfx Debugger", 1 };
    AddSidebarEntry("Dev Tools", "Gfx Debugger", 1);
    AddWidget(path, "Popout Gfx Debugger", WIDGET_WINDOW_BUTTON)
        .CVar("gOpenWindows.GfxDebugger")
        .Options(ButtonOptions().Tooltip(
            "Enables the Gfx Debugger window, allowing you to input commands, type help for some examples"))
        .WindowName("GfxDebuggerWindow");

    path = { "Dev Tools", "Save Editor", 1 };
    AddSidebarEntry("Dev Tools", "Save Editor", 1);
    AddWidget(path, "Popout Save Editor", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.SaveEditor")
        .Options(ButtonOptions().Tooltip("Enables the Save Editor window, allowing you to edit your save file"))
        .WindowName("Save Editor");

    path = { "Dev Tools", "Actor Viewer", 1 };
    AddSidebarEntry("Dev Tools", "Actor Viewer", 1);
    AddWidget(path, "Popout Actor Viewer", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.ActorViewer")
        .Options(ButtonOptions().Tooltip("Enables the Actor Viewer window, allowing you to view actors in the world."))
        .WindowName("Actor Viewer");

    path = { "Dev Tools", "Event Log", 1 };
    AddSidebarEntry("Dev Tools", "Event Log", 1);
    AddWidget(path, "Popout Event Log", WIDGET_WINDOW_BUTTON)
        .CVar("gWindows.EventLog")
        .Options(ButtonOptions().Tooltip("Enables the event log window"))
        .WindowName("Event Log");
}

BenMenu::BenMenu(const std::string& consoleVariable, const std::string& name)
    : Menu(consoleVariable, name, 0, UIWidgets::Colors::LightBlue) {
}

void BenMenu::InitElement() {
    Ship::Menu::InitElement();
    AddSettings();
    AddEnhancements();
    AddDevTools();
    // RegisterResolutionWidgets();
    Rando::RegisterMenu();

    menuEntries = { { "Settings", settingsSidebar, "gSettings.Menu.SettingsSidebarSection", settingsOrder },
                    { "Enhancements", enhancementsSidebar, "gSettings.Menu.EnhancementsSidebarSection",
                      enhancementsOrder },
                    { "Developer Tools", devToolsSidebar, "gSettings.Menu.DevToolsSidebarSection", devToolsOrder },
                    { "Rando", randoSidebar, "gSettings.Menu.RandoSidebarSection", randoOrder } };

    if (CVarGetInteger("gSettings.Menu.SidebarSearch", 0)) {
        InsertSidebarSearch();
    }

    disabledMap = {
        { DISABLE_FOR_CAMERAS_OFF,
          { [](disabledInfo& info) -> bool {
               return !CVarGetInteger("gEnhancements.Camera.DebugCam.Enable", 0) &&
                      !CVarGetInteger("gEnhancements.Camera.FreeLook.Enable", 0);
           },
            "Both Debug Camera and Free Look are Disabled" } },
        { DISABLE_FOR_DEBUG_CAM_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gEnhancements.Camera.DebugCam.Enable", 0); },
            "Debug Camera is Enabled" } },
        { DISABLE_FOR_DEBUG_CAM_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEnhancements.Camera.DebugCam.Enable", 0); },
            "Debug Camera is Disabled" } },
        { DISABLE_FOR_FREE_LOOK_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gEnhancements.Camera.FreeLook.Enable", 0); },
            "Free Look is Enabled" } },
        { DISABLE_FOR_FREE_LOOK_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEnhancements.Camera.FreeLook.Enable", 0); },
            "Free Look is Disabled" } },
        { DISABLE_FOR_GYRO_OFF,
          { [](disabledInfo& info) -> bool {
               return !CVarGetInteger("gEnhancements.Camera.FirstPerson.GyroEnabled", 0);
           },
            "Gyro Aiming is Disabled" } },
        { DISABLE_FOR_GYRO_ON,
          { [](disabledInfo& info) -> bool {
               return CVarGetInteger("gEnhancements.Camera.FirstPerson.GyroEnabled", 0);
           },
            "Gyro Aiming is Enabled" } },
        { DISABLE_FOR_RIGHT_STICK_OFF,
          { [](disabledInfo& info) -> bool {
               return !CVarGetInteger("gEnhancements.Camera.FirstPerson.RightStickEnabled", 0);
           },
            "Right Stick Aiming is Disabled" } },
        { DISABLE_FOR_AUTO_SAVE_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEnhancements.Saving.Autosave", 0); },
            "AutoSave is Disabled" } },
        { DISABLE_FOR_NULL_PLAY_STATE,
          { [](disabledInfo& info) -> bool { return gPlayState == NULL; }, "Save Not Loaded" } },
        { DISABLE_FOR_DEBUG_MODE_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gDeveloperTools.DebugEnabled", 0); },
            "Debug Mode is Disabled" } },
        { DISABLE_FOR_NO_VSYNC,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->CanDisableVerticalSync();
           },
            "Disabling VSync not supported" } },
        { DISABLE_FOR_NO_WINDOWED_FULLSCREEN,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->SupportsWindowedFullscreen();
           },
            "Windowed Fullscreen not supported" } },
        { DISABLE_FOR_NO_MULTI_VIEWPORT,
          { [](disabledInfo& info) -> bool {
               return !Ship::Context::GetInstance()->GetWindow()->GetGui()->SupportsViewports();
           },
            "Multi-viewports not supported" } },
        { DISABLE_FOR_NOT_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() !=
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Available Only on DirectX" } },
        { DISABLE_FOR_DIRECTX,
          { [](disabledInfo& info) -> bool {
               return Ship::Context::GetInstance()->GetWindow()->GetWindowBackend() ==
                      Ship::WindowBackend::FAST3D_DXGI_DX11;
           },
            "Not Available on DirectX" } },
        { DISABLE_FOR_MATCH_REFRESH_RATE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger("gMatchRefreshRate", 0); },
            "Match Refresh Rate is Enabled" } },
        { DISABLE_FOR_MOTION_BLUR_MODE,
          { [](disabledInfo& info) -> bool {
               info.value = CVarGetInteger("gEnhancements.Graphics.MotionBlur.Mode", 0);
               return !info.value;
           },
            "Motion Blur Mode mismatch" } },
        { DISABLE_FOR_MOTION_BLUR_OFF,
          { [](disabledInfo& info) -> bool { return !R_MOTION_BLUR_ENABLED; }, "Motion Blur is Disabled" } },
        { DISABLE_FOR_FRAME_ADVANCE_OFF,
          { [](disabledInfo& info) -> bool { return !(gPlayState != nullptr && gPlayState->frameAdvCtx.enabled); },
            "Frame Advance is Disabled" } },
        { DISABLE_FOR_WARP_POINT_NOT_SET,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger(WARP_POINT_CVAR "Saved", 0); },
            "Warp Point Not Saved" } },
        { DISABLE_FOR_INTRO_SKIP_OFF,
          { [](disabledInfo& info) -> bool { return !CVarGetInteger("gEnhancements.Cutscenes.SkipIntroSequence", 0); },
            "Intro Skip Not Selected" } },
        { DISABLE_FOR_ADVANCED_RESOLUTION_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".Enabled", 0); },
            "Advanced Resolution Enabled" } },
        { DISABLE_FOR_VERTICAL_RES_TOGGLE_ON,
          { [](disabledInfo& info) -> bool {
               return CVarGetInteger(CVAR_PREFIX_ADVANCED_RESOLUTION ".VerticalResolutionToggle", 0);
           },
            "Vertical Resolution Toggle Enabled" } },
        { DISABLE_FOR_LOW_RES_MODE_ON,
          { [](disabledInfo& info) -> bool { return CVarGetInteger(CVAR_LOW_RES_MODE, 0); }, "N64 Mode Enabled" } },
    };
}

void BenMenu::UpdateElement() {
    Ship::Menu::UpdateElement();
}

void BenMenu::Draw() {
    // if (CVarGetInteger("gSettings.Menu.SelectedHeader", 0) &&
    // CVarGetInteger("gSettings.Menu.EnhancementsSidebarSection", 0)) {
    //     UpdateResolutionVars();
    // }
    Ship::Menu::Draw();
}

void BenMenu::DrawElement() {
    Ship::Menu::DrawElement();
}
} // namespace BenGui
