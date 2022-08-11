/*
  ==============================================================================

    IOModule.h
    Created: 19 Jul 2022 3:52:10pm
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
class IOModule  : public juce::Component
{
public:
    IOModule(MarauderAudioProcessor&, SettingsPage&);
    ~IOModule() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    MarauderAudioProcessor& audioProcessor;
    SettingsPage& _settingsPage;
    
    using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using buttonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    /** Skeuomorphic */
    viator_gui::FilmStripKnob _inputDial;
    viator_gui::FilmStripKnob _outputDial;
    std::unique_ptr<sliderAttachment> _inputAttach;
    std::unique_ptr<sliderAttachment> _outputAttach;
    viator_gui::Label _inputLabel {"Input"};
    viator_gui::Label _outputLabel {"Output"};
    viator_gui::ToggleButton _skeuPhaseToggle;
    viator_gui::ToggleButton _skeuHQToggle;
    std::unique_ptr<buttonAttachment> _skeuPhaseToggleAttach;
    std::unique_ptr<buttonAttachment> _skeuHQToggleAttach;
    void activateSkeuComps(bool shouldBeOn);
    void updateFlatColors();
    
    /** Flat */
    viator_gui::Dial _flatInputDial;
    viator_gui::Dial _flatOutputDial;
    std::unique_ptr<sliderAttachment> _flatInputAttach;
    std::unique_ptr<sliderAttachment> _flatOutputAttach;
    std::unique_ptr<buttonAttachment> _flatPhaseToggleAttach;
    viator_gui::PushButton _flatPhaseToggle;
    void activateFlatComps(bool shouldBeOn);
    
    void updateSliderColors(viator_gui::Dial& slider);
    void updateToggleColors(viator_gui::ToggleButton& button);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IOModule)
};
