#include "StdAfx.h"
#include "setup_window.h"
#include "dewesoft_bridge.h"
#include <thread>
#include <chrono>
#include <regex>
#include <iostream>
#include <sstream>

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;
using namespace HTTP_Requests;

    ///* Set default column types (can be overriden in OnCellGetProps event)
// *
// * Arguments are:
// *     zero based index,
// *     header name,
// *     type,
// *     default visibility,
// *     width,
// *     key/property
// */
// channelGrid.setColumn(ctNum, "#", ctNumber, RtTrue, 40, "Num");
// channelGrid.setColumn(ctUsed, "Used", ctUdButton, RtTrue, 60, "Used");
// channelGrid.setColumn(ctStore, "Store", ctUdButton, RtFalse, 50, "Store");
// channelGrid.setColumn(Cols::ctColor, "Color", DrawGridCellType::ctColor, RtTrue, 30, "Color");
// channelGrid.setColumn(ctName, "Name", ctEditText, RtTrue, 140, "Name");
// channelGrid.setColumn(ctDesc, "Description", ctEditText, RtTrue, 140, "Desc");
// channelGrid.setColumn(ctTimebase, "Timebase", ctCombobox, RtTrue, 100, "Timebase");
// channelGrid.setColumn(ctMin, "Min", ctEditNumber, RtTrue, 80, "Min");
// channelGrid.setColumn(otLivePreview, "Values", ctLiveValue, RtTrue, 130, "Value");
// channelGrid.setColumn(ctMax, "Max", ctEditNumber, RtTrue, 80, "Max");
// channelGrid.setColumn(ctValue, "Value", ctNumber, RtTrue, 80, "Value");
// channelGrid.setColumn(ctStartVal, "Default value", ctEditNumber, RtTrue, 80, "InitVal");
// channelGrid.setColumn(ctResetVal, "Value reset", ctCombobox, RtTrue, 140, "ValueRes");
// channelGrid.setColumn(otUnit, "Unit", ctEditText, RtTrue, 40, "Unit");
// channelGrid.setColumn(ctReset, "Reset value", ctButton, RtTrue, 80, "ResetVal");

//// Set default number fromat for "ctNum" (row number) to not have decimals
// channelGrid.getColumn(ctNum).setNumberFormat(cfInteger);

//// we're done setting column info
// channelGrid.applyColumns();

