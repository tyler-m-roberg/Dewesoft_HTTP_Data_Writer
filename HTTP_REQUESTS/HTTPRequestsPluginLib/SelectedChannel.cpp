#include "SelectedChannel.h"

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
