/*
  ==============================================================================

    DelayModule.h
    Created: 9 Aug 2022 4:47:02pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "SettingsPage.h"

//==============================================================================
/*
*/
class DelayModule  : public juce::Component
{
public:
    DelayModule(MarauderAudioProcessor&, SettingsPage&);
    ~DelayModule() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MarauderAudioProcessor& audioProcessor;
    SettingsPage& _settingsPage;
    
    juce::GroupComponent _mainBorder;
    
    using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    /** Skeuomorphic Comps */
    viator_gui::FilmStripKnob _delayDial;
    viator_gui::FilmStripKnob _feedbackDial;
    viator_gui::Label _delayDialLabel {"Tension"};
    viator_gui::Label _feedbackDialLabel {"Intensity"};
    std::unique_ptr<sliderAttachment> _delayDialAttach;
    std::unique_ptr<sliderAttachment> _feedbackDialAttach;
    
    std::vector<viator_gui::FilmStripKnob*> skeuDials =
    {
        &_delayDial, &_feedbackDial
    };
    
    std::vector<viator_gui::Label*> skeuLabels =
    {
        &_delayDialLabel, &_feedbackDialLabel
    };
    
    void activateSkeuComps(bool shouldBeOn);
    
    /** Flat Comps */
    viator_gui::Dial _delayFlatDial;
    viator_gui::Dial _feedbackFlatDial;
    viator_gui::Label _delayFlatDialLabel {"Tension"};
    viator_gui::Label _feedbackFlatDialLabel {"Intensity"};
    std::unique_ptr<sliderAttachment> _delayFlatDialAttach;
    std::unique_ptr<sliderAttachment> _feedbackFlatDialAttach;
    
    std::vector<viator_gui::Dial*> flatDials =
    {
        &_delayFlatDial, &_feedbackFlatDial
    };
    
    std::vector<viator_gui::Label*> flatLabels =
    {
        &_delayFlatDialLabel, &_feedbackFlatDialLabel
    };
    
    void activateFlatComps(bool shouldBeOn);
    void updateSliderColors(viator_gui::Dial& slider);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayModule)
};
