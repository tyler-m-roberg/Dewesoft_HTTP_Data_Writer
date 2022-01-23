#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "Request.h"
#include <codecvt>
#include <thread>
#include <chrono>
#include <curl\curl.h>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>
#include <limits>
#include <functional>
#include <algorithm>

using namespace Dewesoft::Utils::Serialization;
using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Dcom::Utils;
using namespace HTTP_Requests;

void curlThread(std::string data, std::string endpoint)
{
    CURL* curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        struct curl_slist* hs = NULL;
        hs = curl_slist_append(hs, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

Request::Request(InputManagerImpl& inputManager, IAppPtr app)
    : Request(inputManager, app, "", 1.0, "Rising", "", "", "")
{
}

Request::Request(InputManagerImpl& inputManager,
                 IAppPtr app,
                 std::string triggerChannel,
                 double triggerLevel,
                 std::string edgeType,
                 std::string templateFile,
                 std::string reportDirectory,
                 std::string reportName)

    : inputManager(std::move(inputManager))
    , app(std::move(app))
    , triggerChannel(triggerChannel)
    , triggerLevel(triggerLevel)
    , edgeType(edgeType)
    , templateFile(templateFile)
    , reportDirectory(reportDirectory)
    , reportName(reportName)
{

    //Add additional options to object
    additionalOptionsList.emplace_back("Append Date To Report Filename", false);
    additionalOptionsList.emplace_back("Open Excel On New Data", false);
    additionalOptionsList.emplace_back("Force Close Excel On New Data", false);
    additionalOptionsList.emplace_back("Create New File On Write Error", false);
    additionalOptionsList.emplace_back("Use Relative Report Directories", false);

    //Define special channels
    specialChannelsList.emplace_back("Filename");
    specialChannelsList.emplace_back("Date");

}

void Request::getData(const AcquiredDataInfo& acquiredDataInfo, const _bstr_t& usedDataFile)
{
    // Verify channel pointers are set
    if (triggerChannelPtr == nullptr)
        return;

    //Verify channel pointers are set and return if not
    
    for (auto& selectedChannel : selectedChannelList)
    {
        if (selectedChannel.channelPtr == nullptr && !selectedChannel.channelType.compare("Standard Channel"))
            return;
    }

    nlohmann::json postData;  // JSON Data object for post request

    // Convert setting strings to JSON entries
    postData["Template_File_Opt"] = this->templateFile;
    postData["Report_Directory_Opt"] = this->reportDirectory;
    postData["Report_Name_Opt"] = this->reportName;

    nlohmann::json additionalOptionsJSON;  // JSON Data object to hold additional options

    // Loop through additional options and add each to additionalOptions JSON object
    for (auto& option : additionalOptionsList)
    {
        additionalOptionsJSON.push_back(option.toJson());
    }

    postData["Options_Opt"] = additionalOptionsJSON;  // Add additional options JSON object to post data object

    //Get min blocksize to read
    int minBlockSizeValue = minBlockSize();

    //Loop through samples up to the minimum block size to not read outside of channel memory of smaller channels
    for (int i = 0; i < minBlockSizeValue - 1; i++)
    {
        //Get current and next sample of trigger channel for trigger logic. Logic allows loop back of sample buffer
        float currentSampleTriggerChannel = triggerChannelPtr->DBValues[lastPosChecked % triggerChannelPtr->DBBufSize];
        float nextSampleTriggerChannel = triggerChannelPtr->DBValues[(lastPosChecked + 1) % triggerChannelPtr->DBBufSize];

        //Check trigger based on edge type and samples
        if (checkTrigger(edgeType, currentSampleTriggerChannel, nextSampleTriggerChannel))
        {
            nlohmann::json selectedChannelsJSON; //Create JSON object to hold channel informaiton

            //Loop through selected channel list to build JSON object
            for (auto& selectedChannel : selectedChannelList)
            {
                // Run if channel is standard channel
                if (!selectedChannel.channelType.compare("Standard Channel"))
                {
                    selectedChannel.dataType = selectedChannel.channelPtr->DataType;

                    // If selected type is text update text value
                    if (selectedChannel.dataType == 11)
                        selectedChannel.text = selectedChannel.channelPtr->Text;

                    // If is single value use single value accessor
                    else if (selectedChannel.channelPtr->IsSingleValue)
                        selectedChannel.channelValue = selectedChannel.channelPtr->SingleValue;

                    // Channel is async use get value at abs position to read in seek nearest async value
                    else if (selectedChannel.channelPtr->Async)
                    {
                        long* seekPos = new long;
                        selectedChannel.channelValue =
                            selectedChannel.channelPtr->GetValueAtAbsPosDouble((long) lastPosChecked, seekPos, false);

                        delete seekPos;
                    }

                    // If value is normal numeric channel get numeric value of channel
                    else
                        selectedChannel.channelValue =
                            selectedChannel.channelPtr->DBValues[(lastPosChecked + 1) % selectedChannel.channelPtr->DBBufSize];
                }

                // Handle speical channel cases
                else
                {
                    // Handle special channel used file name
                    if (!selectedChannel.channelName.compare("Filename"))
                    {
                        // Use codevect to convert file name to std string from _bstr_t
                        // Warning codevect is depricated, no suitable alternatives replace when available
                        std::string filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(usedDataFile);

                        // Set text to file name and data type to text
                        selectedChannel.text = filename;
                        selectedChannel.dataType = 11;
                    }
                }

                // Add channel to JSON object
                selectedChannelsJSON.push_back(selectedChannel.toJson());
            }

            postData["SelectedChannels"] = selectedChannelsJSON; //Add selected channels to main request JSON Object

            std::thread threadObj(curlThread, postData.dump(), requestEndpoint);  // Create new thread of curlThread with JSON postData as string
            threadObj.detach();                                  // Detatch thread to allow unblocking execution
        }

        //Increment position in data buffer to check for data
        lastPosChecked++;
    }
    
}

void Request::saveSetup(const NodePtr& node) const
{
    // Save main settings
    node->write(u8"TriggerChannel", triggerChannel);
    node->write(u8"TriggerLevel", triggerLevel);
    node->write(u8"EdgeType", edgeType);
    node->write(u8"TemplateFile", templateFile);
    node->write(u8"ReportDirectory", reportDirectory);
    node->write(u8"ReportName", reportName);

    // Create subnode for additional options and add child each option
    const NodePtr additionalOptionsNode = node->addChild(u8"AdditionalOptions");
    for (const AdditionalOptions& item : additionalOptionsList)
    {
        const NodePtr additionalOptionNode = additionalOptionsNode->addChild(u8"AdditionalOption");
        item.saveSetup(additionalOptionNode);
    }

    // Create subnode for selected channels and add each selected channel
    const auto selectedChannelsNode = node->addChild(u8"SelectedChannels");
    for (auto& selectedChannel : selectedChannelList)
    {
        const auto selectedChannelNode = selectedChannelsNode->addChild(u8"SelectedChannel");
        selectedChannel.saveSetup(selectedChannelNode);
    }
}

void Request::saveSettings(const Dewesoft::Utils::Serialization::NodePtr& node) const
{
    node->write(u8"RequestEndpoint", requestEndpoint);
    node->write(u8"UseDefaultRequestEndpoint", useDefaultRequestEndpoint);
}

void Request::loadSetup(const NodePtr& node)
{
    node->read(u8"TriggerChannel", triggerChannel, 1);
    node->read(u8"TriggerLevel", triggerLevel, 1);
    node->read(u8"EdgeType", edgeType, 1);
    node->read(u8"TemplateFile", templateFile, 1);
    node->read(u8"ReportDirectory", reportDirectory, 1);
    node->read(u8"ReportName", reportName, 1);

    const auto selectedChannelsNode = node->findChildNode(u8"SelectedChannels");
    if (!selectedChannelsNode)
    {
        // Do nothing
    }
    else
    {
        for (size_t i = 0; i < selectedChannelsNode->getChildCount(); ++i)
        {
            const auto selectedChannelNode = selectedChannelsNode->getChild(i);
            selectedChannelList.emplace_back();
            selectedChannelList.back().loadSetup(selectedChannelNode);
        }
    }

    const auto additionalOptionsNode = node->findChildNode(u8"AdditionalOptions");
    if (!additionalOptionsNode)
    {
        // Do Nothing
    }
    else
    {
        for (size_t i = 0; i < additionalOptionsNode->getChildCount(); ++i)
        {
            const auto additionalOptionNode = additionalOptionsNode->getChild(i);

            // Need code to check existing options and only load settings from options
            // that match option listbox
            std::string optionName;
            bool enabled;

            additionalOptionNode->read(u8"OptionName", optionName, "");
            additionalOptionNode->read(u8"Enabled", enabled, false);

            for (auto& additionalOptionsListItem : additionalOptionsList)
            {
                if (!additionalOptionsListItem.optionName.compare(optionName))
                {
                    if (enabled)
                    {
                        additionalOptionsListItem.enabled = true;
                    }
                    else
                    {
                        additionalOptionsListItem.enabled = false;
                    }
                }
            }
        }
    }
}

void Request::loadSettings(const Dewesoft::Utils::Serialization::NodePtr& node)
{
    node->read(u8"RequestEndpoint", requestEndpoint, defaultRequestEndpoint);
    node->read(u8"UseDefaultRequestEndpoint", useDefaultRequestEndpoint, 1);

    if (useDefaultRequestEndpoint)
        requestEndpoint = defaultRequestEndpoint;
}

void Request::clear()
{
    triggerChannel == "";
    triggerLevel == 0.0;
    edgeType = "Rising";
    templateFile = "";
    reportDirectory = "";
    reportName = "";

    for (auto& option : additionalOptionsList)
    {
        option.enabled = false;
    }
}


bool Request::checkTrigger(std::string edgeType, float currentSample, float nextSample)
{
    if (!edgeType.compare("Rising"))
    {
        return currentSample <= triggerLevel && nextSample >= triggerLevel;
    }
    else
    {
        return currentSample >= triggerLevel && nextSample <= triggerLevel;
    }
}

int Request::minBlockSize()
{
    int minBlockSizeRtn = (std::numeric_limits<int>::max)();

    minBlockSizeRtn = getBlockSize(triggerChannelPtr);
    for (auto& selectedChannel : selectedChannelList)
    {
        if (!selectedChannel.channelType.compare("Standard Channel"))
        {
            bool checkIfNotSingleValueChannel = !selectedChannel.channelPtr->IsSingleValue;
            bool checkIfNotAsync = !selectedChannel.channelPtr->GetAsync();

            if (checkIfNotSingleValueChannel && checkIfNotAsync)
                minBlockSizeRtn = (std::min)(minBlockSizeRtn, getBlockSize(selectedChannel.channelPtr));
        }
    }

    return minBlockSizeRtn;
}

int Request::getBlockSize(IChannelPtr channel)
{
    int blockSize = (channel->DBPos - (lastPosChecked % channel->DBBufSize) + channel->DBBufSize) % channel->DBBufSize;

    return blockSize;
}

std::string Request::getDefaultRequestEndpoint()
{
    return defaultRequestEndpoint;
}



