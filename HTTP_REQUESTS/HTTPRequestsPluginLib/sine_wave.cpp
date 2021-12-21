#include "sine_wave.h"
#include <cmath>

using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Serialization;

SineWave::SineWave(OutputChannelPtr outputChannel)
    : SineWave(std::move(outputChannel), 1, 0, 1)
{
}

SineWave::SineWave(OutputChannelPtr outputChannel,
                   const double frequency,
                   const double phase,
                   const double amplitude)
    : outputChannel(std::move(outputChannel))
    , frequency(frequency)
    , phase(phase)
    , amplitude(amplitude)
{
}

OutputChannelPtr SineWave::getOutputChannel() const
{
    return outputChannel;
}

double SineWave::getFrequency() const
{
    return frequency;
}

double SineWave::getPhase() const
{
    return phase;
}

double SineWave::getAmplitude() const
{
    return amplitude;
}

void SineWave::getData(const double startTime, const double sampleRate, const size_t numSamples)
{
    const double dt = 1 / sampleRate;
    for (size_t i = 0; i < numSamples; ++i)
    {
        const double time = startTime + i * dt;
        const double sineValue = amplitude * sin(2 * Pi * frequency * time + phase);
        outputChannel->addSyncSample(sineValue);
    }
}

void SineWave::saveSetup(const NodePtr& node) const
{
    node->write(u8"Frequency", frequency);
    node->write(u8"Phase", phase);
    node->write(u8"Amplitude", amplitude);
}

void SineWave::loadSetup(const NodePtr& node)
{
    node->read(u8"Frequency", frequency, 1);
    node->read(u8"Phase", phase, 0);
    node->read(u8"Amplitude", amplitude, 1);
}
