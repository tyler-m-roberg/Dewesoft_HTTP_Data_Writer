#include "AdditionalOptions.h"

using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Serialization;

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
