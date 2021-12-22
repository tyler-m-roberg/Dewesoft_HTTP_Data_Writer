#pragma once
#include <dcomlib/dcom_input_channel/input_manager_impl.h>

class Request
{
public:
    explicit Request();

    explicit Request(std::string outputChannelName,
                      int relayID,
                      double triggerLevel,
                      std::string ipAddress,
                      int relayNum,
                      EdgeTypes edgeType,
                      std::string triggerChannel);


    void getData(const double& startTime, const double& sampleRate, const size_t& numSamples);

    void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
    void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

    int getRelayID() const;
    double getTriggerLevel() const;
    std::string getIPAddress() const;
    int getRelayNum() const;
    EdgeTypes getEdgeType() const;
    std::string getTriggerChannel() const;

    void setTriggerLevel(double triggerLevel);
    void setIPAddress(std::string ipAddress);
    void setRelayNum(int relayNum);
    void setEdgeType(EdgeTypes edgeType);

    bool operator==(const Request& requestObj) const;

    bool operator!=(const Request& requestObj) const;

private:
   
    std::string outputChannelName;

    // Store a relay identifier on creation and use to access output channel
    int relayID;
    std::string ipAddress;
    int relayNum;
    std::string triggerChannel;
    EdgeTypes edgeType;
    double triggerLevel;
};
