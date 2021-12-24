#pragma once
#define CURL_STATICLIB
#include "dcomlib/dcom_utils/dewesoft_dcom_node.h"
#include "SelectedChannel.h"

struct AdditionalOptions
{

    std::string optionName;
    bool enabled;

    AdditionalOptions(std::string optionName, bool enabled)
        : optionName(optionName)
        , enabled(enabled)
    {
    }


};

class Request
{
public:
    explicit Request();

    explicit Request(std::string triggerChannel,
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

};

