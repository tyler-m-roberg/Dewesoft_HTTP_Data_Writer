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

using namespace Dewesoft::Utils::Serialization;
using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Dcom::Utils;
using namespace HTTP_Requests;

void curlThread(std::string data)
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
        curl_easy_setopt(curl, CURLOPT_URL, "https://webhook.site/842ede6d-8439-4a05-b2d3-ae3977b70692");
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
    additionalOptionsList.emplace_back("Append Date To Report Filename", false);
    additionalOptionsList.emplace_back("Open Excel On New Data", false);
    additionalOptionsList.emplace_back("Force Close Excel On New Data", false);
    additionalOptionsList.emplace_back("Create New File On Write Error", false);
    additionalOptionsList.emplace_back("Use Relative Report Directories", false);

    specialChannelsList.emplace_back("Filename");
    specialChannelsList.emplace_back("Date");

    if (!edgeType.compare("Rising"))
    {
        this->prevTriggerSample = (std::numeric_limits<double>::max)();
    }
    else
    {
        this->prevTriggerSample = (std::numeric_limits<double>::lowest)();
    }



}

void Request::getData(const AcquiredDataInfo& acquiredDataInfo)
{
     nlohmann::json postData; //JSON Data object for post request

     //Convert setting strings to JSON entries
     postData["Template_File_Opt"] = this->templateFile;
     postData["Report_Directory_Opt"] = this->reportDirectory;
     postData["Report_Name_Opt"] = this->reportName;

     nlohmann::json additionalOptionsJSON; //JSON Data object to hold additional options

     //Loop through additional options and add each to additionalOptions JSON object
     for (auto& option : additionalOptionsList)
     {
         additionalOptionsJSON.push_back(option.toJson());
     }

     postData["Options_Opt"] = additionalOptionsJSON; //Add additional options JSON object to post data object

    InputListPtr inputList = inputManager.getInputList();

    int64_t numSamples = acquiredDataInfo.endPos - acquiredDataInfo.beginPos;

    if (triggerChannelPtr != nullptr)
    {
        //Need to test db buff reading vs input channel reading
        InputChannel* inputChan = new InputChannelImpl(app, triggerChannelPtr);

        if (inputChan != nullptr)
        {
            for (int64_t x = acquiredDataInfo.beginPos; x <= acquiredDataInfo.endPos; x++)
            {
                float currentTriggerSample = inputChan->getValueAtPos<float>(x, nullptr, true);

                if (prevTriggerSample < triggerLevel && currentTriggerSample >= triggerLevel)
                {
                    InputChannel* dataChan = new InputChannelImpl(app, selectedChannelList[0].channelPtr);

                    postData["Data"] = dataChan->getValueAtPos<float>(x, nullptr);

                    std::thread threadObj(curlThread, postData.dump());  // Create new thread of curlThread with JSON postData as string
                    threadObj.detach(); // Detatch thread to allow unblocking execution
                }

                prevTriggerSample = currentTriggerSample;
            }
        }
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
    for (const auto& item : selectedChannelList)
    {
        const auto selectedChannelNode = selectedChannelsNode->addChild(u8"SelectedChannel");
        item.saveSetup(selectedChannelNode);
    }
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
        //Do nothing
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
        //Do Nothing
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

void Request::clear()
{
    triggerChannel == "";
    triggerLevel == 0.0;
    edgeType = "Rising";
    templateFile = "";
    reportDirectory = "";
    reportName = "";

    selectedChannelList.clear();

    for (auto& option : additionalOptionsList)
    {
        option.enabled = false;
    }
}

bool Request::checkTrigger(const AcquiredDataInfo& acquiredDataInfo)
{
    int64_t numSamples = acquiredDataInfo.endPos - acquiredDataInfo.beginPos;

    InputChannel* inputChan = new InputChannelImpl(app, triggerChannelPtr);

    for (int64_t x = 0; x < numSamples; x++)
    {
        float currentTriggerSample = inputChan->getScaledValueAtPos<float>(x, nullptr, true);

        prevTriggerSample = currentTriggerSample;
        
    }

    return true;
}

void Request::preData()
{
    app->Data->BuildChannelList();
    IChannelListPtr channelListPtr = app->Data->GetUsedChannels();
    for (auto& selectedChannel : selectedChannelList)
    {
        for (int x = 0; x < channelListPtr->Count; x++)
        {
            BSTR bstrChanName = channelListPtr->GetItem(x)->GetName().GetBSTR();
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            std::string chanNameCompare = converter.to_bytes(bstrChanName);

            if (!selectedChannel.channelName.compare(chanNameCompare))
            {
                selectedChannel.channelPtr = channelListPtr->GetItem(x);
            }
        }
    }
    
}
