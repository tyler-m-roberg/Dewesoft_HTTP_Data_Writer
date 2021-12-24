#include "SelectedChannel.h"

using namespace Dewesoft::Utils::Serialization;
using namespace Dewesoft::Utils::Dcom::Utils;

SelectedChannel::SelectedChannel()
{
}

SelectedChannel::SelectedChannel(std::string dataEntryType,
                                 std::string channelType,
                                 std::string channelName,
                                 int pageNum,
                                 std::string cellRef)

    : dataEntryType(dataEntryType)
    , channelType(channelType)
    , channelName(channelName)
    , pageNum(pageNum)
    , cellRef(cellRef)
    

{
}

void SelectedChannel::saveSetup(const NodePtr& node) const
{
    node->write(u8"DataEntryType", dataEntryType);
    node->write(u8"ChannelName", channelName);
    node->write(u8"PageNum", pageNum);
    node->write(u8"CellRef", cellRef);
    node->write(u8"ChannelType", channelType);
}

bool SelectedChannel::operator==(const SelectedChannel& selectedChannel) const
{
    return (!this->dataEntryType.compare(selectedChannel.dataEntryType) && !this->channelType.compare(selectedChannel.channelType) &&
            !this->channelName.compare(selectedChannel.channelName) && this->pageNum == selectedChannel.pageNum &&
            !this->cellRef.compare(selectedChannel.cellRef));
}

bool SelectedChannel::operator!=(const SelectedChannel& selectedChannel) const
{
    return !(*this == selectedChannel);
}
