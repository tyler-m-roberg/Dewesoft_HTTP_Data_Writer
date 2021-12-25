#pragma once
#include <dcomlib/dcom_output_channel/output_channel.h>
#include <dcomlib/dcom_output_channel/output_factory.h>

class SelectedChannel
{
public:
    explicit SelectedChannel();

    explicit SelectedChannel(std::string dataEntryType, std::string channelType, std::string channelName, int pageNum, std::string cellRef);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;


    bool operator==(const SelectedChannel& requestObj) const;

    bool operator!=(const SelectedChannel& requestObj) const;

    std::string dataEntryType;
    std::string channelName;
    int pageNum;
    std::string cellRef;
    std::string channelType;
};
