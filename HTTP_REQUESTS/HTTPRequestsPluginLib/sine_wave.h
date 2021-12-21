#pragma once

#include <dcomlib/dcom_output_channel/output_channel.h>
#include <dcomlib/dcom_output_channel/output_factory.h>

class SineWave
{
public:
	explicit SineWave(Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr outputChannel);
	explicit SineWave(Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr outputChannel,
		double frequency,
		double phase,
		double amplitude);

	Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr getOutputChannel() const;
	double getFrequency() const;
	double getPhase() const;
	double getAmplitude() const;
	
	void getData(double startTime, double sampleRate, size_t numSamples);

	void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
	void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

private:
	static constexpr double Pi{ 3.141592653589793238463 };

	Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr outputChannel;
	double frequency;
	double phase;
	double amplitude;
};