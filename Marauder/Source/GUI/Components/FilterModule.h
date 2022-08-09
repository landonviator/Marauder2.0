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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterModule)
};
