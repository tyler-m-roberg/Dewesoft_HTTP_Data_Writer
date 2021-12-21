#include "StdAfx.h"
#include "setup_window.h"
#include "dewesoft_bridge.h"
#include <thread>
#include <chrono>
#include <regex>

using namespace Dewesoft::MUI;
using namespace Dewesoft::RT::Core;

/*
    todo
    Fix unique ids to get highest number from loaded relays;
*/

int SetupWindow::uniqueIDIndex = 1;

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

    deleteChannelBtn = Button::Connect(ui, "deleteChannelBtn");
    addChannelBtn = Button::Connect(ui, "addChannelBtn");

    channelListBox = ListBox::Connect(ui, "channelListBox");

    optionsListBox = CheckListBox::Connect(ui, "optionsListBox");


    //Fill CBoxes that only need one write;
    addDataEntryTypeToCBox(dataEntryTypeCBox);
    addEdgeCondToCBox(edgeTypeCBox);

    //Fill options box
    addItemsToOptionsListBox(optionsListBox);

    // Add submit event to submit button
    // submitBtn.OnClick += event(&SetupWindow::onSubmitClick);

    // Add delete event to delete button
    // deleteBtn.OnClick += event(&SetupWindow::onDeleteClick);

    // Get list of relays that have been created and loop through relays to determine max uniqueID and set uniqueIDIndex to 1 greater than
    // current max std::vector<Webrelay> webrelayList = bridge.getRelayListForUI();

    int maxUniqueID = 0;

    /////*for (auto& webrelay : webrelayList)
    ////{
    ////    maxUniqueID = (maxUniqueID <= webrelay.getRelayID()) ? webrelay.getRelayID() + 1 : maxUniqueID;
    ////}*/

    uniqueIDIndex = maxUniqueID;

}

SetupWindow::~SetupWindow()
{
}

void SetupWindow::setupEnter()
{
    //Fill comboboxes for channels
    addChannelsToTriggerChannelCBox(triggerChanCBox);
    addChannelsToChannelSelectionCBox(channelSelectionCBox);

    //Add saved items to list box
    addItemsToChannelListBox(channelListBox);
    

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
    checkListBox.addItem("Append Date To Report Filename");
    checkListBox.addItem("Open Excel On New Data");
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

// Return a unique ID for a new relay
int SetupWindow::getUniqueID()
{
    return SetupWindow::uniqueIDIndex++;
}

// Get information from UI and add to listbox and create a relay using bridge function
// void SetupWindow::onSubmitClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
//{
//    //// add code to create webrelay object
//    //int uniqueRelayID = getUniqueID();
//    //std::string ipAddress = ipAddressTxtBox.getText().toStdString();
//    //int relayNum = std::stoi(relayNumTxtBox.getText().toStdString());
//    //double triggerLevel = std::stod(triggerLevelTxtBox.getText().toStdString());
//    //std::string triggerChannel = triggerChannelCBox.getText().toStdString();
//    //EdgeTypes edgeType = (edgeTypeCBox.getText().toStdString().compare("Rising") == 0) ? RisingEdge : FallingEdge;
//    //std::string edgeTypeString = (edgeType == RisingEdge) ? "Rising Edge" : "Falling Edge";
//
//    //Webrelay* comparisonRelay = new Webrelay(nullptr, "", uniqueRelayID, triggerLevel, ipAddress, relayNum, edgeType, triggerChannel);
//
//    //std::vector<Webrelay> webrelayList = bridge.getRelayListForUI();
//
//    //for (auto& webrelay : webrelayList)
//    //{
//    //    if (webrelay == *comparisonRelay)
//    //    {
//    //        return;
//    //    }
//    //}
//
//    //std::string listBoxString = std::string("Unique ID: ") + std::to_string(uniqueRelayID) + ", IP Address: " + ipAddress +
//    //                            ", Relay #: " + std::to_string(relayNum) + ", Trigger Channel: " + triggerChannel +
//    //                            ", Edge Type: " + edgeTypeString + ", Trigger Level: " + std::to_string(triggerLevel);
//
//    //listBox.addItem(listBoxString);
//
//    //bridge.createWebRelay(std::string("Webrelay / ") + ipAddress + std::string(" / ") + std::to_string(relayNum), uniqueRelayID,
//    triggerLevel, ipAddress, relayNum, edgeType, triggerChannel);
//}

// void SetupWindow::onDeleteClick(Dewesoft::MUI::Button& btn, Dewesoft::MUI::EventArgs& args)
//{
//    //std::string selectedItem = listBox.getSelectedItem().toStdString();
//
//    //
//    //int indexOfFirstSpace = NULL;
//    //int indexOfFirstComma = NULL;
//
//    //for (int i = 0; i < selectedItem.length(); i++)
//    //{
//    //    if (selectedItem[i] == ' ')
//    //    {
//    //        indexOfFirstSpace = i;
//    //    }
//
//    //    if (selectedItem[i] == ',')
//    //    {
//    //        indexOfFirstComma = i;
//    //        break;
//    //    }
//    //}
//
//    //int substringLength = indexOfFirstComma - indexOfFirstSpace;
//
//    //std::string substring = selectedItem.substr(indexOfFirstSpace + 1, substringLength - 1);
//
//    ////finish code here
//
//    //int relayID = std::stoi(substring);
//
//    //bridge.deleteWebRelay(relayID);
//
//    //listBox.deleteSelected();
//
//}

/*
    need to add code to handle triggering, sending get request and updating output channel
*/
