/*
  ==============================================================================

    BitCrusherModule.cpp
    Created: 9 Aug 2022 4:41:55pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BitCrusherModule.h"

//==============================================================================
BitCrusherModule::BitCrusherModule(MarauderAudioProcessor& p, SettingsPage& s) : audioProcessor(p), _settingsPage(s)
{
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Bit Crusher Module");
}

BitCrusherModule::~BitCrusherModule()
{
}

void BitCrusherModule::paint (juce::Graphics& g)
{
}

void BitCrusherModule::resized()
{
    _mainBorder.setBounds(getLocalBounds());
}
