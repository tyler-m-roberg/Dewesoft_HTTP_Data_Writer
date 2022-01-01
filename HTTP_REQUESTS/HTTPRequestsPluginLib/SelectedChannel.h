#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <dcomlib/dcom_output_channel/output_channel.h>
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>

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

        static std::string stringifyChannel(SelectedChannel* channel);

        bool operator==(const SelectedChannel& selectedChannel) const;
        bool operator==(const SelectedChannel* selectedChannel) const;

        bool operator!=(const SelectedChannel& requestObj) const;

        json toJson();

        std::string dataEntryType;
        std::string channelName;
        int pageNum;
        std::string cellRef;
        std::string channelType;
        long dataType;
        std::string text;
        double channelValue;
        IChannelPtr channelPtr;
    };
}
