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
, _band1Mute(true, "M", false)
, _band1Solo(true, "S", false)
, _band2Mute(true, "M", false)
, _band2Solo(true, "S", false)
, _band3Mute(true, "M", false)
, _band3Solo(true, "S", false)
, _band4Mute(true, "M", false)
, _band4Solo(true, "S", false)
{
    //addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Multiband Module");
    
    for (auto& dial : skeuDials)
    {
        addAndMakeVisible(dial);
        dial->setDoubleClickReturnValue(true, 100.0);
    }
    
    _band1MixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band1MixID, _band1MixDial);
    _band2MixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band2MixID, _band2MixDial);
    _band3MixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band3MixID, _band3MixDial);
    _band4MixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band4MixID, _band4MixDial);
    
    for (auto& label : skeuLabels)
    {
        addAndMakeVisible(label);
    }
    
    for (auto& toggle : skeuToggles)
    {
        addAndMakeVisible(toggle);
    }
    
    _band1MuteAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band1ID, _band1Mute);
    _band2MuteAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band2ID, _band2Mute);
    _band3MuteAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band3ID, _band3Mute);
    _band4MuteAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band4ID, _band4Mute);

}

MultibandModule::~MultibandModule()
{
}

void MultibandModule::paint (juce::Graphics& g)
{
    // Paint background
    g.setColour(m_mainCompColor.withAlpha(0.2f));
    g.drawLine(getWidth(), getHeight() * 0.125, getWidth(), getHeight() * 0.875, 3);
    
    for (auto& dial : skeuDials)
    {
        dial->updateLabelColor(m_textAccentColor.withLightness(0.75f));
    }
    
    for (auto& label : skeuLabels)
    {
        label->setColour(juce::Label::ColourIds::textColourId, m_textAccentColor.withLightness(0.75f));
    }
    
    for (auto& toggle : skeuToggles)
    {
        toggle->updateLabelColor(m_textAccentColor);
    }
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
        
        _band1MixDial.setBounds(dialX,
                                dialY,
                                skeuDialSize,
                                skeuDialSize);
        
        _band2MixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(),
                                _band1MixDial.getY(),
                                skeuDialSize,
                                skeuDialSize);
        
        _band3MixDial.setBounds(dialX,
                                _band2MixDial.getY() + _band2MixDial.getHeight() * ySkeuDialSpace,
                                skeuDialSize,
                                skeuDialSize);
        
        _band4MixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(),
                                _band3MixDial.getY(),
                                skeuDialSize,
                                skeuDialSize);
        
        _band1Mute.setBounds(_band1MixDial.getX(),
                             _band1MixDial.getY() + _band1MixDial.getHeight(),
                             _band1MixDial.getWidth() * 0.5, _band1MixDial.getWidth() * 0.4);
        
        _band1Solo.setBounds(_band1MixDial.getWidth() * 0.5,
                             _band1Mute.getY(),
                             _band1Mute.getWidth(), _band1Mute.getHeight());
        
        _band2Mute.setBounds(_band2MixDial.getX(),
                             _band2MixDial.getY() + _band2MixDial.getHeight(),
                             _band2MixDial.getWidth() * 0.5, _band2MixDial.getWidth() * 0.4);
        
        _band2Solo.setBounds(_band2MixDial.getX() + _band2MixDial.getWidth() * 0.5,
                             _band2Mute.getY(),
                             _band2Mute.getWidth(), _band2Mute.getHeight());
        
        _band3Mute.setBounds(_band3MixDial.getX(),
                             _band3MixDial.getY() + _band3MixDial.getHeight(),
                             _band3MixDial.getWidth() * 0.5, _band3MixDial.getWidth() * 0.4);
        
        _band3Solo.setBounds(_band3MixDial.getWidth() * 0.5,
                             _band3Mute.getY(),
                             _band3Mute.getWidth(), _band3Mute.getHeight());
        
        _band4Mute.setBounds(_band4MixDial.getX(),
                             _band4MixDial.getY() + _band4MixDial.getHeight(),
                             _band4MixDial.getWidth() * 0.5, _band4MixDial.getWidth() * 0.4);
        
        _band4Solo.setBounds(_band4MixDial.getX() + _band4MixDial.getWidth() * 0.5,
                             _band4Mute.getY(),
                             _band4Mute.getWidth(), _band4Mute.getHeight());
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
