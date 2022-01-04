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

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
    , bridge(bridge)
{
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
        if (channelPtrs[x]->DataType != 9 && channelPtrs[x]->DataType != 10 && channelPtrs[x]->DataType != 11 && (channelPtrs[x]->GetIsSingleValue() || channelPtrs[x]->Async))
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
    //selectedItem.erase(std::remove_if(selectedItem.begin(), selectedItem.end(), isspace), selectedItem.end());

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
