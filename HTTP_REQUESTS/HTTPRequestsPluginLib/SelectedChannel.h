#pragma once
#include "dcomlib/dcom_utils/dewesoft_dcom_node.h"

class SelectedChannel
{
public:
    explicit SelectedChannel();

    explicit SelectedChannel(std::string dataEntryType, std::string channelType, std::string channelName, int pageNum, std::string cellRef);

    void saveSetup(const NodePtr& node) const;


    bool operator==(const SelectedChannel& requestObj) const;

    bool operator!=(const SelectedChannel& requestObj) const;

    std::string dataEntryType;
    std::string channelName;
    int pageNum;
    std::string cellRef;
    std::string channelType;
};
