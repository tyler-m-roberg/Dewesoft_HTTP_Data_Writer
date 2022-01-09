#pragma once
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <nlohmann/adl_serializer.hpp>
#include <dcomlib/dcom_output_channel/output_channel.h>
using json = nlohmann::json;

namespace HTTP_Requests
{
    class AdditionalOptions
    {
    public:
        std::string optionName;
        bool enabled;

        AdditionalOptions(std::string optionName, bool enabled);

        void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
        void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);
        json toJson();
    };

 }  // namespace HTTP_Requests
