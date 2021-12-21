#include "gtest/gtest.h"
#include <sine_wave.h>

using SimpleTest = testing::Test;

class SineWaveTest : public testing::Test
{
public:
    SineWaveTest()
        : sineWave(nullptr, 50.0, 0.0, 1.0)
    {
    }

protected:
    SineWave sineWave;
};

TEST_F(SimpleTest, Create)
{
    ASSERT_NO_THROW(SineWave(nullptr, 50.0, 0.0, 1.0));
}

TEST_F(SineWaveTest, Frequency)
{
    ASSERT_EQ(sineWave.getFrequency(), 50.0);
}

TEST_F(SineWaveTest, Phase)
{
    ASSERT_EQ(sineWave.getPhase(), 0.0);
}

TEST_F(SineWaveTest, Amplitude)
{
    ASSERT_EQ(sineWave.getAmplitude(), 1.0);
}
