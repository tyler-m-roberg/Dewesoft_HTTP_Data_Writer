#include "StdAfx.h"
#include "settings_window.h"

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

SettingsWindow::SettingsWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSettingsWindow(ui, bridge, "ui/settings_window.xml")
    , bridge(bridge)
{

    //Connect to UI Components
    endpointTextBox = TextBox::Connect(ui, "httpEndPointTxtBox");
    useDefaultEndpointCheckBox = CheckBox::Connect(ui, "useDefaultChkBox");

    endpointTextBox.OnTextChanged += event(&SettingsWindow::onEndpointTextChanged);
    useDefaultEndpointCheckBox.OnCheckedChanged += event(&SettingsWindow::onUseDefaultEndPointChanged);
}

void SettingsWindow::onEndpointTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{

}

void SettingsWindow::onUseDefaultEndPointChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args)
{

}


