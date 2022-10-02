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
, _lpDial(2, "", 20.0, 20000.0, true)
, _hpDial(2, "", 20.0, 20000.0, true)
, _lpFlatDial("", "", 20.0, 20000.0, 0.01, 0.0)
, _hpFlatDial("", "", 20.0, 20000.0, 1.0, 1000.0)
{
    for (auto& dial : skeuDials)
    {
        addAndMakeVisible(dial);
    }
    
    _lpDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, lpID, _lpDial);
    _hpDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, hpID, _hpDial);
    
    for (auto& label : skeuLabels)
    {
        addAndMakeVisible(label);
    }
    
    for (auto& dial : flatDials)
    {
        addAndMakeVisible(dial);
    }
    
    _lpFlatDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, lpID, _lpFlatDial);
    _hpFlatDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, hpID, _hpFlatDial);
    
    for (auto& label : flatLabels)
    {
        addAndMakeVisible(label);
    }
}

FilterModule::~FilterModule()
{
}

void FilterModule::paint (juce::Graphics& g)
{
    // Paint background
    g.setColour(m_mainCompColor.withAlpha(0.2f));
    g.drawLine(0, getHeight() * 0.125, 0, getHeight() * 0.875, 3);
    
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

void FilterModule::resized()
{
    const auto dialX = getWidth() * 0.075;
    const auto flatX = getWidth() * 0.075;
    const auto dialY = getHeight() * 0.28;
    const auto flatY = getHeight() * 0.28;
    const auto ySkeuDialSpace = 1.17;
    const auto yFlatDialSpace = 1.17;
    const auto skeuDialSize = getWidth() * 0.35;
    const auto flatDialSize = getWidth() * 0.38;
    
    if (_settingsPage.getUIType())
    {
        _hpDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _lpDial.setBounds(_hpDial.getX() + _hpDial.getWidth() * ySkeuDialSpace, _hpDial.getY(), skeuDialSize, skeuDialSize);
        activateFlatComps(false);
        activateSkeuComps(true);
    }
    
    else
    {
        _hpFlatDial.setBounds(flatX, flatY, flatDialSize, flatDialSize);
        _lpFlatDial.setBounds(_hpFlatDial.getX() + _hpFlatDial.getWidth() * yFlatDialSpace, _hpFlatDial.getY(), flatDialSize, flatDialSize);
        activateFlatComps(true);
        activateSkeuComps(false);
    }
}

void FilterModule::activateSkeuComps(bool shouldBeOn)
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

void FilterModule::activateFlatComps(bool shouldBeOn)
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

void FilterModule::updateSliderColors(viator_gui::Dial &slider)
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
