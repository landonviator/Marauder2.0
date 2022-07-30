#ifndef MultiBandProcessor_h
#define MultiBandProcessor_h

#include <JuceHeader.h>
template <typename SampleType>
class MultiBandProcessor
{
public:
    
    /** Creates an uninitialised clipper. Call prepare() before first use. */
    MultiBandProcessor();
    
    /** Initialises the clipper. */
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    
    void reset() noexcept;
    
    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& inBlock  = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert (inBlock.getNumSamples() == outBlock.getNumSamples());

        auto len         = inBlock.getNumSamples();
        auto numChannels = inBlock.getNumChannels();
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            for (size_t sample = 0; sample < len; ++sample)
            {
                auto* input = inBlock.getChannelPointer (channel);
                auto* output = outBlock.getChannelPointer (channel);
                
                _lowBandsFilter.processSample(channel, input[sample], _lowBand, _lowMidBand);
                _midBandsFilter.processSample(channel, _lowMidBand, _lowMidBand, _midBand);
                _highBandsFilter.processSample(channel, _midBand, _midBand, _highBand);
                
                output[sample] = _lowBand * _band1 + _lowMidBand * _band2 + _midBand * _band3 + _highBand * _band4;
            }
        }
    }
    
    SampleType getLowBand()
    {
        return _lowBand;
    }
    
    SampleType getLowMidBand()
    {
        return _lowMidBand;
    }
    
    SampleType getMidBand()
    {
        return _midBand;
    }
    
    SampleType getHighBand()
    {
        return _highBand;
    }
    
    
    /** The parameters of this module. */
    enum class ParameterId
    {
        kBand1,
        kBand2,
        kBand3,
        kBand4,
        kMix,
        kOutput,
        kBypass
    };
    
    void toggleBand1(bool bandOn);
    void toggleBand2(bool bandOn);
    void toggleBand3(bool bandOn);
    void toggleBand4(bool bandOn);
    void setMix(SampleType newMix);
    void setOutput(SampleType newOutput);
    
private:
    
    // Member variables
    juce::SmoothedValue<float> _cutoff;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    float _currentSampleRate;
    bool _band1 = true;
    bool _band2 = true;
    bool _band3 = true;
    bool _band4 = true;
    
    // Bands
    float _lowBand;
    float _lowMidBand;
    float _midBand;
    float _highBand;
    
    juce::dsp::LinkwitzRileyFilter<float> _lowBandsFilter;
    juce::dsp::LinkwitzRileyFilter<float> _midBandsFilter;
    juce::dsp::LinkwitzRileyFilter<float> _highBandsFilter;
};

#endif /* Distortion_h */

