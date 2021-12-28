#pragma once
#include <string>
#include <dcomlib/dcom_output_channel/output_channel.h>

class SelectedChannel
{
public:
    explicit SelectedChannel();

    explicit SelectedChannel(std::string dataEntryType, std::string channelType, std::string channelName, int pageNum, std::string cellRef);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

    static std::string stringifyChannel(SelectedChannel* channel);

    bool operator==(const SelectedChannel& selectedChannel) const;
    bool operator==(const SelectedChannel* selectedChannel) const;

    bool operator!=(const SelectedChannel& requestObj) const;

    std::string dataEntryType;
    std::string channelName;
    int pageNum;
    std::string cellRef;
    std::string channelType;
};
