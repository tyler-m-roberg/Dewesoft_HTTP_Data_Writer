#include "StdAfx.h"
#include "setup_window.h"
#include "dewesoft_bridge.h"
#include <thread>
#include <chrono>
#include <regex>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <sstream>
constexpr int CHANNEL_GRID_COLUMN_WIDTH = 6;

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;
using namespace HTTP_Requests;

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
    , bridge(bridge)
{
    uiRefreshTimer = Timer::Create(ui);
    uiRefreshTimer.setInterval(100);

    // Connect to UI componenets*****************************************
    triggerLevelTextBox = TextBox::Connect(ui, "triggerLevelTextBox");
    templateFileTextBox = TextBox::Connect(ui, "templateFileTextBox");
    reportDirTextBox = TextBox::Connect(ui, "reportDirTextBox");
    reportNameTextBox = TextBox::Connect(ui, "reportNameTextBox");

    triggerChanCBox = ComboBox::Connect(ui, "triggerChanCBox");
    edgeTypeCBox = ComboBox::Connect(ui, "edgeTypeCBox");

    templateBtn = Button::Connect(ui, "templateBtn");
    reportDirBtn = Button::Connect(ui, "reportDirBtn");

    addChannelBtn = Button::Connect(ui, "addChannelBtn");

    optionsStackPanel = StackPanel::Connect(ui, "optionsListStackPanel");
    addEdgeCondToCBox(edgeTypeCBox);
    //********************************************************************

    // Add submit event to addChannelBtn
    addChannelBtn.OnClick += event(&SetupWindow::onAddChannelClick);

    // Add handlers for on updates for settings drop downs and text boxes
    triggerLevelTextBox.OnTextChanged += event(&SetupWindow::onTriggerLevelTextChanged);
    templateFileTextBox.OnTextChanged += event(&SetupWindow::onTemplateFileTextChanged);
    reportDirTextBox.OnTextChanged += event(&SetupWindow::onReportDirTextChanged);
    reportNameTextBox.OnTextChanged += event(&SetupWindow::onReportNameTextChanged);
    triggerChanCBox.OnSelectedIndexChanged += event(&SetupWindow::onTriggerChanChanged);
    edgeTypeCBox.OnSelectedIndexChanged += event(&SetupWindow::onEdgeTypeChanged);
    templateBtn.OnClick += event(&SetupWindow::templateSelectClick);
    reportDirBtn.OnClick += event(&SetupWindow::reportDirSelectClick);
    uiRefreshTimer.OnTimer += event(&SetupWindow::onUiRefreshTimer);

    uiPtr = ui;

    selectedChannelsGrid = DSDrawGrid::Connect(ui, "selectedChannelsGrid");
    selectedChannelsGrid.setGridSize(2, CHANNEL_GRID_COLUMN_WIDTH);

    /* Set default column types (can be overriden in OnCellGetProps event)
     *
     * Arguments are:
     *     zero based index,
     *     header name,
     *     type,
     *     default visibility,
     *     width,
     *     key/property
     */

    int columnWidths[] = {selectedChannelsGrid.getWidth() * 0.2,
                          selectedChannelsGrid.getWidth() * 0.2,
                          selectedChannelsGrid.getWidth() * 0.3,
                          selectedChannelsGrid.getWidth() * 0.1,
                          selectedChannelsGrid.getWidth() * 0.1,
                          selectedChannelsGrid.getWidth() * 0.1};

    selectedChannelsGrid.setColumn(0, "Data Entry Type", ctCombobox, RtTrue, columnWidths[0], "dataEntryTypeDSGrid");
    selectedChannelsGrid.setColumn(1, "Channel Type", ctCombobox, RtTrue, columnWidths[1], "channelTypeDSGrid");
    selectedChannelsGrid.setColumn(2, "Channel", ctCombobox, RtTrue, columnWidths[2], "channelDSGrid");
    selectedChannelsGrid.setColumn(3, "Page #", ctEditNumber, RtTrue, columnWidths[3], "pageNumDSGrid");
    selectedChannelsGrid.setColumn(4, "Cell / Starting Cell", ctEditText, RtTrue, columnWidths[4], "cellRefDSGrid");
    selectedChannelsGrid.setColumn(5, "Delete", ctButton, RtTrue, columnWidths[5], "deleteDSGrid");

    addChannelBtn.setWidth(selectedChannelsGrid.getWidth());

    //// Set default number fromat for "ctNum" (row number) to not have decimals
    selectedChannelsGrid.getColumn(3).setNumberFormat(cfInteger);

    //// we're done setting column info
    selectedChannelsGrid.applyColumns();

    selectedChannelsGrid.OnCellGetProps += event(&SetupWindow::onGridGetProps);
    selectedChannelsGrid.OnCellInput += event(&SetupWindow::onCellInputEventHandler);
    selectedChannelsGrid.OnCellAction += event(&SetupWindow::onCellActionEventHandler);
    selectedChannelsGrid.OnCellGetComboItems += event(&SetupWindow::onGridComboItems);
}

