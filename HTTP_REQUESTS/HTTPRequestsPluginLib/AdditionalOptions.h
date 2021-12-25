#pragma once
#include <dcomlib/dcom_output_channel/output_channel.h>
#include <dcomlib/dcom_output_channel/output_factory.h>

class AdditionalOptions
{
public:
    std::string optionName;
    bool enabled;

    AdditionalOptions(std::string optionName, bool enabled);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
};
