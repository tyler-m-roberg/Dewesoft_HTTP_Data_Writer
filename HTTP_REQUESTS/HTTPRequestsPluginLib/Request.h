#pragma once
#define CURL_STATICLIB
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
#include "SelectedChannel.h"
#include "AdditionalOptions.h"
#include <vector>

struct IApp;

class Request
{
public:
    using InputManagerImpl = Dewesoft::Utils::Dcom::InputChannel::InputManagerImpl;

    explicit Request(InputManagerImpl& inputManager);

    explicit Request(InputManagerImpl& inputManager,
                     std::string triggerChannel,
                     double triggerLevel,
                     std::string edgeType,
                     std::string templateFile,
                     std::string reportDirectory,
                     std::string reportName);

    void getData(const double& startTime, const double& sampleRate, const size_t& numSamples, const int64_t beginPos, const int64_t endPos);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

    void clear();

    std::string triggerChannel;
    double triggerLevel;
    std::string edgeType;
    std::string templateFile;
    std::string reportDirectory;
    std::string reportName;

    std::vector<AdditionalOptions> additionalOptionsList;
    std::vector<SelectedChannel> selectedChannelList;
    std::vector<std::string> specialChannelsList;

    InputManagerImpl& inputManager;
};
