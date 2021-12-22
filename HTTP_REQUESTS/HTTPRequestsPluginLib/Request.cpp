#include "Request.h"
#include "SelectedChannel.h"



using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Serialization;

Webrelay::Webrelay(OutputChannelPtr outputChannel)
    : Webrelay(std::move(outputChannel), "", 0, 1.0, "10.200.47.1", 1, RisingEdge, "")
{
}

Webrelay::Webrelay(OutputChannelPtr outputChannel,
                   std::string outputChannelName,
                   int relayID,
                   double triggerLevel,
                   std::string ipAddress,
                   int relayNum,
                   EdgeTypes edgeType,
                   std::string triggerChannel)

    :

    outputChannel(std::move(outputChannel))
    , outputChannelName(outputChannelName)
    , relayID(relayID)
    , triggerLevel(triggerLevel)
    , ipAddress(ipAddress)
    , relayNum(relayNum)
    , edgeType(edgeType)
    , triggerChannel(triggerChannel)

{
}

OutputChannelPtr Webrelay::getOutputChannel() const
{
    return outputChannel;
}

std::string Webrelay::getOutputChannelName() const
{
    return outputChannelName;
}

void Webrelay::getData(const double& startTime, const double& sampleRate, const size_t& numSamples)
{
    // Add code to loop through channelData vector until data object is found with correct channel name. Then check if
    // criteria is met for trigger for rising or falling edge based on edge type and trigger level selection
    // **Possibly add some sort of delay value to prevent false triggering**
    // If triggered then send code for get request, possibly off load get request to sepperate thread.
    // Example if trigger type is rising, if prev value was less than trigger level and current value is = or greater than trigger
    // level then trigger and send get request to turn relay on. If triggered wait delay time (delay time will need to be added
    // as relay value) and if a falling edge is detected ie. prev data greater than trigger and current val less than or = then send
    // get request to turn relay off and set wait delay time. and repeat, reverse steps for falling edge trigger.

    const double dt = 1 / sampleRate;

    for (size_t i = 0; i < numSamples; ++i)
    {
        outputChannel->addSyncSample(1.0);
    }
}

void Webrelay::saveSetup(const NodePtr& node) const
{
    node->write(u8"OutputChannelName", outputChannelName);
    node->write(u8"RelayID", relayID);
    node->write(u8"IPAddress", ipAddress);
    node->write(u8"RelayNum", relayNum);
    node->write(u8"TriggerChannel", triggerChannel);
    node->write(u8"EdgeType", (edgeType == RisingEdge) ? 0 : 1);
    node->write(u8"TriggerLevel", triggerLevel);
}

void Webrelay::loadSetup(const NodePtr& node)
{
    node->read(u8"OutputChannelName", outputChannelName, 1);
    node->read(u8"RelayID", relayID, 1);
    node->read(u8"IPAddress", ipAddress, 1);
    node->read(u8"relayNum", relayNum, 1);
    node->read(u8"TriggerChannel", triggerChannel, 1);

    int tempEdgeType;
    node->read(u8"EdgeType", tempEdgeType, 1);
    edgeType = (tempEdgeType == 0) ? RisingEdge : FallingEdge;
    node->read(u8"TriggerLevel", triggerLevel, 1);
}

int Webrelay::getRelayID() const
{
    return this->relayID;
}

double Webrelay::getTriggerLevel() const
{
    return this->triggerLevel;
}

std::string Webrelay::getIPAddress() const
{
    return this->ipAddress;
}

int Webrelay::getRelayNum() const
{
    return this->relayNum;
}

EdgeTypes Webrelay::getEdgeType() const
{
    return this->edgeType;
}

std::string Webrelay::getTriggerChannel() const
{
    return this->triggerChannel;
}

void Webrelay::setTriggerLevel(double triggerLevel)
{
    this->triggerLevel = triggerLevel;
}

void Webrelay::setIPAddress(std::string ipAddress)
{
    this->ipAddress = ipAddress;
}

void Webrelay::setRelayNum(int relayNum)
{
    this->relayNum = relayNum;
}

void Webrelay::setEdgeType(EdgeTypes edgeType)
{
    this->edgeType = edgeType;
}

bool Webrelay::operator==(const Webrelay& relay1) const
{
    return (!this->ipAddress.compare(relay1.ipAddress) && this->relayNum == relay1.relayNum);
}

bool Webrelay::operator!=(const Webrelay& relay1) const
{
    return !(*this == relay1);
}
