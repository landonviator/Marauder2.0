/*
  ==============================================================================

    FilterModule.cpp
    Created: 9 Aug 2022 5:01:14pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterModule.h"

//==============================================================================
FilterModule::FilterModule(MarauderAudioProcessor& p, SettingsPage& s) : audioProcessor(p), _settingsPage(s)
{
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Filter Module");
    _mainBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
}

FilterModule::~FilterModule()
{
}

void FilterModule::paint (juce::Graphics& g)
{
    // Paint background
    g.setColour(m_mainCompColor.withAlpha(0.2f));
    g.drawLine(0, getHeight() * 0.125, 0, getHeight() * 0.875, 3);
    
    _mainBorder.setColour(juce::GroupComponent::ColourIds::textColourId, m_textAccentColor.withLightness(0.75f).withAlpha(0.5f));
}

void FilterModule::resized()
{
    _mainBorder.setBounds(getLocalBounds().withY(getHeight() * 0.015));
}
