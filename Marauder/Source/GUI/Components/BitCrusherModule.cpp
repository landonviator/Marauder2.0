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
, _crushDial(1, "", 1.0, 16.0, true)
, _resampleDial(1, "", 5.0, 50.0, false)
, _driveDial(1, " dB", 0.0, 20.0, false)
, _mixDial(1, " %", 0.0, 100.0, true)
, _crushDialFlat("", "", 1.0, 16.0, 1.0, 16.0)
, _resampleDialFlat("", "", 5.0, 50.0, 0.01, 50.0)
, _driveDialFlat(" dB", "", 0.0, 20.0, 0.01, 0.0)
, _mixDialFlat(" %", "", 0.0, 100.0, 1.0, 100.0)
{
    for (auto& dial : skeuDials)
    {
        addAndMakeVisible(dial);
    }
    
    _crushDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, bitDepth1ID, _crushDial);
    _resampleDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, resampleRateID, _resampleDial);
    _driveDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, driveID, _driveDial);
    _mixDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, masterMixID, _mixDial);
    
    for (auto& label : skeuLabels)
    {
        addAndMakeVisible(label);
    }
    
    for (auto& dial : flatDials)
    {
        addAndMakeVisible(dial);
    }
    
    _crushDialFlatAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, bitDepth1ID, _crushDialFlat);
    _resampleDialFlatAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, resampleRateID, _resampleDialFlat);
    _driveDialFlatAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, driveID, _driveDialFlat);
    _mixDialFlatAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, masterMixID, _mixDialFlat);
    
    for (auto& label : flatLabels)
    {
        addAndMakeVisible(label);
    }
}

BitCrusherModule::~BitCrusherModule()
{
}

void BitCrusherModule::paint (juce::Graphics& g)
{
    // Paint background
    g.setColour(m_mainCompColor.withAlpha(0.2f));
    g.drawLine(0, getHeight(), getWidth(), getHeight(), 3);
    
    for (auto& dial : skeuDials)
    {
        dial->updateLabelColor(m_textAccentColor.withLightness(0.75f));
    }
    
    for (auto& label : skeuLabels)
    {
        label->setColour(juce::Label::ColourIds::textColourId, m_textAccentColor.withLightness(0.75f));
    }
    
    for (auto& dial : flatDials)
    {
        updateSliderColors(*dial);
    }
}

void BitCrusherModule::resized()
{
    const auto dialX = getWidth() * 0.088;
    const auto dialY = getHeight() * 0.14;
    const auto ySkeuDialSpace = 1.35;
    const auto xSkeuDialSpace = 1.2;
    const auto skeuDialSize = getWidth() * 0.38;
    const auto flatDialSize = getWidth() * 0.41;
    const auto yFlatDialSpace = 1.2;
    const auto flatX = 0;
    const auto flatY = getHeight() * 0.175;
    
    // Skeuomorphic
    if (_settingsPage.getUIType())
    {
        _crushDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _resampleDial.setBounds(_crushDial.getX() + _crushDial.getWidth() * xSkeuDialSpace, dialY, skeuDialSize, skeuDialSize);
        _driveDial.setBounds(_crushDial.getX(), _crushDial.getY() + _crushDial.getHeight() * ySkeuDialSpace, skeuDialSize, skeuDialSize);
        _mixDial.setBounds(_resampleDial.getX(), _driveDial.getY(), skeuDialSize, skeuDialSize);
        activateFlatComps(false);
        activateSkeuComps(true);
    }
    
    else
    {
        _crushDialFlat.setBounds(flatX, flatY, flatDialSize, flatDialSize);
        _resampleDialFlat.setBounds(_crushDialFlat.getX() + _crushDialFlat.getWidth() * xSkeuDialSpace, _crushDialFlat.getY(), flatDialSize, flatDialSize);
        _driveDialFlat.setBounds(_crushDialFlat.getX(), _crushDialFlat.getY() + _crushDialFlat.getHeight() * yFlatDialSpace, flatDialSize, flatDialSize);
        _mixDialFlat.setBounds(_resampleDialFlat.getX(), _driveDialFlat.getY(), flatDialSize, flatDialSize);
        activateFlatComps(true);
        activateSkeuComps(false);
    }
}

void BitCrusherModule::activateSkeuComps(bool shouldBeOn)
{
    for (auto& dial : skeuDials)
    {
        dial->setEnabled(shouldBeOn);
        dial->setVisible(shouldBeOn);
    }
    
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

void BitCrusherModule::activateFlatComps(bool shouldBeOn)
{
    for (auto& dial : flatDials)
    {
        dial->setEnabled(shouldBeOn);
        dial->setVisible(shouldBeOn);
    }
    
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

void BitCrusherModule::updateSliderColors(viator_gui::Dial &slider)
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