SetupWindow::~SetupWindow()
{
}

void SetupWindow::setupEnter()
{
    // Fill comboboxes for channels
    addChannelsToTriggerChannelCBox(triggerChanCBox);

    // Add saved items to list box
    addItemsToChannelGrid();

    addItemsToOptionsListBox(uiPtr, optionsStackPanel);

    templateFileTextBox.setText(bridge.requestObj.templateFile);
    reportDirTextBox.setText(bridge.requestObj.reportDirectory);
    reportNameTextBox.setText(bridge.requestObj.reportName);
    triggerChanCBox.setSelectedIndex(triggerChanCBox.getIndexOf(bridge.requestObj.triggerChannel));
    triggerLevelTextBox.setText(std::to_string(bridge.requestObj.triggerLevel));
    edgeTypeCBox.setSelectedIndex(edgeTypeCBox.getIndexOf(bridge.requestObj.edgeType));

    uiRefreshTimer.setEnabled(true);
}

void SetupWindow::setupLeave()
{
    uiRefreshTimer.setEnabled(false);
}

void SetupWindow::addChannelsToTriggerChannelCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.clear();

    std::vector<IChannelPtr> channelPtrs = bridge.getUsedChannelsForUI();

    for (int x = 0; x < channelPtrs.size(); x++)
    {
        if (channelPtrs[x]->DataType != 9 && channelPtrs[x]->DataType != 10 && channelPtrs[x]->DataType != 11 &&
            !channelPtrs[x]->GetIsSingleValue())
        {
            std::string channelName = channelPtrs[x]->GetName();
            comboBox.addItem(channelName);
        }
    }
}

void SetupWindow::addEdgeCondToCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.addItem("Rising");
    comboBox.addItem("Falling");
}

void SetupWindow::addItemsToChannelGrid()
{
    selectedChannelsGrid.setGridSize(bridge.requestObj.selectedChannelList.size() + 1, CHANNEL_GRID_COLUMN_WIDTH);

    //// we're done setting column info
    selectedChannelsGrid.applyColumns();

    if (selectedChannelsGrid.assigned())
        selectedChannelsGrid.invalidate();
}

void SetupWindow::addItemsToOptionsListBox(WindowPtr ui, Dewesoft::MUI::StackPanel& stackPanel)
{
    // Clear all check box controls from stack panel on open
    for (auto& control : stackPanel.getChildControls())
    {
        stackPanel.removeControl(control);
    }

    // Loop through options in request object from bridge and add check box for each option
    // Set check status based on status saved in request obejct
    for (auto& option : bridge.requestObj.additionalOptionsList)
    {
        // Create control and link to ui
        Dewesoft::MUI::CheckBox checkBox;
        checkBox = CheckBox::Create(ui);

        // Set checkbox options
        checkBox.setLabel(option.optionName);
        checkBox.setIsChecked(option.enabled);

        // Add to stack panel
        stackPanel.addControl(checkBox);

        // Add on selection change to checkbox
        checkBox.OnCheckedChanged += event(&SetupWindow::onOptionsSelectionChanged);
    }

    // Issue with stack panel where last control gets overlayed on top of first control.
    // The below code adds a hidden button of zero size to the stack panel last to
    // work around this issue;
    Dewesoft::MUI::Button blankLabel;
    blankLabel = Button::Create(ui);
    blankLabel.setHeight(0);
    blankLabel.setWidth(0);
    blankLabel.setVisibility(Dewesoft::MUI::Visibility::vHidden);
    stackPanel.addControl(blankLabel);
}

