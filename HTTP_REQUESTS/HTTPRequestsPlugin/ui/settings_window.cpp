#include "StdAfx.h"
#include "settings_window.h"
#include "dewesoft_bridge.h"

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

SettingsWindow::SettingsWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSettingsWindow(ui, bridge, "ui/settings_window.xml")
    , bridge(bridge)
{
    // Connect to UI Components
    endpointTextBox = TextBox::Connect(ui, "httpEndPointTxtBox");
    useDefaultEndpointCheckBox = CheckBox::Connect(ui, "useDefaultChkBox");

    endpointTextBox.OnTextChanged += event(&SettingsWindow::onEndpointTextChanged);
    useDefaultEndpointCheckBox.OnCheckedChanged += event(&SettingsWindow::onUseDefaultEndPointChanged);
}

void SettingsWindow::settingsEnter()
{
    
    if (bridge.requestObj.useDefaultRequestEndpoint)
    {
        this->endpointTextBox.setText(bridge.requestObj.getDefaultRequestEndpoint());
        this->useDefaultEndpointCheckBox.setIsChecked(bridge.requestObj.useDefaultRequestEndpoint);
        this->endpointTextBox.setIsEnabled(false);
    }
    else
    {

        this->useDefaultEndpointCheckBox.setIsChecked(bridge.requestObj.useDefaultRequestEndpoint);
        this->endpointTextBox.setText(bridge.requestObj.requestEndpoint);

    }

}

void SettingsWindow::onEndpointTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.requestEndpoint = txtBox.getText();
}

void SettingsWindow::onUseDefaultEndPointChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args)
{
    if (checkBox.getIsChecked())
    {
    
        bridge.requestObj.useDefaultRequestEndpoint = true;
        this->endpointTextBox.setText(bridge.requestObj.getDefaultRequestEndpoint());
        bridge.requestObj.requestEndpoint = bridge.requestObj.getDefaultRequestEndpoint();
        this->endpointTextBox.setIsEnabled(false);
    }

    else
    {
        bridge.requestObj.useDefaultRequestEndpoint = false;
        bridge.requestObj.requestEndpoint = this->endpointTextBox.getText();
        this->endpointTextBox.setIsEnabled(true);
    }
}
