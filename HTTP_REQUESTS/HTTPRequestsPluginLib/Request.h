#pragma once
#define CURL_STATICLIB
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
#include "SelectedChannel.h"
#include "AdditionalOptions.h"
#include "TextualChannel.h"
#include <vector>
#include <algorithm>

class DewesoftBridge;

namespace HTTP_Requests
{

class Request
{
public:
    using InputManagerImpl = Dewesoft::Utils::Dcom::InputChannel::InputManagerImpl;
    using AcquiredDataInfo = Dewesoft::Utils::Dcom::InputChannel::AcquiredDataInfo;

    explicit Request(InputManagerImpl& inputManager, IAppPtr app);

    explicit Request(InputManagerImpl& inputManager,
                     IAppPtr app,
                     std::string triggerChannel,
                     double triggerLevel,
                     std::string edgeType,
                     std::string templateFile,
                     std::string reportDirectory,
                     std::string reportName);

    void getData(const AcquiredDataInfo& acquiredDataInfo, const _bstr_t& usedFile);
    int minBlockSize();
    static int getBlockSize(IChannelPtr channel, uint64_t lastPosChecked);
    static bool checkTrigger(const std::string& edgeType,
                             const double& triggerLevel,
                             const double& currentSample,
                             const double& nextSample);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void saveSettings(const Dewesoft::Utils::Serialization::NodePtr& node) const;

    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);
    void loadSettings(const Dewesoft::Utils::Serialization::NodePtr& node);

    void clear();

    static void curlThread(std::string data, std::string endpoint);
    static double getTriggerTimeThread(IChannelPtr channel,
                                       uint64_t* lastPosChecked,
                                       const double& triggerValue,
                                       const std::string& edgeType);
    static double getChannelValueAtTimeThread(IChannelPtr channel, uint64_t* lastPosChecked, const double& time);

    std::string triggerChannel;
    IChannelPtr triggerChannelPtr;
    double triggerLevel;
    std::string edgeType;
    std::string templateFile;
    std::string reportDirectory;
    std::string reportName;

    uint64_t lastPosCheckedTrigger;

    std::vector<AdditionalOptions> additionalOptionsList;
    std::vector<SelectedChannel> selectedChannelList;
    std::vector<std::string> specialChannelsList;

    InputManagerImpl& inputManager;
    IAppPtr app;

    std::string getDefaultRequestEndpoint();
    std::string requestEndpoint;

    bool useDefaultRequestEndpoint;

private:
    std::string defaultRequestEndpoint = "localhost/dewesoft/realtime_excel/";
};
}  // namespace HTTP_Requests
