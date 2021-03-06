#pragma once
#include <memory>
#include <vector>
#include <mui/ds_window.h>
#include <mui/controls.h>
#include <mui/layout.h>
#include "base_setup_window.h"
#include "modal_window.h"
#include <mui/controls/timer_ptr.h>
#include "../../HTTPRequestsPluginLib/Request.h"
#include "../../HTTPRequestsPluginLib/SelectedChannel.h"

class DewesoftBridge;

class SetupWindow : public BaseSetupWindow
{
public:
    explicit SetupWindow(Dewesoft::MUI::WindowPtr ui, DewesoftBridge& bridge);
    ~SetupWindow();

    void setupEnter();
    void setupLeave();

    void addChannelsToTriggerChannelCBox(Dewesoft::MUI::ComboBox& comboBox);

    void addItemsToChannelGrid();
    void addItemsToOptionsListBox(Dewesoft::MUI::WindowPtr ui, Dewesoft::MUI::StackPanel& stackPanel);

    void addEdgeCondToCBox(Dewesoft::MUI::ComboBox& comboBox);

private:
    Dewesoft::MUI::Timer uiRefreshTimer;
    DewesoftBridge& bridge;

    void onAddChannelClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);

    // Event handlers for settings changes
    void onTriggerLevelTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onTemplateFileTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onReportDirTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onReportNameTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args);
    void onTriggerChanChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args);
    void onEdgeTypeChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args);
    void onOptionsSelectionChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args);
    void onUiRefreshTimer(Dewesoft::MUI::Timer& ctrl, Dewesoft::MUI::EventArgs& args);
    void templateSelectClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
    void reportDirSelectClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args);
    void onGridGetProps(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellPropsArgs& args);
    void onCellInputEventHandler(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellInputArgs& args);
    void onCellActionEventHandler(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellActionEventArgs& args);
    void onGridComboItems(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridComboItemsArgs& args);

    Dewesoft::MUI::TextBox triggerLevelTextBox;
    Dewesoft::MUI::TextBox templateFileTextBox;
    Dewesoft::MUI::TextBox reportDirTextBox;
    Dewesoft::MUI::TextBox reportNameTextBox;

    Dewesoft::MUI::ComboBox triggerChanCBox;
    Dewesoft::MUI::ComboBox edgeTypeCBox;

    Dewesoft::MUI::Button templateBtn;
    Dewesoft::MUI::Button reportDirBtn;

    Dewesoft::MUI::Button addChannelBtn;

    Dewesoft::MUI::StackPanel optionsStackPanel;

    Dewesoft::MUI::WindowPtr uiPtr;

    Dewesoft::MUI::DSDrawGrid selectedChannelsGrid;

    bool acceptsInput;
    Dewesoft::MUI::DrawGridCellState cellState;
    std::string editText;
    std::string textItem;
};
