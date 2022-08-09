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
}

FilterModule::~FilterModule()
{
}

void FilterModule::paint (juce::Graphics& g)
{
}

void FilterModule::resized()
{
    _mainBorder.setBounds(getLocalBounds());
}