// void SetupWindow::setupEnter()
//{
//    uiRefreshTimer.setEnabled(true);
//}
//
// void SetupWindow::setupLeave()
//{
//    uiRefreshTimer.setEnabled(false);
//}
//
// void SetupWindow::onUiRefreshTimer(Timer& ctrl, Dewesoft::MUI::EventArgs& args)
//{
//    if (channelGrid.assigned())
//        channelGrid.invalidate();
//}
//
// void SetupWindow::onTabChanged(TabControl& ctrl, Dewesoft::MUI::EventArgs& args)
//{
//
//}
//
// void SetupWindow::testGridComboItems(DSDrawGrid& grid, DrawGridComboItemsArgs& args)
//{
//    Int col = args.getColumn();
//
//    Int index;
//    switch (col)
//    {
//        case ctTimebase:
//            index = args.add("Async");
//            index = args.add("Single value");
//            break;
//        case ctResetVal:
//            index = args.add("On start measurement");
//            index = args.add("On load setup");
//            index = args.add("Always keep last value");
//            break;
//        default:
//            break;
//    }
//}
//
// void SetupWindow::testGridLiveValues(DSDrawGrid& grid, DrawGridLiveValueArgs& args)
//{
//    args.setLimitMaximum(500);
//    args.setLimitMinimum(0);
//
//    args.setCurrentMinimum(value);
//    args.setCurrentMaximum(value);
//
//    args.setOverloadMinimum(0);
//    args.setOverloadMaximum(500);
//    args.setShowOverloadWarning(true);
//    args.setUnit("Cpp");
//    args.setPrecision(3);
//}
//
// void SetupWindow::testGridGetProps(DSDrawGrid& grid, DrawGridCellPropsArgs& args)
//{
//    Int row = args.getRow();
//    Int col = args.getColumn();
//
//    switch (col)
//    {
//        case ctNum:
//            args.setNumber(static_cast<Float>(row));
//            break;
//        case ctUsed:
//            args.setText("Used");
//            args.setIsButtonDown(RtTrue);
//            break;
//        case ctStore:
//            args.setText("Store");
//            args.setIsButtonDown(RtTrue);
//            break;
//        case Cols::ctColor:
//            args.setCellColor(0xff0000);
//            break;
//        case ctName:
//            args.setText("Channel name C++");
//            break;
//        case ctDesc:
//            args.setText("Channel description C++");
//            break;
//        case ctTimebase:
//            args.setText("Async C++");
//            break;
//        case ctMin:
//            args.setNumber(0.0);
//            break;
//        case ctMax:
//            args.setNumber(100.0);
//            break;
//        case ctValue:
//            if (value > 600)
//            {
//                decrement = true;
//            }
//            else if (value < -100)
//            {
//                decrement = false;
//            }
//
//            if (decrement)
//            {
//                value--;
//            }
//            else
//            {
//                value++;
//            }
//
//            args.setNumber(value);
//            break;
//        case ctStartVal:
//            args.setNumber(0.0);
//            break;
//        case ctResetVal:
//        {
//            args.setText("On start measurement C++");
//            break;
//        }
//        case otUnit:
//            args.setText("");
//            break;
//        case ctReset:
//            args.setText("Reset");
//            break;
//        default:
//            args.setText("");
//            break;
//    }
//}

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
    pageNumTextBox = TextBox::Connect(ui, "pageNumTextBox");
    cellRefTextBox = TextBox::Connect(ui, "cellRefTextBox");

    triggerChanCBox = ComboBox::Connect(ui, "triggerChanCBox");
    edgeTypeCBox = ComboBox::Connect(ui, "edgeTypeCBox");
    dataEntryTypeCBox = ComboBox::Connect(ui, "dataEntryTypeCBox");
    channelSelectionCBox = ComboBox::Connect(ui, "channelSelectionCBox");
    channelTypeCBox = ComboBox::Connect(ui, "channelTypeCBox");

    deleteChannelBtn = Button::Connect(ui, "deleteChannelBtn");
    addChannelBtn = Button::Connect(ui, "addChannelBtn");

    channelListBox = ListBox::Connect(ui, "channelListBox");
    optionsStackPanel = StackPanel::Connect(ui, "optionsListStackPanel");
    //********************************************************************

    // Fill CBoxes that only need one write;
    addDataEntryTypeToCBox(dataEntryTypeCBox);
    addEdgeCondToCBox(edgeTypeCBox);
    addChannelTypeToCBox(channelTypeCBox);
    //*******************************************************************

    // Add submit event to addChannelBtn
    addChannelBtn.OnClick += event(&SetupWindow::onAddChannelClick);

    // Add delete event to delete button
    deleteChannelBtn.OnClick += event(&SetupWindow::onDeleteChannelClick);

    // Add handlers for on updates for settings drop downs and text boxes
    triggerLevelTextBox.OnTextChanged += event(&SetupWindow::onTriggerLevelTextChanged);
    templateFileTextBox.OnTextChanged += event(&SetupWindow::onTemplateFileTextChanged);
    reportDirTextBox.OnTextChanged += event(&SetupWindow::onReportDirTextChanged);
    reportNameTextBox.OnTextChanged += event(&SetupWindow::onReportNameTextChanged);
    triggerChanCBox.OnSelectedIndexChanged += event(&SetupWindow::onTriggerChanChanged);
    edgeTypeCBox.OnSelectedIndexChanged += event(&SetupWindow::onEdgeTypeChanged);
    channelTypeCBox.OnSelectedIndexChanged += event(&SetupWindow::onChannelTypeChanged);

    uiPtr = ui;

    selectedChannelsGrid = DSDrawGrid::Connect(ui, "selectedChannelsGrid");
    selectedChannelsGrid.setGridSize(2, 15);

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
    selectedChannelsGrid.setColumn(0, "Data Entry Type", ctCombobox, RtTrue, 200, "dataEntryTypeDSGrid");
    selectedChannelsGrid.setColumn(1, "Channel Type", ctCombobox, RtTrue, 200, "channelTypeDSGrid");
    selectedChannelsGrid.setColumn(2, "Channel", ctCombobox, RtTrue, 360, "channelDSGrid");
    selectedChannelsGrid.setColumn(3, "Page #", ctEditNumber, RtTrue, 100, "pageNumDSGrid");
    selectedChannelsGrid.setColumn(4, "Cell / Starting Cell", ctEditText, RtTrue, 100, "cellRefDSGrid");

    //// Set default number fromat for "ctNum" (row number) to not have decimals
    selectedChannelsGrid.getColumn(3).setNumberFormat(cfInteger);

    //// we're done setting column info
    selectedChannelsGrid.applyColumns();

    selectedChannelsGrid.OnCellInput += event(&SetupWindow::testGridCellInput);
}

