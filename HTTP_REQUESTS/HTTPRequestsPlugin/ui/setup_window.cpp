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

SetupWindow::SetupWindow(WindowPtr ui, DewesoftBridge& bridge)
    : BaseSetupWindow(ui, bridge, "ui/setup_window.xml")
    , bridge(bridge)
{
    // Connect to UI componenets
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

    optionsListBox = CheckListBox::Connect(ui, "optionsListBox");

    // Fill CBoxes that only need one write;
    addDataEntryTypeToCBox(dataEntryTypeCBox);
    addEdgeCondToCBox(edgeTypeCBox);
    addChannelTypeToCBox(channelTypeCBox);

    // Fill options box
    addItemsToOptionsListBox(optionsListBox);

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
    optionsListBox.OnClickCheck += event(&SetupWindow::onOptionsSelectionChanged);
    channelTypeCBox.OnSelectedIndexChanged += event(&SetupWindow::onChannelTypeChanged);

    // Get list of relays that have been created and loop through relays to determine max uniqueID and set uniqueIDIndex to 1 greater than
    // current max std::vector<Webrelay> webrelayList = bridge.getRelayListForUI();
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

    //TODO
    //Add logic to load selected channels from vector for selected channel list

    // Get created relays and loop through and add to listbox
    // std::vector<Webrelay> webrelayList = bridge.getRelayListForUI();

    /*for (auto& webrelay : webrelayList)
    {
        std::string edgeType = (webrelay.getEdgeType() == RisingEdge) ? "Rising Edge" : "Falling Edge";

        listBox.addItem(std::string("Unique ID: ") + std::to_string(webrelay.getRelayID()) + ", IP Address: " + webrelay.getIPAddress() +
                            ", Relay #: " + std::to_string(webrelay.getRelayNum()) + ", Trigger Channel: " + webrelay.getTriggerChannel() +
                            ", Trigger Level: " + std::to_string(webrelay.getTriggerLevel()) + ", Edge Type: " + edgeType);
    }*/
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
        if (channelPtrs[x]->DataType != 9 && channelPtrs[x]->DataType != 10 && channelPtrs[x]->DataType != 11)
        {
            std::string channelName = channelPtrs[x]->GetName();
            comboBox.addItem(channelName);
        }
    }
}

void SetupWindow::addItemsToChannelListBox(Dewesoft::MUI::ListBox& listBox)
{
    listBox.clear();
}

void SetupWindow::addItemsToOptionsListBox(Dewesoft::MUI::CheckListBox& checkListBox)
{
    checkListBox.clear();

    for (auto& listItem : bridge.requestObj.additionalOptionsList)
    {
        checkListBox.addItem(listItem.optionName);
    }
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
   
    std::string  listBoxString = std::string("Data Entry Type : ") + dataEntryType + "    ,    Channel Type : " + channelType +
                                "    ,    Channel : " + selectedChannel + "    ,    Page# : " + std::to_string(pageNum) +
                               "    ,    Cell / Starting Cell : " + cellRef;

    channelListBox.addItem(listBoxString);

    bridge.requestObj.selectedChannelList.emplace_back(dataEntryType, channelType, selectedChannel, pageNum, cellRef);

}

void SetupWindow::onDeleteChannelClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
{
    std::string selectedItem = channelListBox.getSelectedItem().toStdString();

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

    //Create Selected Channel object from values vector and use to compare to channels in Request Selected Channel Vector
    //TODO

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
}
void SetupWindow::onEdgeTypeChanged(Dewesoft::MUI::ComboBox& comboBox, Dewesoft::MUI::EventArgs& args)
{
    bridge.requestObj.edgeType = comboBox.getSelectedItem();
}
void SetupWindow::onOptionsSelectionChanged(Dewesoft::MUI::CheckListBox& chkListBox, Dewesoft::MUI::EventArgs& args)
{
    //Loop through options list
    for (int x = 0; x < bridge.requestObj.additionalOptionsList.size(); x++)
    {
        //Loop through checkbox items
        for (int i = 0; i < chkListBox.getCount(); i++)
        {

            //Check if option name from class and check list are equal
            if (!chkListBox.getItemAt(i).toStdString().compare(bridge.requestObj.additionalOptionsList[x].optionName))
            {
                //Set enabled value to match check box
                if (chkListBox.isChecked(i))
                {
                    bridge.requestObj.additionalOptionsList[x].enabled = true;
                }
                else
                {
                    bridge.requestObj.additionalOptionsList[x].enabled = false;
                }
            }            
        }
    }
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
