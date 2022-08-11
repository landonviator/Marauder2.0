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
, _band1FlatMixDial(" %", "Band 1", 0.0, 100.0, 1.0, 100.0)
, _band2FlatMixDial(" %", "Band 2", 0.0, 100.0, 1.0, 100.0)
, _band3FlatMixDial(" %", "Band 3", 0.0, 100.0, 1.0, 100.0)
, _band4FlatMixDial(" %", "Band 4", 0.0, 100.0, 1.0, 100.0)
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
    
    for (auto& dial : flatDials)
    {
        addAndMakeVisible(dial);
        dial->setDoubleClickReturnValue(true, 100.0);
    }
    
    _band1FlatMixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band1MixID, _band1FlatMixDial);
    _band2FlatMixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band2MixID, _band2FlatMixDial);
    _band3FlatMixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band3MixID, _band3FlatMixDial);
    _band4FlatMixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, band4MixID, _band4FlatMixDial);
    
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
    _band1SoloAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band1SoloID, _band1Solo);
    _band2SoloAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band2SoloID, _band2Solo);
    _band3SoloAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band3SoloID, _band3Solo);
    _band4SoloAttach = std::make_unique<buttonAttachment>(audioProcessor._treeState, band4SoloID, _band4Solo);
    
    setSkeuOnClicks();
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
    
    for (auto& dial : flatDials)
    {
        updateSliderColors(*dial);
    }
}

void MultibandModule::resized()
{
    _mainBorder.setBounds(getLocalBounds());
    
    const auto dialX = getWidth() * 0.01;
    const auto dialY = getHeight() * 0.12;
    const auto ySkeuDialSpace = 2.0;
    const auto skeuDialSize = getWidth() * 0.5;
    const auto toggleX = getWidth() * 0.31;
    const auto toggleSize = getWidth() * 0.35;
    
    // Skeuomorphic
    if (_settingsPage.getUIType())
    {
        activateFlatComps(false);
        
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
    
    // Flat
    else
    {
        activateSkeuComps(false);
        const auto yFlatDialSpace = 1.3;
        const auto flatDialSize = getWidth() * 0.55;

        _band1FlatMixDial.setBounds(dialX,
                                dialY,
                                skeuDialSize,
                                skeuDialSize);
        
        _band2FlatMixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(),
                                _band1MixDial.getY(),
                                skeuDialSize,
                                skeuDialSize);
        
        _band3FlatMixDial.setBounds(dialX,
                                _band2MixDial.getY() + _band2MixDial.getHeight() * ySkeuDialSpace,
                                skeuDialSize,
                                skeuDialSize);
        
        _band4FlatMixDial.setBounds(_band1MixDial.getX() + _band1MixDial.getWidth(),
                                _band3MixDial.getY(),
                                skeuDialSize,
                                skeuDialSize);
        
        activateFlatComps(true);
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

void MultibandModule::activateFlatComps(bool shouldBeOn)
{
    for (auto& dial : flatDials)
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
            if (skeuLabels[label]->getAttachedComponent() != flatDials[label])
            {
                skeuLabels[label]->attachToComponent(flatDials[label], false);
            }
        }
    }
}

void MultibandModule::setSkeuOnClicks()
{
    for (size_t solo = 0; solo < skeuSolos.size(); ++solo)
    {
        skeuSolos[solo]->onClick = [this, solo]()
        {
            if (skeuSolos[solo]->getToggleState())
            {
                skeuMuteToggleLogic(*skeuMutes[solo], *skeuSolos[solo]);
            }
            
            else
            {
                resetMuteSoloLogic();
            }
        };
    }
}

void MultibandModule::skeuMuteToggleLogic(viator_gui::ToggleButton& muteButton, viator_gui::ToggleButton& soloButton)
{
    for (auto& button : skeuSolos)
    {
        if (button != &soloButton)
        {
            if (button->getToggleState())
            {
                button->setToggleState(false, juce::dontSendNotification);
            }
        }
    }
    
    if (muteButton.getToggleState())
    {
        muteButton.triggerClick();
    }
    
    for (auto& button : skeuMutes)
    {
        if (soloButton.getToggleState())
        {
            if (button != &muteButton)
            {
                if (!button->getToggleState())
                {
                    button->triggerClick();
                }
            }
        }

        else
        {
            if (button != &muteButton)
            {
                if (button->getToggleState())
                {
                    button->triggerClick();
                }
            }
        }
    }
}


void MultibandModule::resetMuteSoloLogic()
{
    for (auto& toggle : skeuToggles)
    {
        if (toggle->getToggleState())
        {
            toggle->triggerClick();
        }
    }
}

void MultibandModule::updateSliderColors(viator_gui::Dial &slider)
{
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, m_mainCompFillColor);
    slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, m_bgLighterColor);
    slider.setColour(juce::Slider::ColourIds::trackColourId, m_textAccentColor);
    slider.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    slider.setColour(juce::Slider::ColourIds::textBoxTextColourId, m_textAccentColor);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, m_mainCompColor);
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, m_bgTransparent);
    
    if (_settingsPage.getGlowState())
    {
        slider.updateShadowColor(m_mainCompColor, _settingsPage.getCurrentGlowValue());
    }
    
    else
    {
        slider.updateShadowColor(juce::Colours::black, 0.4f);
    }
    
    slider.forceShadow();
}