SetupWindow::~SetupWindow()
{
}
void SetupWindow::setupEnter()
{
    // Fill comboboxes for channels
    addChannelsToTriggerChannelCBox(triggerChanCBox);
    addChannelsToChannelSelectionCBox(channelSelectionCBox);

    // Add saved items to list box
    addItemsToChannelListBox(channelListBox);

    
    addItemsToOptionsListBox(uiPtr, optionsStackPanel);

    templateFileTextBox.setText(bridge.requestObj.templateFile);
    reportDirTextBox.setText(bridge.requestObj.reportDirectory);
    reportNameTextBox.setText(bridge.requestObj.reportName);
    triggerChanCBox.setSelectedIndex(triggerChanCBox.getIndexOf(bridge.requestObj.triggerChannel));
    triggerLevelTextBox.setText(std::to_string(bridge.requestObj.triggerLevel));
    edgeTypeCBox.setSelectedIndex(edgeTypeCBox.getIndexOf(bridge.requestObj.edgeType));
}

void SetupWindow::setupLeave()
{
}

void SetupWindow::addChannelsToTriggerChannelCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.clear();

    std::vector<IChannelPtr> channelPtrs = bridge.getUsedChannelsForUI();

    for (int x = 0; x < channelPtrs.size(); x++)
    {
        if (channelPtrs[x]->DataType != 9 && channelPtrs[x]->DataType != 10 && channelPtrs[x]->DataType != 11 && !channelPtrs[x]->GetIsSingleValue())
        {
            std::string channelName = channelPtrs[x]->GetName();
            comboBox.addItem(channelName);
        }
    }
}

void SetupWindow::addItemsToChannelListBox(Dewesoft::MUI::ListBox& listBox)
{
    listBox.clear();

    for (auto& selectedChannel : bridge.requestObj.selectedChannelList)
    {
        listBox.addItem(SelectedChannel::stringifyChannel(&selectedChannel));
    }

}

void SetupWindow::addItemsToOptionsListBox(WindowPtr ui, Dewesoft::MUI::StackPanel& stackPanel)
{
    //Clear all check box controls from stack panel on open
    for (auto& control : stackPanel.getChildControls())
    {
        stackPanel.removeControl(control);
    }

    //Loop through options in request object from bridge and add check box for each option
    //Set check status based on status saved in request obejct
    for (auto& option : bridge.requestObj.additionalOptionsList)
    {
        //Create control and link to ui
        Dewesoft::MUI::CheckBox checkBox;
        checkBox = CheckBox::Create(ui);

        //Set checkbox options
        checkBox.setLabel(option.optionName);
        checkBox.setIsChecked(option.enabled);

        //Add to stack panel
        stackPanel.addControl(checkBox);

        //Add on selection change to checkbox
        checkBox.OnCheckedChanged += event(&SetupWindow::onOptionsSelectionChanged);
        
    }


    //Issue with stack panel where last control gets overlayed on top of first control.
    //The below code adds a hidden button of zero size to the stack panel last to
    // work around this issue;
    Dewesoft::MUI::Button blankLabel;
    blankLabel = Button::Create(ui);
    blankLabel.setHeight(0);
    blankLabel.setWidth(0);
    blankLabel.setVisibility(Dewesoft::MUI::Visibility::vHidden);
    stackPanel.addControl(blankLabel);
}

