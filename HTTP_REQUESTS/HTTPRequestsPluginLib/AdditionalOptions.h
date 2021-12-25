#pragma once
#include <string>
#include <dcomlib/dcom_output_channel/output_channel.h>

class AdditionalOptions
{
public:
    std::string optionName;
    bool enabled;

    AdditionalOptions(std::string optionName, bool enabled);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);
};
