#include "sine_generator.h"
#include <string>

using namespace Dewesoft::Utils::Dcom::OutputChannel;
using namespace Dewesoft::Utils::Dcom::InputChannel;
using namespace Dewesoft::Utils::Serialization;

SineGenerator::SineGenerator(OutputFactory& outputFactory)
    : defaultFrequency(2.0)
    , outputFactory(outputFactory)
{
}

void SineGenerator::addSineWave(const double amplitude, const double phase)
{
    addSineWave(amplitude, phase, defaultFrequency);
}

void SineGenerator::addSineWave(const double amplitude, const double phase, const double frequency)
{
    const OutputChannelPtr outputChannel = outputFactory.createOutputChannel(SineWaveOutputType, SetSineWaveOutputDefaults);
    SetupCreatedSineWaveOutput(outputChannel, sineWaves.size() + 1, amplitude);
    sineWaves.emplace_back(outputChannel, frequency, phase, amplitude);
}

void SineGenerator::getData(const double startTime, const double sampleRate, const size_t numSamples)
{
    for (auto& sineWave : sineWaves)
        sineWave.getData(startTime, sampleRate, numSamples);

}

void SineGenerator::clear()
{
    sineWaves.clear();
}

void SineGenerator::saveSettings(const NodePtr& node) const
{
    node->write(u8"DefaultFrequency", defaultFrequency);
}

void SineGenerator::loadSettings(const NodePtr& node)
{
    node->read(u8"DefaultFrequency", defaultFrequency, 2);
}

void SineGenerator::saveSetup(const NodePtr& node) const
{
    const auto sineWaveList = node->addChild(u8"SineWaveList");
    for (const auto& sineWave : sineWaves)
    {
        const auto sineWaveNode = sineWaveList->addChild(u8"SineWave");
        sineWaveNode->write(u8"OutputChannelId", sineWave.getOutputChannel()->getId());
        sineWave.saveSetup(sineWaveNode);
    }
}

void SineGenerator::loadSetup(const NodePtr& node)
{
    const auto sineWaveList = node->findChildNode(u8"SineWaveList");
    if (!sineWaveList)
        return;

    for (size_t i = 0; i < sineWaveList->getChildCount(); ++i)
    {
        const auto sineWaveNode = sineWaveList->getChild(i);
        sineWaves.emplace_back(loadOutputChannel(sineWaveNode));
        sineWaves.back().loadSetup(sineWaveNode);
    }
}

void SineGenerator::SetSineWaveOutputDefaults(const OutputChannelPtr& channel)
{
    channel->setName(u8"SineWave");
    channel->setUserScaleMax(1.0f);
    channel->setUserScaleMin(-1.0f);
    channel->setUsed(true);
}

void SineGenerator::SetupCreatedSineWaveOutput(const OutputChannelPtr& channel, const size_t numSineWaves, const double amplitude)
{
    channel->setName(u8"SineWave_" + std::to_string(numSineWaves));
    channel->setUserScaleMax(static_cast<float>(amplitude));
    channel->setUserScaleMin(static_cast<float>(-amplitude));
    channel->setUsed(true);
}

OutputChannelPtr SineGenerator::loadOutputChannel(const NodePtr& node) const
{
    int32_t outputChannelId;
    node->read(u8"OutputChannelId", outputChannelId, -1);
    return outputFactory.findOutputChannel(outputChannelId, SineWaveOutputType, SetSineWaveOutputDefaults);
}

