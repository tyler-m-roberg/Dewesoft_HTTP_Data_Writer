#include "SelectedChannel.h"
#include "SelectedChannelProperties.h"

using namespace Dewesoft::Utils::Serialization;
using namespace HTTP_Requests;

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

void SelectedChannel::loadSetup(const NodePtr& node)
{
    node->read(u8"DataEntryType", dataEntryType, "");
    node->read(u8"DataEntryType", dataEntryType,"");
    node->read(u8"ChannelName", channelName,"");
    node->read(u8"PageNum", pageNum,1);
    node->read(u8"CellRef", cellRef,"");
    node->read(u8"ChannelType", channelType,"");

}

std::string SelectedChannel::stringifyChannel(const SelectedChannel* channel)
{
    return std::string("Data Entry Type:") + channel->dataEntryType + ",Channel Type:" + channel->channelType +
                                ",Channel:" + channel->channelName + ",Page #:" + std::to_string(channel->pageNum) +
                                ",Cell / Starting Cell:" + channel->cellRef;
}

bool SelectedChannel::operator==(const SelectedChannel& selectedChannel) const
{
    return (!this->dataEntryType.compare(selectedChannel.dataEntryType) && !this->channelType.compare(selectedChannel.channelType) &&
            !this->channelName.compare(selectedChannel.channelName) && this->pageNum == selectedChannel.pageNum &&
            !this->cellRef.compare(selectedChannel.cellRef));
}

bool SelectedChannel::operator==(const SelectedChannel* selectedChannel) const
{
    return (!this->dataEntryType.compare(selectedChannel->dataEntryType) && !this->channelType.compare(selectedChannel->channelType) &&
            !this->channelName.compare(selectedChannel->channelName) && this->pageNum == selectedChannel->pageNum &&
            !this->cellRef.compare(selectedChannel->cellRef));
}

bool SelectedChannel::operator!=(const SelectedChannel& selectedChannel) const
{
    return !(*this == selectedChannel);
}

json SelectedChannel::toJson(const SelectedChannelProperties& channelProps) const
{
    return json{

        {
            "DataEntryType", this->dataEntryType
        },
        {
            "ChannelName", this->channelName
        },
        {
            "PageNum", this->pageNum
        },
        {
            "CellRef", this->cellRef
        },
        {
            "ChannelType", this->channelType
        },
        {
            "DataType", channelProps.dataType
        },
        {
            "Text", channelProps.text
        },
        {
            "ChannelValue", channelProps.channelValue
        }

    };
}
