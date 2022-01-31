#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <dcomlib/dcom_output_channel/output_channel.h>
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
#include <thread>
#include <future>

using json = nlohmann::json;

namespace HTTP_Requests
{
class SelectedChannel
{
public:
    explicit SelectedChannel();

    explicit SelectedChannel(std::string dataEntryType, std::string channelType, std::string channelName, int pageNum, std::string cellRef);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

    static std::string stringifyChannel(const SelectedChannel* channel);

    std::string getColItem(const int& colNum) const;
    void setColItem(const int& colNum, const std::string& val);

    bool operator==(const SelectedChannel& selectedChannel) const;
    bool operator==(const SelectedChannel* selectedChannel) const;
    bool operator!=(const SelectedChannel& requestObj) const;

    json toJson() const;

    std::string dataEntryType;
    std::string channelType;
    std::string channelName;
    int pageNum;
    std::string cellRef;
    long dataType;
    std::string text;
    double channelValue;
    IChannelPtr channelPtr;

    uint64_t lastPos;
    std::future<double> getChannelValueFuture;
};
}  // namespace HTTP_Requests
