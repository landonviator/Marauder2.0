/*
  ==============================================================================

    MultibandModule.cpp
    Created: 9 Aug 2022 4:30:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MultibandModule.h"

//==============================================================================
MultibandModule::MultibandModule(MarauderAudioProcessor& p, SettingsPage& s) : audioProcessor(p), _settingsPage(s)
, _band1MixDial(2, " %", 0.0, 100.0)
, _band2MixDial(2, " %", 0.0, 100.0)
, _band3MixDial(2, " %", 0.0, 100.0)
, _band4MixDial(2, " %", 0.0, 100.0)
{
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Multiband Module");

    addAndMakeVisible(_band1MixDial);
    _band1MixDial.setDoubleClickReturnValue(true, 100.0);
    
    addAndMakeVisible(_band2MixDial);
    _band2MixDial.setDoubleClickReturnValue(true, 100.0);
    
    addAndMakeVisible(_band3MixDial);
    _band3MixDial.setDoubleClickReturnValue(true, 100.0);
    
    addAndMakeVisible(_band4MixDial);
    _band4MixDial.setDoubleClickReturnValue(true, 100.0);
    
    addAndMakeVisible(_band1MixDialLabel);
    addAndMakeVisible(_band2MixDialLabel);
    addAndMakeVisible(_band3MixDialLabel);
    addAndMakeVisible(_band4MixDialLabel);
}

MultibandModule::~MultibandModule()
{
}

void MultibandModule::paint (juce::Graphics& g)
{
}

void MultibandModule::resized()
{
    _mainBorder.setBounds(getLocalBounds());
    
    const auto dialX = getWidth() * 0.01;
    const auto dialY = getHeight() * 0.12;
    
    // Skeuomorphic
    if (_settingsPage.getUIType())
    {
        //activateFlatComps(false);
        const auto ySkeuDialSpace = 2.0;
        const auto skeuDialSize = getWidth() * 0.5;
        const auto toggleX = getWidth() * 0.31;
        const auto toggleSize = getWidth() * 0.35;
        
        _band1MixDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _band2MixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(), _band1MixDial.getY(), skeuDialSize, skeuDialSize);
        _band3MixDial.setBounds(dialX, _band2MixDial.getY() + _band2MixDial.getHeight() * ySkeuDialSpace, skeuDialSize, skeuDialSize);
        _band4MixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(), _band3MixDial.getY(), skeuDialSize, skeuDialSize);
        //_skeuPhaseToggle.setBounds(toggleX, _outputDial.getY() + _outputDial.getHeight(), toggleSize, toggleSize);
        activateSkeuComps(true);
    }
}

void MultibandModule::activateSkeuComps(bool shouldBeOn)
{
    for (auto& dial : skeuDials)
    {
        dial->setEnabled(shouldBeOn);
        dial->setVisible(shouldBeOn);
    }
    
    // Toggle
    //_skeuPhaseToggle.setEnabled(shouldBeOn);
    //_skeuPhaseToggle.setVisible(shouldBeOn);
    
    // Switch component attachment
    if (shouldBeOn)
    {
        for (size_t label = 0; label < skeuLabels.size(); ++label)
        {
            if (skeuLabels[label]->getAttachedComponent() != skeuDials[label])
            {
                skeuLabels[label]->attachToComponent(skeuDials[label], false);
            }
        }
    }
}
