/*
  ==============================================================================

    DelayModule.cpp
    Created: 9 Aug 2022 4:47:02pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayModule.h"

//==============================================================================
DelayModule::DelayModule(MarauderAudioProcessor& p, SettingsPage& s) : audioProcessor(p), _settingsPage(s)
{
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Delay Module");
}

DelayModule::~DelayModule()
{
}

void DelayModule::paint (juce::Graphics& g)
{
}

void DelayModule::resized()
{
    _mainBorder.setBounds(getLocalBounds());
}
