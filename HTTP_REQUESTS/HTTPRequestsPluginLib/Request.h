#pragma once
#define CURL_STATICLIB
#include <commonlib/serialization/node.h>
#include <dcomlib/dcom_input_channel/input_manager_impl.h>
#include "SelectedChannel.h"
#include "AdditionalOptions.h"
#include "TextualChannel.h"
#include <vector>
#include <unordered_map>
#include <algorithm>

class DewesoftBridge;

namespace HTTP_Requests
{

    class Request
    {
    public:
        using InputManagerImpl = Dewesoft::Utils::Dcom::InputChannel::InputManagerImpl;
        using AcquiredDataInfo = Dewesoft::Utils::Dcom::InputChannel::AcquiredDataInfo;

        explicit Request(InputManagerImpl& inputManager, IAppPtr app);

        explicit Request(InputManagerImpl& inputManager,
                         IAppPtr app,
                         std::string triggerChannel,
                         double triggerLevel,
                         std::string edgeType,
                         std::string templateFile,
                         std::string reportDirectory,
                         std::string reportName);

        void getData(const AcquiredDataInfo& acquiredDataInfo,const _bstr_t& usedFile);
        int minBlockSize();
        int getBlockSize(IChannelPtr channel);
        bool checkTrigger(std::string edgeType, float currentSample, float nextSample);

        void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
        void saveSettings(const Dewesoft::Utils::Serialization::NodePtr& node) const;

        void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);
        void loadSettings(const Dewesoft::Utils::Serialization::NodePtr& node);

        void clear();

        std::string triggerChannel;
        IChannelPtr triggerChannelPtr;
        double triggerLevel;
        std::string edgeType;
        std::string templateFile;
        std::string reportDirectory;
        std::string reportName;


        int64_t lastPosChecked;

        std::vector<AdditionalOptions> additionalOptionsList;
        std::vector<SelectedChannel> selectedChannelList;
        std::vector<std::string> specialChannelsList;

        struct selectedChannelHasher
        {
        public:
            size_t operator()(const SelectedChannel& channel) const
            {
                return ((std::hash<std::string>()(channel.dataEntryType) ^ (std::hash<std::string>()(channel.channelType) << 1) ^
                         (std::hash<std::string>()(channel.channelName) << 1) ^ (std::hash<std::string>()(channel.cellRef) << 1)) >>
                        1) ^
                       (std::hash<int>()(channel.pageNum) << 1);
            }
        };

        std::unique_ptr <std::unordered_map<SelectedChannel, SelectedChannelProperties, selectedChannelHasher>> selectedChannelSet;

        InputManagerImpl& inputManager;
        IAppPtr app;

        std::string getDefaultRequestEndpoint();
        std::string requestEndpoint;

        bool useDefaultRequestEndpoint;

     private:
        std::string defaultRequestEndpoint = "localhost/dewesoft/realtime_excel/";

    };
}  // namespace HTTP_Requests
