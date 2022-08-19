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
, _delayDial(0, "", 0.0, 1700.0, true)
, _feedbackDial(0, "", 0.0, 0.95, false)
, _delayLPDial(0, "", 20.0, 20000.0, true)
, _delayFlatDial("", "", 0.0, 1700.0, 1.0, 0.0)
, _feedbackFlatDial("", "", 0.0, 95.0, 0.01, 0.0)
, _delayLPFlatDial("", "", 20.0, 20000.0, 1.0, 1000.0)
{
    addAndMakeVisible(_mainBorder);
    _mainBorder.setText("Delay Module");
    _mainBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    
    for (auto& dial : skeuDials)
    {
        addAndMakeVisible(dial);
    }
    
    _delayDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, delayTimeID, _delayDial);
    _feedbackDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, feedbackID, _feedbackDial);
    _delayLPDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, delayLPID, _delayLPDial);
    
    for (auto& label : skeuLabels)
    {
        addAndMakeVisible(label);
    }
    
    for (auto& dial : flatDials)
    {
        addAndMakeVisible(dial);
    }
    
    _delayFlatDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, delayTimeID, _delayFlatDial);
    _feedbackFlatDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, feedbackID, _feedbackFlatDial);
    _delayLPFlatDialAttach = std::make_unique<sliderAttachment>(audioProcessor._treeState, delayLPID, _delayLPFlatDial);
    
    for (auto& label : flatLabels)
    {
        addAndMakeVisible(label);
    }
}

DelayModule::~DelayModule()
{
}

void DelayModule::paint (juce::Graphics& g)
{
    // Paint background
    g.setColour(m_mainCompColor.withAlpha(0.2f));
    g.drawLine(0, getHeight() * 0.125, 0, getHeight() * 0.875, 3);
    g.drawLine(getWidth() * 0.125, 0, getWidth() * 0.875, 0, 3);
    
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

void DelayModule::resized()
{
    _mainBorder.setBounds(getLocalBounds().withY(getHeight() * 0.057));
    
    const auto dialX = getWidth() * 0.088;
    const auto dialY = getHeight() * 0.29;
    const auto xSkeuDialSpace = 1.86;
    const auto skeuDialSize = getWidth() * 0.18;
    const auto flatDialSize = getWidth() * 0.2;
    const auto xFlatDialSpace = 1.65;
    
    if (_settingsPage.getUIType())
    {
        _delayDial.setBounds(dialX, dialY, skeuDialSize, skeuDialSize);
        _feedbackDial.setBounds(_delayDial.getX() + _delayDial.getWidth() * xSkeuDialSpace, dialY, skeuDialSize, skeuDialSize);
        _delayLPDial.setBounds(_feedbackDial.getX() + _feedbackDial.getWidth() * xSkeuDialSpace, dialY, skeuDialSize, skeuDialSize);
        activateFlatComps(false);
        activateSkeuComps(true);
    }
    
    else
    {
        _delayFlatDial.setBounds(dialX, dialY, flatDialSize, flatDialSize);
        _feedbackFlatDial.setBounds(_delayFlatDial.getX() + _delayFlatDial.getWidth() * xFlatDialSpace, dialY, flatDialSize, flatDialSize);
        _delayLPFlatDial.setBounds(_feedbackFlatDial.getX() + _feedbackFlatDial.getWidth() * xFlatDialSpace, dialY, flatDialSize, flatDialSize);
        activateFlatComps(true);
        activateSkeuComps(false);
    }
}

void DelayModule::activateSkeuComps(bool shouldBeOn)
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

void DelayModule::activateFlatComps(bool shouldBeOn)
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

void DelayModule::updateSliderColors(viator_gui::Dial &slider)
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
