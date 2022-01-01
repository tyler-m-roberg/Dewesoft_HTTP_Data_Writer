#pragma once
#include <string>

namespace HTTP_Requests
{
    class TextualChannel
    {
    public:
        TextualChannel();

        TextualChannel(std::string channelName, std::string channelText);

        std::string channelName;
        std::string channelText;
    };
}  // namespace HTTP_Requests
