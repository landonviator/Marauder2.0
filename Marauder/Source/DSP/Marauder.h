#ifndef Marauder_h
#define Marauder_h

#include <JuceHeader.h>

namespace viator_dsp
{
template <typename SampleType>
class Marauder
{
public:
    
    /** Creates an uninitialised clipper. Call prepare() before first use. */
    Marauder();
    
    /** Initialises the clipper. */
    void prepare(const juce::dsp::ProcessSpec& spec) noexcept;
    
    void reset() noexcept;
    
    void processBuffer(juce::AudioBuffer<float>& buffer)
    {
        auto data = buffer.getArrayOfWritePointers();
                
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                // Divide into bands
                _mbProcessor.processSample(data[ch][sample], ch);
                
                auto rateDivide = static_cast<int> (_resample.getNextValue());
                
                // Dry signal
                float _drySignal = data[ch][sample];
                
                // Reduce bit depth
                float totalQLevels = std::powf(2, _bitDepth.getNextValue());
                
                // Wet signals
                float lowBand = _mbProcessor.getLowBand() * _lowMix.getNextValue();
                float lowMidBand = _mbProcessor.getLowMidBand() * _lowMidMix.getNextValue();
                float midBand = _mbProcessor.getMidBand() * _midMix.getNextValue();
                float highBand = _mbProcessor.getHighBand() * _highMix.getNextValue();
                float _wetSignal = lowBand + lowMidBand + midBand + highBand;
                
                _wetSignal = 0.5 * _wetSignal + 0.5;
                
                _wetSignal = totalQLevels * _wetSignal;
                
                _wetSignal = (std::round(_wetSignal) / totalQLevels) * 2 - 1;
                
                float remainder = std::fmodf(_wetSignal, 1.0 / totalQLevels);
                
                _wetSignal -= remainder;
                
                // Remove content above nyquist of the resampler
                _leftAliasFilter.setCutoffFrequency(_resample.getNextValue() * 882 * 0.5);
                _rightAliasFilter.setCutoffFrequency(_resample.getNextValue() * 882 * 0.5);
                
                if (_resample.getNextValue() > 49)
                {
                    if (ch == 0)
                    {
                        _wetSignal = _leftAliasFilter.processSample(ch, _wetSignal);
                    }
                    
                    else if (ch == 1)
                    {
                        _wetSignal = _rightAliasFilter.processSample(ch, _wetSignal);
                    }
                }
                
                // Resampler
                if (rateDivide > 1)
                {
                    if (sample % rateDivide != 0)
                    {
                        _wetSignal = data[ch][sample - sample % rateDivide];
                    }
                }
                
                data[ch][sample] = (1.0 - _mix.getNextValue()) * _drySignal + _wetSignal * _mix.getNextValue();
            }
        }
    }
    
    void setBitDepth(SampleType newBitDepth);
    void setResampledRate(SampleType newResampleRate);
    
    void toggleBand1(bool bandOn);
    void toggleBand2(bool bandOn);
    void toggleBand3(bool bandOn);
    void toggleBand4(bool bandOn);
    
    void setLowCutoff(SampleType newCutoff);
    void setMidCutoff(SampleType newCutoff);
    void setHighCutoff(SampleType newCutoff);
    
    void setLowMix(SampleType newMix);
    void setLowMidMix(SampleType newMix);
    void setMidMix(SampleType newMix);
    void setHighMix(SampleType newMix);
    
    void setMasterMix(SampleType newMix);
    
private:
    juce::SmoothedValue<float> _drive;
    juce::SmoothedValue<float> _bitDepth;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    juce::SmoothedValue<float> _resample;
    
    juce::SmoothedValue<float> _lowMix;
    juce::SmoothedValue<float> _lowMidMix;
    juce::SmoothedValue<float> _midMix;
    juce::SmoothedValue<float> _highMix;
    
    float _currentSampleRate;
    int _previousSample = 0;
    float _rateDivide;
    
    juce::NormalisableRange<float> _bitRateRange;
    
    viator_dsp::MultiBandProcessor<float> _mbProcessor;
    juce::dsp::LinkwitzRileyFilter<float> _leftAliasFilter;
    juce::dsp::LinkwitzRileyFilter<float> _rightAliasFilter;
};
}
#endif /* Marauder_h */

