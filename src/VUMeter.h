#pragma once

#include <JuceHeader.h>
#include "AnalysisData.h"

class VUMeter : public juce::Component, private juce::Timer
{
public:
    VUMeter(AnalysisData& analysis);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    struct Channel
    {
        float level;
        float leveldB;
        float peak;
        float peakdB;
        int peakHold;
        int x;
    };

    void timerCallback() override;
    void updateChannel(Channel& channel, Measurement& measurement);
    void drawLevel(juce::Graphics& g, const Channel& channel);
    void drawPeak(juce::Graphics& g, const Channel& channel);

    int positionForLevel(float dbLevel) const noexcept
    {
        return int(std::round(juce::jmap(dbLevel, maxdB, mindB, maxPos, minPos)));
    }

    AnalysisData& analysis;

    static constexpr float maxdB = 12.0f;   // highest dB shown
    static constexpr float mindB = -60.0f;  // lowest dB shown
    static constexpr float stepdB = 6.0f;   // draw a tick every 6 dB

    static constexpr float clampdB = -120.0f;
    static constexpr float clampLevel = 0.000001f;  // -120 dB

    static constexpr int refreshRate = 60;
    static constexpr int holdMax = refreshRate * 2;

    Channel channels[4];  // L, R, M, S

    float maxPos;  // maxdB line
    float minPos;  // mindB line

    float levelDecay;  // filter coefficients
    float peakDecay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VUMeter)
};
