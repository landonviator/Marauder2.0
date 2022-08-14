/*
  ==============================================================================

    BitCrusherModule.h
    Created: 9 Aug 2022 4:41:55pm
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
class BitCrusherModule  : public juce::Component
{
public:
    BitCrusherModule(MarauderAudioProcessor&, SettingsPage&);
    ~BitCrusherModule() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MarauderAudioProcessor& audioProcessor;
    SettingsPage& _settingsPage;
    
    juce::GroupComponent _mainBorder;
    
    using sliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    /** Skeuomorphic Comps */
    viator_gui::FilmStripKnob _crushDial;
    viator_gui::FilmStripKnob _resampleDial;
    viator_gui::FilmStripKnob _driveDial;
    viator_gui::FilmStripKnob _mixDial;
    viator_gui::Label _crushDialLabel {"Bit Crush"};
    viator_gui::Label _resampleDialLabel {"Resample"};
    viator_gui::Label _driveDialLabel {"Drive"};
    viator_gui::Label _mixDialLabel {"Mix"};
    std::unique_ptr<sliderAttachment> _crushDialAttach;
    std::unique_ptr<sliderAttachment> _resampleDialAttach;
    std::unique_ptr<sliderAttachment> _driveDialAttach;
    std::unique_ptr<sliderAttachment> _mixDialAttach;
    
    std::vector<viator_gui::FilmStripKnob*> skeuDials =
    {
        &_crushDial, &_resampleDial, &_driveDial, &_mixDial
    };
    
    std::vector<viator_gui::Label*> skeuLabels =
    {
        &_crushDialLabel, &_resampleDialLabel, &_driveDialLabel, &_mixDialLabel
    };
    
    void activateSkeuComps(bool shouldBeOn);
    
    /** Flat Comps*/
    viator_gui::Dial _crushDialFlat;
    viator_gui::Dial _resampleDialFlat;
    viator_gui::Dial _driveDialFlat;
    viator_gui::Dial _mixDialFlat;
    viator_gui::Label _crushDialLabelFlat {"Bit Crush"};
    viator_gui::Label _resampleDialLabelFlat {"Resample"};
    viator_gui::Label _driveDialLabelFlat {"Drive"};
    viator_gui::Label _mixDialLabelFlat {"Mix"};
    std::unique_ptr<sliderAttachment> _crushDialFlatAttach;
    std::unique_ptr<sliderAttachment> _resampleDialFlatAttach;
    std::unique_ptr<sliderAttachment> _driveDialFlatAttach;
    std::unique_ptr<sliderAttachment> _mixDialFlatAttach;
    
    std::vector<viator_gui::Dial*> flatDials =
    {
        &_crushDialFlat, &_resampleDialFlat, &_driveDialFlat, &_mixDialFlat
    };
    
    std::vector<viator_gui::Label*> flatLabels =
    {
        &_crushDialLabelFlat, &_resampleDialLabelFlat, &_driveDialLabelFlat, &_mixDialLabelFlat
    };
    
    void activateFlatComps(bool shouldBeOn);
    void updateSliderColors(viator_gui::Dial& slider);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherModule)
};
