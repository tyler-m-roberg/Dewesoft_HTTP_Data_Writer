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

class BaseSetupWindow : public Dewesoft::MUI::DSWindow
{
public:

protected:
    DewesoftBridge& bridge;

    Dewesoft::MUI::CaptionPanel generalSettingsPanel;
    Dewesoft::MUI::Label triggerChanLabel;
    Dewesoft::MUI::ComboBox triggerChanCBox;
    Dewesoft::MUI::Label triggerLevelLabel;
    Dewesoft::MUI::TextBox triggerLevelTextBox;
    Dewesoft::MUI::Label edgeTypeLabel;
    Dewesoft::MUI::ComboBox edgeTypeCBox;
    Dewesoft::MUI::Label templateFileLabel;
    Dewesoft::MUI::TextBox templateFileTextBox;
    Dewesoft::MUI::Label reportDirLabel;
    Dewesoft::MUI::TextBox reportDirTextBox;
    Dewesoft::MUI::Label reportNameLabel;
    Dewesoft::MUI::TextBox reportNameTextBox;
    Dewesoft::MUI::CaptionPanel optionsListPanel;
    Dewesoft::MUI::StackPanel optionsListStackPanel;
    Dewesoft::MUI::CaptionPanel addChannelPanel;
    Dewesoft::MUI::Label dataEntryTypeLabel;
    Dewesoft::MUI::ComboBox dataEntryTypeCBox;
    Dewesoft::MUI::ComboBox channelTypeCBox;
    Dewesoft::MUI::Label channelSelectionLabel;
    Dewesoft::MUI::ComboBox channelSelectionCBox;
    Dewesoft::MUI::Label pageNumLabel;
    Dewesoft::MUI::TextBox pageNumTextBox;
    Dewesoft::MUI::Label cellRefLabel;
    Dewesoft::MUI::TextBox cellRefTextBox;
    Dewesoft::MUI::Button addChannelBtn;
    Dewesoft::MUI::CaptionPanel currentChannelsPanel;
    Dewesoft::MUI::Button deleteChannelBtn;
    Dewesoft::MUI::ListBox channelListBox;
    Dewesoft::MUI::CaptionPanel dsGridPanel;
    Dewesoft::MUI::DSDrawGrid selectedChannelsGrid;

    BaseSetupWindow(Dewesoft::MUI::WindowPtr& ui, DewesoftBridge& bridge, std::string pathToXmlFile) : DSWindow (ui, pathToXmlFile), bridge(bridge)
    {
        // connect class to UI controls
        generalSettingsPanel = Dewesoft::MUI::CaptionPanel::Connect(ui, "generalSettingsPanel");
        triggerChanLabel = Dewesoft::MUI::Label::Connect(ui, "triggerChanLabel");
        triggerChanCBox = Dewesoft::MUI::ComboBox::Connect(ui, "triggerChanCBox");
        triggerLevelLabel = Dewesoft::MUI::Label::Connect(ui, "triggerLevelLabel");
        triggerLevelTextBox = Dewesoft::MUI::TextBox::Connect(ui, "triggerLevelTextBox");
        edgeTypeLabel = Dewesoft::MUI::Label::Connect(ui, "edgeTypeLabel");
        edgeTypeCBox = Dewesoft::MUI::ComboBox::Connect(ui, "edgeTypeCBox");
        templateFileLabel = Dewesoft::MUI::Label::Connect(ui, "templateFileLabel");
        templateFileTextBox = Dewesoft::MUI::TextBox::Connect(ui, "templateFileTextBox");
        reportDirLabel = Dewesoft::MUI::Label::Connect(ui, "reportDirLabel");
        reportDirTextBox = Dewesoft::MUI::TextBox::Connect(ui, "reportDirTextBox");
        reportNameLabel = Dewesoft::MUI::Label::Connect(ui, "reportNameLabel");
        reportNameTextBox = Dewesoft::MUI::TextBox::Connect(ui, "reportNameTextBox");
        optionsListPanel = Dewesoft::MUI::CaptionPanel::Connect(ui, "optionsListPanel");
        optionsListStackPanel = Dewesoft::MUI::StackPanel::Connect(ui, "optionsListStackPanel");
        addChannelPanel = Dewesoft::MUI::CaptionPanel::Connect(ui, "addChannelPanel");
        dataEntryTypeLabel = Dewesoft::MUI::Label::Connect(ui, "dataEntryTypeLabel");
        dataEntryTypeCBox = Dewesoft::MUI::ComboBox::Connect(ui, "dataEntryTypeCBox");
        channelTypeCBox = Dewesoft::MUI::ComboBox::Connect(ui, "channelTypeCBox");
        channelSelectionLabel = Dewesoft::MUI::Label::Connect(ui, "channelSelectionLabel");
        channelSelectionCBox = Dewesoft::MUI::ComboBox::Connect(ui, "channelSelectionCBox");
        pageNumLabel = Dewesoft::MUI::Label::Connect(ui, "pageNumLabel");
        pageNumTextBox = Dewesoft::MUI::TextBox::Connect(ui, "pageNumTextBox");
        cellRefLabel = Dewesoft::MUI::Label::Connect(ui, "cellRefLabel");
        cellRefTextBox = Dewesoft::MUI::TextBox::Connect(ui, "cellRefTextBox");
        addChannelBtn = Dewesoft::MUI::Button::Connect(ui, "addChannelBtn");
        currentChannelsPanel = Dewesoft::MUI::CaptionPanel::Connect(ui, "currentChannelsPanel");
        deleteChannelBtn = Dewesoft::MUI::Button::Connect(ui, "deleteChannelBtn");
        channelListBox = Dewesoft::MUI::ListBox::Connect(ui, "channelListBox");
        dsGridPanel = Dewesoft::MUI::CaptionPanel::Connect(ui, "dsGridPanel");
        selectedChannelsGrid = Dewesoft::MUI::DSDrawGrid::Connect(ui, "selectedChannelsGrid");
    }
};
