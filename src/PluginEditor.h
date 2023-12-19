#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VUMeter.h"

class AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AudioProcessorEditor(AudioProcessor&);

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    VUMeter meter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessorEditor)
};
