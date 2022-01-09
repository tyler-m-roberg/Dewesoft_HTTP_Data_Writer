//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

#pragma once
#include "mui/mui.h"
#include "mui/controls.h"
#include <mui/ds_window.h>

class DewesoftBridge;

class BaseSettingsWindow : public Dewesoft::MUI::DSWindow
{
public:

protected:
    DewesoftBridge& bridge;

    Dewesoft::MUI::TextBox httpEndPointTxtBox;
    Dewesoft::MUI::CheckBox useDefaultChkBox;

    BaseSettingsWindow(Dewesoft::MUI::WindowPtr& ui, DewesoftBridge& bridge, std::string pathToXmlFile) : DSWindow (ui, pathToXmlFile), bridge(bridge)
    {
        // connect class to UI controls
        httpEndPointTxtBox = Dewesoft::MUI::TextBox::Connect(ui, "httpEndPointTxtBox");
        useDefaultChkBox = Dewesoft::MUI::CheckBox::Connect(ui, "useDefaultChkBox");
    }
};