// Get information from UI and add to listbox and create a relay using bridge function
void SetupWindow::onAddChannelClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    std::string dataEntryType = "Single Value";
    std::string channelType = "Standard Channel";
    std::string selectedChannel = bridge.getUsedChannelsForUI()[0]->GetName();
    int pageNum = 1;
    std::string cellRef = "A1";

    bridge.requestObj.selectedChannelList.emplace_back(dataEntryType, channelType, selectedChannel, pageNum, cellRef);
    selectedChannelsGrid.setGridSize(bridge.requestObj.selectedChannelList.size() + 1, CHANNEL_GRID_COLUMN_WIDTH);
    selectedChannelsGrid.applyColumns();
    if (selectedChannelsGrid.assigned())
        selectedChannelsGrid.invalidate();
}

void SetupWindow::onTriggerLevelTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.triggerLevel = std::stod(triggerLevelTextBox.getText().toStdString());
}
void SetupWindow::onTemplateFileTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.templateFile = templateFileTextBox.getText().toStdString();
}
void SetupWindow::onReportDirTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.reportDirectory = reportDirTextBox.getText().toStdString();
}
void SetupWindow::onReportNameTextChanged(Dewesoft::MUI::TextBox& txtBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.reportName = reportNameTextBox.getText().toStdString();
}
void SetupWindow::onTriggerChanChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.triggerChannel = comboBox.getSelectedItem();
    bridge.requestObj.triggerChannelPtr = bridge.getIChannelPtrFromChannelName(comboBox.getSelectedItem());
}
void SetupWindow::onEdgeTypeChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.edgeType = comboBox.getSelectedItem();
}

void SetupWindow::onOptionsSelectionChanged(Dewesoft::MUI::CheckBox& checkBox, Dewesoft::MUI::EventArgs& args)
{
    for (auto& option : bridge.requestObj.additionalOptionsList)
    {
        if (!option.optionName.compare(checkBox.getLabel().toStdString()))
        {
            if (checkBox.getIsChecked())
            {
                option.enabled = true;
            }
            else
            {
                option.enabled = false;
            }
        }
    }
}

// Use dewesoft UI components to generate file selection dialog
void SetupWindow::templateSelectClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    // Create unique ptr to window for file selection dialog
    std::unique_ptr<OpenFileDialog> templateSelectWindow = std::make_unique<OpenFileDialog>(OpenFileDialog::Create(uiPtr));

    templateSelectWindow->setIsMultiselect(false);
    templateSelectWindow->showDialog();

    // Do nothing if a file was not selected
    if (!templateSelectWindow->getFileName().toStdString().compare(""))
    {
        return;
    }

    // Set template text box to file.
    templateFileTextBox.setText(templateSelectWindow->getFileName());
}

// Use windows api to generate folder selection dialog.
void SetupWindow::reportDirSelectClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    // Winows api object to hold returned file info
    BROWSEINFO bi = {0};

    bi.lpszTitle = _T("Select directory to store report..");

    // Create list to hold returned folder list
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

    // Execute if returned list is not empty
    if (pidl != NULL)
    {
        // TCHAR to hold returned file path
        TCHAR tszPath[MAX_PATH] = _T("\0");

        // If path exists in returned list set text box to path
        if (SHGetPathFromIDList(pidl, tszPath) == TRUE)
        {
            reportDirTextBox.setText(tszPath);
        }

        // — Free pidl memory
        CoTaskMemFree(pidl);
    }
}

void SetupWindow::onUiRefreshTimer(Timer& ctrl, Dewesoft::MUI::EventArgs& args)
{
    if (selectedChannelsGrid.assigned())
    {
        int columnWidths[] = {selectedChannelsGrid.getWidth() * 0.2,
                              selectedChannelsGrid.getWidth() * 0.2,
                              selectedChannelsGrid.getWidth() * 0.3,
                              selectedChannelsGrid.getWidth() * 0.1,
                              selectedChannelsGrid.getWidth() * 0.1,
                              selectedChannelsGrid.getWidth() * 0.1};

        selectedChannelsGrid.getColumn(0).setWidth(columnWidths[0]);
        selectedChannelsGrid.getColumn(1).setWidth(columnWidths[1]);
        selectedChannelsGrid.getColumn(2).setWidth(columnWidths[2]);
        selectedChannelsGrid.getColumn(3).setWidth(columnWidths[3]);
        selectedChannelsGrid.getColumn(4).setWidth(columnWidths[4]);
        selectedChannelsGrid.getColumn(5).setWidth(columnWidths[5]);

        addChannelBtn.setWidth(selectedChannelsGrid.getWidth());

        //// we're done setting column info
        selectedChannelsGrid.applyColumns();

        selectedChannelsGrid.invalidate();
    }
}

