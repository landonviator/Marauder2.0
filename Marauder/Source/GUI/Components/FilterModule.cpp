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
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Filter Module");
    _mainBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    
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
    
    _mainBorder.setColour(juce::GroupComponent::ColourIds::textColourId, m_textAccentColor.withLightness(0.75f).withAlpha(0.5f));
}

void FilterModule::resized()
{
    _mainBorder.setBounds(getLocalBounds().withY(getHeight() * 0.015));
    
    const auto dialX = getWidth() * 0.4;
    const auto dialY = getHeight() * 0.17;
    const auto ySkeuDialSpace = 1.3;
    const auto skeuDialSize = getWidth() * 0.5;
    
    if (_settingsPage.getUIType())
    {
        _lpDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _hpDial.setBounds(_lpDial.getX(), _lpDial.getY() + _lpDial.getHeight() * ySkeuDialSpace, skeuDialSize, skeuDialSize);
        activateFlatComps(false);
        activateSkeuComps(true);
    }
    
    else
    {
        _lpFlatDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _hpFlatDial.setBounds(_lpDial.getX(), _lpDial.getY() + _lpDial.getHeight() * ySkeuDialSpace, skeuDialSize, skeuDialSize);
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
