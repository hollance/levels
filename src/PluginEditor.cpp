#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorEditor::AudioProcessorEditor(AudioProcessor& p) :
    juce::AudioProcessorEditor(&p),
    meter(p.analysis)
{
    addAndMakeVisible(meter);
    setOpaque(true);
    setSize(80, 400);
    setWantsKeyboardFocus(false);
}

void AudioProcessorEditor::paint(juce::Graphics&)
{
    // VUMeter paints the entire bounds.
}

void AudioProcessorEditor::resized()
{
    meter.setBounds(getLocalBounds());
}
