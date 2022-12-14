#pragma once
#include <JuceHeader.h>
#include "Parameters/Globals.h"
#include "DSP/Marauder.h"

//==============================================================================
/**
*/
class MarauderAudioProcessor  : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    MarauderAudioProcessor();
    ~MarauderAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState _treeState;
    juce::ValueTree variableTree
    { "Variables", {},
        {
        { "Group", {{ "name", "Vars" }},
            {
                { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "height" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "glowslider" }, { "value", 1 }}},
                { "Parameter", {{ "id", "glowtoggle" }, { "value", 1 }}},
                { "Parameter", {{ "id", "gradienttoggle" }, { "value", 1 }}},
                { "Parameter", {{ "id", "colortheme" }, { "value", 0 }}},
                { "Parameter", {{ "id", "skeutoggle" }, { "value", 1 }}}
            }
        }
        }
    };
    
    /** Window Vars =====================================================*/
    float windowWidth {0.0f};
    float windowHeight {0.0f};
    
private:
    /** JUCE Boilerplate */
    juce::dsp::ProcessSpec spec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    
    /** DSP */
    juce::dsp::Gain<float> _inputGainModule;
    juce::dsp::Gain<float> _outputGainModule;
    viator_dsp::Marauder<float> _marauder;
    juce::dsp::LinkwitzRileyFilter<float> _aliasFilter;
    juce::dsp::LinkwitzRileyFilter<float> _artifactFilter;
    viator_dsp::SVFilter<float> _hpFilter;
    viator_dsp::SVFilter<float> _lpFilter;
    void normalProcessBlock(juce::AudioBuffer<float>& buffer);
    bool hqToggle;
    void updateParameters();
    
    static constexpr auto effectDelaySamples = 192000;
    juce::dsp::DelayLine<float> delay { effectDelaySamples };
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> linear { effectDelaySamples };
    
    std::array<float, 2> delayValue { {} };
    std::array<float, 2> lastDelayOutput;
    std::array<juce::LinearSmoothedValue<float>, 2> delayFeedbackVolume;
    juce::SmoothedValue<float> delayAmount;
    viator_dsp::SVFilter<float> _feedbackLPFilter;
    void processDelay(juce::dsp::AudioBlock<float>& block);
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MarauderAudioProcessor)
};
