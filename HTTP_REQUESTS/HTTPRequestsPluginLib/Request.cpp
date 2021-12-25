#include "Request.h"
#include <thread>
#include <chrono>
#include <curl\curl.h>
#include <nlohmann/json.hpp>


using namespace Dewesoft::Utils::Serialization;

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
        curl_easy_setopt(curl, CURLOPT_URL, "https://webhook.site/7b6c7f77-2d4c-4d56-bc1c-ad9c4ae9bf66");
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

Request::Request()
    : Request("", 1.0, "Rising", "", "", "")
{
    
}

Request::Request(std::string triggerChannel,
                 double triggerLevel,
                 std::string edgeType,
                 std::string templateFile,
                 std::string reportDirectory,
                 std::string reportName)

    : triggerChannel(triggerChannel)
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
}

void Request::getData(
    const double& startTime, const double& sampleRate, const size_t& numSamples, const int64_t beginPos, const int64_t endPos)
{
    // nlohmann::json postData;

    // InputListPtr inputList = inputManager.getInputList();
    // postData["Input List Size"] = inputList->size();

    // postData["Begin Pos"] = beginPos;
    // postData["End Pos"] = endPos;

    // for (auto it = inputList->begin(); it != inputList->end(); ++it)
    //{
    //    postData[it->getName()] = it->getValueAtPos<float>(0, nullptr, true);
    //}

    // std::thread threadObj(curlThread, postData.dump());
    // threadObj.detach();
}

void Request::saveSetup(const NodePtr& node) const
{
    //Save main settings
     node->write(u8"TriggerChannel", triggerChannel);
     node->write(u8"TriggerLevel", triggerLevel);
     node->write(u8"EdgeType", edgeType);
     node->write(u8"TemplateFile", templateFile);
     node->write(u8"ReportDirectory", reportDirectory);
     node->write(u8"ReportName", reportName);


     //Create subnode for additional options and add child each option
     const NodePtr additionalOptionsNode = node->addChild(u8"AdditionalOptions");
     for (const AdditionalOptions& item : additionalOptionsList)
     {
         const NodePtr additionalOptionNode = additionalOptionsNode->addChild(u8"AdditionalOption");
         item.saveSetup(additionalOptionNode);
     }

     //Create subnode for selected channels and add each selected channel
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
     node->read(u8"ReportDirectory", reportDirectory,1);
     node->read(u8"ReportName", reportName, 1);

    const auto selectedChannelsNode = node->findChildNode(u8"SelectedChannels");
    if (!selectedChannelsNode)
    {

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
    }
    else
    {
        for (size_t i = 0; i < additionalOptionsNode->getChildCount(); ++i)
        {
            const auto additionalOptionNode = additionalOptionsNode->getChild(i);
            //Need code to check existing options and only load settings from options
            //that match option list
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

}

