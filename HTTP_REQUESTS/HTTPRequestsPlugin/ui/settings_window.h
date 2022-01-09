#pragma once
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <mui/layout.h>
#include "base_settings_window.h"

class DewesoftBridge;

class SettingsWindow : public BaseSettingsWindow
{
public:
    SettingsWindow(Dewesoft::MUI::WindowPtr ui, DewesoftBridge& bridge);
    void settingsEnter();
    void settingsLeave();



private:
    DewesoftBridge& bridge;

};
