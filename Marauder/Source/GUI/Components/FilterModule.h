/*
  ==============================================================================

    FilterModule.h
    Created: 9 Aug 2022 5:01:14pm
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
class FilterModule  : public juce::Component
{
public:
    FilterModule(MarauderAudioProcessor&, SettingsPage&);
    ~FilterModule() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MarauderAudioProcessor& audioProcessor;
    SettingsPage& _settingsPage;
    
    juce::GroupComponent _mainBorder;
    
    using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    /** Skeuomorphic Comps */
    viator_gui::FilmStripKnob _lpDial;
    viator_gui::FilmStripKnob _hpDial;
    viator_gui::Label _lpDialLabel {"LPF"};
    viator_gui::Label _hpDialLabel {"HPF"};
    std::unique_ptr<sliderAttachment> _lpDialAttach;
    std::unique_ptr<sliderAttachment> _hpDialAttach;
    
    std::vector<viator_gui::FilmStripKnob*> skeuDials =
    {
        &_lpDial, &_hpDial
    };
    
    std::vector<viator_gui::Label*> skeuLabels =
    {
        &_lpDialLabel, &_hpDialLabel
    };
    
    void activateSkeuComps(bool shouldBeOn);
    
    /** Flat Comps */
    viator_gui::Dial _lpFlatDial;
    viator_gui::Dial _hpFlatDial;
    viator_gui::Label _lpFlatDialLabel {"LPF"};
    viator_gui::Label _hpFlatDialLabel {"HPF"};
    std::unique_ptr<sliderAttachment> _lpFlatDialAttach;
    std::unique_ptr<sliderAttachment> _hpFlatDialAttach;
    
    std::vector<viator_gui::Dial*> flatDials =
    {
        &_lpFlatDial, &_hpFlatDial
    };
    
    std::vector<viator_gui::Label*> flatLabels =
    {
        &_lpFlatDialLabel, &_hpFlatDialLabel
    };
    
    void activateFlatComps(bool shouldBeOn);
    void updateSliderColors(viator_gui::Dial& slider);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModule)
};
