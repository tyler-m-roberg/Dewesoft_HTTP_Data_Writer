#include "AdditionalOptions.h"

using namespace Dewesoft::Utils::Serialization;
using namespace HTTP_Requests;

AdditionalOptions::AdditionalOptions(std::string optionName, bool enabled)
    : optionName(optionName)
    , enabled(enabled)
{
}

void AdditionalOptions::saveSetup(const NodePtr& node) const
{
    node->write(u8"OptionName", optionName);
    node->write(u8"Enabled", enabled);
}

void AdditionalOptions::loadSetup(const NodePtr& node)
{
}

json AdditionalOptions::toJson()
{
    return json
    {
        
        {
            "Option_Name", this->optionName
        },
        {
            "Enabled", this->enabled
        }
        
    };
}