void SetupWindow::onGridGetProps(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellPropsArgs& args)
{
    int row = args.getRow();
    int col = args.getColumn();

    if (bridge.requestObj.selectedChannelList.size() > 0 && row != 0)
    {
        switch (col)
        {
            case 0:
                args.setText(bridge.requestObj.selectedChannelList[row - 1].getColItem(col));
                break;
            case 1:
                args.setText(bridge.requestObj.selectedChannelList[row - 1].getColItem(col));
                break;
            case 2:
                args.setText(bridge.requestObj.selectedChannelList[row - 1].getColItem(col));
                break;
            case 3:
                args.setNumber(std::stoi(bridge.requestObj.selectedChannelList[row - 1].getColItem(col)));
                break;
            case 4:
                args.setText(bridge.requestObj.selectedChannelList[row - 1].getColItem(col));
                break;
            case 5:
                args.setText("...");
                break;
            default:
                break;
        }
    }
}

void SetupWindow::onCellInputEventHandler(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellInputArgs& args)
{
    int row = args.getRow();
    int col = args.getColumn();

    if (row != 0)
    {
        switch (col)
        {
            case 0:
                bridge.requestObj.selectedChannelList[row - 1].setColItem(col, args.getText());
                break;
            case 1:

                bridge.requestObj.selectedChannelList[row - 1].setColItem(col, args.getText());

                if (!args.getText().toStdString().compare("Standard Channel"))
                {
                    if (bridge.getUsedChannelsForUI().size() > 0)
                    {
                        std::string channelName = bridge.getUsedChannelsForUI()[0]->GetName();
                        bridge.requestObj.selectedChannelList[row - 1].setColItem((col + 1), channelName);
                    }
                }
                else
                {
                    if (bridge.requestObj.specialChannelsList.size() > 0)
                        bridge.requestObj.selectedChannelList[row - 1].setColItem((col + 1), bridge.requestObj.specialChannelsList[0]);
                }

                if (grid.assigned())
                    grid.invalidate();

                break;
            case 2:
                bridge.requestObj.selectedChannelList[row - 1].setColItem(col, args.getText());
                break;
            case 3:
                bridge.requestObj.selectedChannelList[row - 1].setColItem(col, args.getText());
                break;
            case 4:
                bridge.requestObj.selectedChannelList[row - 1].setColItem(col, args.getText());
                break;
            default:
                break;
        }
    }
}

void SetupWindow::onCellActionEventHandler(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellActionEventArgs& args)
{
    int row = args.getRow();
    int col = args.getColumn();

    switch (col)
    {
        case 5:
            if (args.getButtonDownState() && row != 0)
            {
                bridge.requestObj.selectedChannelList.erase(bridge.requestObj.selectedChannelList.begin() + (row - 1));

                grid.setGridSize(bridge.requestObj.selectedChannelList.size() + 1, CHANNEL_GRID_COLUMN_WIDTH);
                selectedChannelsGrid.applyColumns();
                if (grid.assigned())
                    grid.invalidate();
            }
            break;
    }
}

void SetupWindow::onGridComboItems(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridComboItemsArgs& args)
{
    int col = args.getColumn();
    int row = args.getRow();

    int index;
    switch (col)
    {
        case 0:
            index = args.add("Single Value");
            index = args.add("Multi Value");
            break;
        case 1:
            index = args.add("Standard Channel");
            index = args.add("Special Channel");
            break;
        case 2:
            if (row != 0)
            {
                if (!grid.getCell(col - 1, row).toStdString().compare("Standard Channel"))
                {
                    std::vector<IChannelPtr> channelPtrs = bridge.getUsedChannelsForUI();

                    for (int x = 0; x < channelPtrs.size(); x++)
                    {
                        std::string channelName = channelPtrs[x]->GetName();

                        args.add(channelName);
                    }
                }
                else
                {
                    for (auto& specialChan : bridge.requestObj.specialChannelsList)
                    {
                        args.add(specialChan);
                    }
                }
            }

            break;
        default:
            break;
    }
}
