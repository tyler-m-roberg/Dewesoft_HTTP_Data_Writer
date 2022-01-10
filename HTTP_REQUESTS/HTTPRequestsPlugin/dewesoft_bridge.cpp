#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Ws2_32.lib")
#define CURL_STATICLIB
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "StdAfx.h"
#include "dewesoft_bridge.h"
#include <curl\curl.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>
#include <codecvt>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Dcom::Utils;
using namespace HTTP_Requests;

DewesoftBridge::DewesoftBridge(InputManagerImpl& inputManager, OutputFactoryImpl& outputFactory, const IAppPtr app)
    : inputManager(inputManager)
    , outputFactory(outputFactory)
    , app(app)
    , pluginGroup(nullptr)
    , sineGenerator(outputFactory)
    , requestObj(inputManager, app)
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
    if (node->isWrite())
    {
        requestObj.saveSettings(node);
    }
    else
    {
        requestObj.loadSettings(node);
    }
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
    requestObj.clear();
}

void DewesoftBridge::onNewSetup()
{
}

void DewesoftBridge::onLoadSetup(NodePtr node, bool dataFile)
{
    requestObj.loadSetup(node);
}

void DewesoftBridge::onSaveSetup(NodePtr node, bool dataFile)
{
    requestObj.saveSetup(node);
}

void DewesoftBridge::onPreInitiate()
{
    int i = 0;
}

void DewesoftBridge::onStartData()
{

}

void DewesoftBridge::onGetData(const AcquiredDataInfo& acquiredDataInfo)
{
    const double sampleRate = inputManager.getCurrentSampleRate();
    const double startTime = acquiredDataInfo.beginPos / sampleRate;
    const size_t numSamples = acquiredDataInfo.endPos - acquiredDataInfo.beginPos;

    _bstr_t dataFile = app->GetUsedDatafile();

    requestObj.getData(acquiredDataInfo, dataFile);

}

void DewesoftBridge::onStopData()
{
}

void DewesoftBridge::onStartStoring()
{
    requestObj.lastPosChecked = 0;
    for (auto& selectedChannel : requestObj.selectedChannelList)
    {
        selectedChannel.channelPtr = getIChannelPtrFromChannelName(selectedChannel.channelName);
    }

    requestObj.triggerChannelPtr = getIChannelPtrFromChannelName(requestObj.triggerChannel);
}

void DewesoftBridge::onStopStoring()
{
    for (auto& selectedChannel : requestObj.selectedChannelList)
    {
        selectedChannel.channelPtr = nullptr;
    }

    requestObj.triggerChannelPtr = nullptr;
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

std::string DewesoftBridge::getStringChannelValue(long index)
{
    return std::string(app->Data->GetUsedChannels()->GetItem(index)->Text);
}


IChannelPtr DewesoftBridge::getIChannelPtrFromChannelName(std::string chanName)
{
    return app->Data->FindChannel(chanName.c_str());
}
