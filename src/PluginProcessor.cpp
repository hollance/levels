#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessor::AudioProcessor() :
    juce::AudioProcessor(
        BusesProperties()
            .withInput("Input", juce::AudioChannelSet::stereo(), true)
            .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

void AudioProcessor::prepareToPlay(double, int)
{
    reset();
}

void AudioProcessor::releaseResources()
{
    // do nothing
}

void AudioProcessor::reset()
{
    analysis.reset();
}

bool AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

void AudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto numInputChannels = getTotalNumInputChannels();
    auto numOutputChannels = getTotalNumOutputChannels();
    auto numSamples = buffer.getNumSamples();

    // Clear any output channels that don't contain input data.
    for (auto i = numInputChannels; i < numOutputChannels; ++i) {
        buffer.clear(i, 0, numSamples);
    }

    bool stereo = numInputChannels > 1;
    const float* channelL = buffer.getReadPointer(0);
    const float* channelR = buffer.getReadPointer(stereo ? 1 : 0);

    float levelL = 0.0f;
    float levelR = 0.0f;
    float levelM = 0.0f;
    float levelS = 0.0f;

    for (int sample = 0; sample < numSamples; ++sample) {
        float sampleL = channelL[sample];
        float sampleR = channelR[sample];
        float sampleM = (sampleL + sampleR) * 0.5f;
        float sampleS = (sampleL - sampleR) * 0.5f;

        levelL = std::max(levelL, std::abs(sampleL));
        levelR = std::max(levelR, std::abs(sampleR));
        levelM = std::max(levelM, std::abs(sampleM));
        levelS = std::max(levelS, std::abs(sampleS));
    }

    analysis.levelL.update(levelL);
    analysis.levelR.update(levelR);
    analysis.levelM.update(levelM);
    analysis.levelS.update(levelS);
}

void AudioProcessor::getStateInformation(juce::MemoryBlock&)
{
    // do nothing
}

void AudioProcessor::setStateInformation(const void*, int)
{
    // do nothing
}

juce::AudioProcessorEditor* AudioProcessor::createEditor()
{
    return new AudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioProcessor();
}
