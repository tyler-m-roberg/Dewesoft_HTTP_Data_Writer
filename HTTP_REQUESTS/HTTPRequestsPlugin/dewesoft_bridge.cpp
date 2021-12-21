#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#define CURL_STATICLIB
#include "StdAfx.h"
#include "dewesoft_bridge.h"
#include <curl\curl.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Dcom::Utils;

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

DewesoftBridge::DewesoftBridge(InputManagerImpl& inputManager, OutputFactoryImpl& outputFactory, const IAppPtr app)
    : inputManager(inputManager)
    , outputFactory(outputFactory)
    , app(app)
    , pluginGroup(nullptr)
    , sineGenerator(outputFactory)
{
}

void DewesoftBridge::setPluginProps(const IAppPtr app, const _bstr_t pluginGuid, IPluginGroup* pluginGroup)
{
    this->app = app;
    this->pluginGuid = pluginGuid;
    this->pluginGroup = pluginGroup;
}

bool DewesoftBridge::checkSupportedDewesoftVersion(std::string& errorMessage) const
{
    long major, minor, revision;
    app->GetInterfaceVersion(&major, &minor, &revision);

    // If function returns false, this message will be displayed on Dewesoft startup.
    errorMessage = "Current Test plugin version is supported from DEWESoftX 2020.1 onwards.";

    return true;
}

void DewesoftBridge::onEnterHardwareSetup()
{
}

void DewesoftBridge::onUpdateSettings(NodePtr node)
{
}

void DewesoftBridge::onInitiateHardware()
{
}

void DewesoftBridge::onSetupEnter(const bool analysisMode)
{
    setupWindow->setupEnter();
}

void DewesoftBridge::onSetupLeave(const bool analysisMode)
{
    setupWindow->setupLeave();
}

void DewesoftBridge::onClearSetup()
{
}

void DewesoftBridge::onNewSetup()
{
}

void DewesoftBridge::onLoadSetup(NodePtr node, bool dataFile)
{
}

void DewesoftBridge::onSaveSetup(NodePtr node, bool dataFile)
{
}

void DewesoftBridge::onPreInitiate()
{
}

void DewesoftBridge::onStartData()
{
}

void DewesoftBridge::onGetData(const AcquiredDataInfo& acquiredDataInfo)
{
    const double sampleRate = inputManager.getCurrentSampleRate();
    const double startTime = acquiredDataInfo.beginPos / sampleRate;
    const size_t numSamples = acquiredDataInfo.endPos - acquiredDataInfo.beginPos;

    nlohmann::json postData;

    InputListPtr inputList = inputManager.getInputList();
    postData["Input List Size"] = inputList->size();

    postData["Begin Pos"] = acquiredDataInfo.beginPos;
    postData["End Pos"] = acquiredDataInfo.endPos;

    for (auto it = inputList->begin(); it != inputList->end(); ++it)
    {
        postData[it->getName()] = it->getValueAtPos<float>(0, nullptr, true);
    }

    std::thread threadObj(curlThread, postData.dump());
    threadObj.detach();
}

void DewesoftBridge::onStopData()
{
}

void DewesoftBridge::onStartStoring()
{
}

void DewesoftBridge::onStopStoring()
{
}

void DewesoftBridge::onPrepareAnalysis()
{
}

void DewesoftBridge::onStartAnalysis()
{
}

bool DewesoftBridge::isRecalculationRequired() const
{
    return false;
}

bool DewesoftBridge::showSettings(Dewesoft::MUI::IWindow* window)
{
    try
    {
        settingsWindow = std::make_unique<SettingsWindow>(window, *this);
    }
    catch (const std::exception& e)
    {
        app->WriteErrorMessage(e.what());
        return false;
    }
    return true;
}

void DewesoftBridge::hideSettings()
{
    settingsWindow.reset(nullptr);
}

bool DewesoftBridge::showSetup(Dewesoft::MUI::IWindow* window)
{
    try
    {
        setupWindow = std::make_unique<SetupWindow>(window, *this);
    }
    catch (const std::exception& e)
    {
        std::string msg = e.what();
        app->WriteErrorMessage(e.what());
        return false;
    }
    return true;
}

void DewesoftBridge::hideSetup()
{
    setupWindow.reset(nullptr);
}

std::vector<IChannelPtr> DewesoftBridge::getUsedChannelsForUI()
{
    app->Data->BuildChannelList();
    IChannelListPtr channelListPtr = app->Data->GetUsedChannels();

    std::vector<IChannelPtr> channelPtrs;

    for (int x = 0; x < channelListPtr->Count; x++)
    {
        channelPtrs.emplace_back(channelListPtr->GetItem(x));
    }

    return channelPtrs;

}

std::vector<IGHObjectPtr> DewesoftBridge::getHeaderChannelsForUI()
{
    
    IGlobalHeaderPtr headerListPtr = app->GlobalHeader;

    std::vector<IGHObjectPtr> headerPtrs;

    for (int x = 0; x < headerListPtr->Count; x++)
    {

        headerPtrs.emplace_back(headerListPtr->GetItem(x));
    }

    return headerPtrs;
}
