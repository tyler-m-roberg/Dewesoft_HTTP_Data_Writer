#pragma once
#include <dcomlib/dcom_input_channel/input_manager_impl.h>


class SelectedChannel
{
public:
    explicit SelectedChannel();

    explicit SelectedChannel(std::string dataEntryType, std::string channelName, int pageNum, std::string cellRef, std::string channelType);

    bool operator==(const SelectedChannel& requestObj) const;

    bool operator!=(const SelectedChannel& requestObj) const;

    std::string dataEntryType;
    std::string channelName;
    int pageNum;
    std::string cellRef;
    std::string channelType;

};
