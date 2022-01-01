#include "TextualChannel.h"

using namespace HTTP_Requests;

TextualChannel::TextualChannel()
    : TextualChannel("","")
{

}

TextualChannel::TextualChannel(std::string channelName, std::string channelText)
    : channelName(channelName)
    , channelText(channelText)
{

}
