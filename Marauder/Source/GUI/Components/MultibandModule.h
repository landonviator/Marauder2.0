/*
  ==============================================================================

    MultibandModule.h
    Created: 9 Aug 2022 4:30:30pm
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
class MultibandModule  : public juce::Component
{
public:
    MultibandModule(MarauderAudioProcessor&, SettingsPage&);
    ~MultibandModule() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MarauderAudioProcessor& audioProcessor;
    SettingsPage& _settingsPage;
    
    juce::GroupComponent _mainBorder;
    
    /** Skeuomorphic Comps */
    viator_gui::FilmStripKnob _band1MixDial;
    viator_gui::FilmStripKnob _band2MixDial;
    viator_gui::FilmStripKnob _band3MixDial;
    viator_gui::FilmStripKnob _band4MixDial;
    viator_gui::Label _band1MixDialLabel {"Low Band"};
    viator_gui::Label _band2MixDialLabel {"Low Mid Band"};
    viator_gui::Label _band3MixDialLabel {"Mid Band"};
    viator_gui::Label _band4MixDialLabel {"High Band"};
    
    void activateSkeuComps(bool shouldBeOn);
    
    std::vector<viator_gui::FilmStripKnob*> skeuDials =
    {
        &_band1MixDial, &_band2MixDial, &_band3MixDial, &_band4MixDial
    };
    
    std::vector<viator_gui::Label*> skeuLabels =
    {
        &_band1MixDialLabel, &_band2MixDialLabel, &_band3MixDialLabel, &_band4MixDialLabel
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultibandModule)
};
