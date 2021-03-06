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



private:
    DewesoftBridge& bridge;

    //Event handlers for settings change
    void onEndpointTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onUseDefaultEndPointChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args);

    Dewesoft::MUI::TextBox endpointTextBox;
    Dewesoft::MUI::CheckBox useDefaultEndpointCheckBox;

};