void SetupWindow::addChannelsToChannelSelectionCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.clear();

    std::vector<IChannelPtr> channelPtrs = bridge.getUsedChannelsForUI();

    for (int x = 0; x < channelPtrs.size(); x++)
    {
        std::string channelName = channelPtrs[x]->GetName();

        comboBox.addItem(channelName);
    }
}

void SetupWindow::addEdgeCondToCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.clear();
    comboBox.addItem("Rising");
    comboBox.addItem("Falling");
}

void SetupWindow::addDataEntryTypeToCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.addItem("Single Value");
    comboBox.addItem("Multi Value");
}

void SetupWindow::addChannelTypeToCBox(Dewesoft::MUI::ComboBox& comboBox)
{
    comboBox.addItem("Standard Channel");
    comboBox.addItem("Special Channel");
}


// Get information from UI and add to listbox and create a relay using bridge function
void SetupWindow::onAddChannelClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{

    std::string dataEntryType = dataEntryTypeCBox.getSelectedItem().toStdString();
    std::string channelType = channelTypeCBox.getSelectedItem().toStdString();
    std::string selectedChannel = channelSelectionCBox.getSelectedItem().toStdString();
    int pageNum = std::stoi(pageNumTextBox.getText().toStdString());
    std::string cellRef = cellRefTextBox.getText().toStdString();

    bridge.requestObj.selectedChannelList.emplace_back(dataEntryType, channelType, selectedChannel, pageNum, cellRef);
    channelListBox.addItem(SelectedChannel::stringifyChannel(&bridge.requestObj.selectedChannelList.back()));

}

void SetupWindow::onDeleteChannelClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    std::string selectedItem = channelListBox.getSelectedItem().toStdString();

    //**Removed due to issue with removing white space in channel names
    //Remove white space from string
    selectedItem.erase(std::remove_if(selectedItem.begin(), selectedItem.end(), isspace), selectedItem.end());

    //Use string stream to tokenize string off delimiter and then take substring delimited by colon and store in vector
    std::stringstream ss(selectedItem);
    std::string token;
    std::vector<std::string>* values = new std::vector<std::string>();
    while (std::getline(ss, token, ','))
    {
        values->emplace_back(token.substr(token.find(':') + 1, std::string::npos));
    }

    SelectedChannel* comparisonChannel = new SelectedChannel(values->at(0), values->at(1), values->at(2), std::stoi (values->at(3)), values->at(4));

    int index = 0;
    for (auto& channel : bridge.requestObj.selectedChannelList)
    {
        if (channel == comparisonChannel)
        {
            bridge.requestObj.selectedChannelList.erase(bridge.requestObj.selectedChannelList.begin() + index);
            break;
        }
        index++;
    }


    channelListBox.deleteSelected();

    delete values;
    delete comparisonChannel;

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


void SetupWindow::onChannelTypeChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args)
{
    channelSelectionCBox.clear();
    if (!comboBox.getSelectedItem().toStdString().compare("Special Channel"))
    {
        for (auto& specialChan : bridge.requestObj.specialChannelsList)
        {
            channelSelectionCBox.addItem(specialChan);
        }
    }
    else
    {
        addChannelsToChannelSelectionCBox(channelSelectionCBox);
    }
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

void SetupWindow::testGridCellInput(Dewesoft::MUI::DSDrawGrid& grid, Dewesoft::MUI::DrawGridCellInputArgs& args)
{
    Dewesoft::RT::Core::CharPtr* toString;
    args->toString(toString);
    
}
