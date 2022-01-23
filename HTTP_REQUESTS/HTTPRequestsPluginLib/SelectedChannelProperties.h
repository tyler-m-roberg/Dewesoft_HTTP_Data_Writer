#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <dcomlib/dcom_output_channel/output_channel.h>
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
class SelectedChannelProperties
{
public:

    SelectedChannelProperties();

    long dataType;
    std::string text;
    double channelValue;
    IChannelPtr channelPtr;
};
