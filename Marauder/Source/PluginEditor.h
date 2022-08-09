#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./GUI/LookAndFeel/PanelLAF.h"
#include "./GUI/LookAndFeel/FullDial.h"
#include "./GUI/Components/SettingsPage.h"
#include "./GUI/Components/Header.h"
#include "./GUI/Components/IOModule.h"
#include "./GUI/Components/MultibandModule.h"
#include "./GUI/Components/BitCrusherModule.h"
#include "./GUI/Components/DelayModule.h"
#include "./GUI/Components/FilterModule.h"

//==============================================================================

class MarauderAudioProcessorEditor  :
public juce::AudioProcessorEditor
{
public:
    MarauderAudioProcessorEditor (MarauderAudioProcessor&);
    ~MarauderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:

    MarauderAudioProcessor& audioProcessor;
    
    /** Boiler plate UI stuff */
    void uiConstructor();
    void initWindow();
    void uiPaint(juce::Graphics &g);
    void uiResized();
    void saveWindowSize();
    bool constructorFinished = false;
    
    /** Components */
    Header _headerComp;
    SettingsPage _settingsPage;
    IOModule _ioComp;
    MultibandModule _multibandComp;
    BitCrusherModule _bitCrusherComp;
    DelayModule _delayComp;
    FilterModule _filterComp;
    
    enum class Theme
    {
        kDracula,
        kPrimeDark,
        kTokyoLofi,
        kWinter,
        kFoxy,
        kPurp,
        kDull
    };
    
    Theme _pluginTheme = Theme::kDracula;
    
    void setPluginTheme(Theme newTheme);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarauderAudioProcessorEditor)
};
