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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusherModule)
};
