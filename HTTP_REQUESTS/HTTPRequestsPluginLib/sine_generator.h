#pragma once
#include <vector>

#include <dcomlib/dcom_input_channel/input_manager.h>
#include <dcomlib/dcom_output_channel/output_factory.h>
#include <commonlib/serialization/node.h>
#include "sine_wave.h"

class SineGenerator
{
public:
	explicit SineGenerator(Dewesoft::Utils::Dcom::OutputChannel::OutputFactory& outputFactory);

	void addSineWave(double amplitude, double phase);
	void addSineWave(double amplitude, double phase, double frequency);
	void getData(double startTime, double sampleRate, size_t numSamples);
	void clear();
	
	void saveSettings(const Dewesoft::Utils::Serialization::NodePtr& node) const;
	void loadSettings(const Dewesoft::Utils::Serialization::NodePtr& node);

	void saveSetup(const Dewesoft::Utils::Serialization::NodePtr& node) const;
	void loadSetup(const Dewesoft::Utils::Serialization::NodePtr& node);

private:
	inline static Dewesoft::Utils::Dcom::OutputChannel::OutputType SineWaveOutputType{
		Dewesoft::Utils::Dcom::Utils::ChannelDataType::Double,
		Dewesoft::Utils::Dcom::Utils::ChannelTimeBase::synchronous };
	static void SetSineWaveOutputDefaults(const Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr& channel);
	static void SetupCreatedSineWaveOutput(const Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr& channel,
		size_t numSineWaves, double amplitude);

	Dewesoft::Utils::Dcom::OutputChannel::OutputChannelPtr loadOutputChannel(const Dewesoft::Utils::Serialization::NodePtr& node) const;

	double defaultFrequency;
	Dewesoft::Utils::Dcom::OutputChannel::OutputFactory& outputFactory;
	std::vector<SineWave> sineWaves;
};
