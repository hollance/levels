#include "VUMeter.h"

VUMeter::VUMeter(AnalysisData& analysis_) : analysis(analysis_)
{
    levelDecay = 1.0f - std::exp(-1.0f / (float(refreshRate) * 0.2f));
    peakDecay = 1.0f - std::exp(-1.0f / (float(refreshRate) * 0.5f));

    int xs[] = { 0, 16, 50, 66 };
    for (int i = 0; i < 4; ++i) {
        channels[i].level = 0.0f;
        channels[i].leveldB = clampdB;
        channels[i].peak = 0.0f;
        channels[i].peakdB = clampdB;
        channels[i].peakHold = 0;
        channels[i].x = xs[i];
    }

    setOpaque(true);
    startTimerHz(refreshRate);
}

void VUMeter::paint(juce::Graphics& g)
{
    const auto bounds = getLocalBounds();

    g.fillAll(juce::Colour(245, 240, 235));

    for (int i = 0; i < 4; ++i) {
        drawLevel(g, channels[i]);
    }

    g.setFont(11.0f);
    for (float db = maxdB; db >= mindB; db -= stepdB) {
        int y = positionForLevel(db);

        g.setColour(db == 0.0f ? juce::Colour(120, 120, 120) : juce::Colour(200, 200, 200));
        g.fillRect(0, y, 30, 1);
        g.fillRect(50, y, 30, 1);

        g.setColour(juce::Colour(80, 80, 80));
        g.drawSingleLineText(juce::String(int(db)), bounds.getCentreX(), y + 4,
                             juce::Justification::horizontallyCentred);
    }

    for (int i = 0; i < 4; ++i) {
        drawPeak(g, channels[i]);
    }

    int y = bounds.getHeight() - 5;
    g.setFont(14.0f);
    g.setColour(juce::Colour(0, 0, 0));
    g.drawSingleLineText("L", 4, y);
    g.drawSingleLineText("R", 19, y);
    g.drawSingleLineText("M", 52, y);
    g.drawSingleLineText("S", 70, y);
}

void VUMeter::drawLevel(juce::Graphics& g, const Channel& channel)
{
    if (channel.level > 0.0f) {
        int y = positionForLevel(channel.leveldB);
        if (channel.leveldB > 0.0f) {
            int y0 = positionForLevel(0.0f);
            g.setColour(juce::Colour(226, 74, 81));
            g.fillRect(channel.x, y, 14, y0 - y);
            g.setColour(juce::Colour(65, 206, 88));
            g.fillRect(channel.x, y0, 14, getHeight() - y0);
        } else if (y < getHeight()) {
            g.setColour(juce::Colour(65, 206, 88));
            g.fillRect(channel.x, y, 14, getHeight() - y);
        }
    }
}

void VUMeter::drawPeak(juce::Graphics& g, const Channel& channel)
{
    if (channel.peakdB > clampdB) {
        g.setColour(juce::Colour(0, 0, 0));
        g.fillRect(channel.x, positionForLevel(channel.peakdB) - 1, 14, 3);
    }
}

void VUMeter::resized()
{
    maxPos = 10.0f;
    minPos = float(getHeight()) - 30.0f;
}

void VUMeter::timerCallback()
{
    updateChannel(channels[0], analysis.levelL);
    updateChannel(channels[1], analysis.levelR);
    updateChannel(channels[2], analysis.levelM);
    updateChannel(channels[3], analysis.levelS);

    repaint();
}

void VUMeter::updateChannel(Channel& channel, Measurement& measurement)
{
    float newLevel = measurement.read();
    if (newLevel > channel.level) {
        channel.level = newLevel;  // instantaneous attack
    } else {
        channel.level += (newLevel - channel.level) * levelDecay;
    }
    if (channel.level > clampLevel) {
        channel.leveldB = juce::Decibels::gainToDecibels(channel.level);
    } else {
        channel.leveldB = clampdB;
    }

    if (channel.level > channel.peak) {
        channel.peak = channel.level;
        channel.peakdB = channel.leveldB;
        channel.peakHold = holdMax;
    } else if (channel.peakHold > 0) {
        channel.peakHold -= 1;
    } else if (channel.peakdB > clampdB) {
        channel.peak += (channel.level - channel.peak) * peakDecay;
        if (channel.peak > clampLevel) {
            channel.peakdB = juce::Decibels::gainToDecibels(channel.peak);
        } else {
            channel.peakdB = clampdB;
        }
    }
}
