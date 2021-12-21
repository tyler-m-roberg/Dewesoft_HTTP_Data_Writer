#pragma once
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <mui/layout.h>
#include "base_setup_window.h"
#include "modal_window.h"
#include <mui/controls/timer_ptr.h>

class DewesoftBridge;

class SetupWindow : public BaseSetupWindow
{
public:
    explicit SetupWindow(Dewesoft::MUI::WindowPtr ui, DewesoftBridge& bridge);
    ~SetupWindow();

    void setupEnter();
    void setupLeave();

    void addChannelsToTriggerChannelCBox(Dewesoft::MUI::ComboBox& comboBox);
    void addChannelsToChannelSelectionCBox(Dewesoft::MUI::ComboBox& comboBox);
    void addEdgeCondToCBox(Dewesoft::MUI::ComboBox &comboBox);

    void addItemsToChannelListBox(Dewesoft::MUI::ListBox &listBox);
    void addItemsToOptionsListBox(Dewesoft::MUI::CheckListBox &checkListBox);

    void addDataEntryTypeToCBox(Dewesoft::MUI::ComboBox& comboBox);

    static int uniqueIDIndex;

    static int getUniqueID();

private:
    DewesoftBridge& bridge;

    //void onSubmitClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
    //void onDeleteClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);

    Dewesoft::MUI::TextBox triggerLevelTextBox;
    Dewesoft::MUI::TextBox templateFileTextBox;
    Dewesoft::MUI::TextBox reportDirTextBox;
    Dewesoft::MUI::TextBox reportNameTextBox;
    Dewesoft::MUI::TextBox pageNumTextBox;
    Dewesoft::MUI::TextBox cellRefTextBox;

    Dewesoft::MUI::ComboBox triggerChanCBox;
    Dewesoft::MUI::ComboBox edgeTypeCBox;
    Dewesoft::MUI::ComboBox dataEntryTypeCBox;
    Dewesoft::MUI::ComboBox channelSelectionCBox;

    Dewesoft::MUI::Button deleteChannelBtn;
    Dewesoft::MUI::Button addChannelBtn;

    Dewesoft::MUI::ListBox channelListBox;

    Dewesoft::MUI::CheckListBox optionsListBox;
};
