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
                
                // Dry signal
                float _drySignal = data[ch][sample];
                
                // Wet signals
                float lowBand = _mbProcessor.getLowBand() * _lowMix.getNextValue();
                float lowMidBand = _mbProcessor.getLowMidBand() * _lowMidMix.getNextValue();
                float midBand = _mbProcessor.getMidBand() * _midMix.getNextValue();
                float highBand = _mbProcessor.getHighBand() * _highMix.getNextValue();
                _wetSignal = (lowBand + lowMidBand + midBand + highBand) * 0.5 + 0.5;
                
                _wetSignal = totalQLevels * _wetSignal;

                _wetSignal = (std::round(_wetSignal) / totalQLevels) * 2 - 1;

                remainder = std::fmodf(_wetSignal, 1.0 / totalQLevels);

                _wetSignal -= remainder;
                
                // Resampler
                if (rateDivide > 1)
                {
                    if (sample % rateDivide != 0)
                    {
                        _wetSignal = data[ch][sample - sample % rateDivide];
                    }
                }
                
                // Lofi distortion
                if (_wetSignal < 0)
                {
                    _wetSignal *= juce::jmap(_drive.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
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
    
    void setDrive(SampleType newDrive);
    void setMasterMix(SampleType newMix);
    
private:
    juce::SmoothedValue<float> _drive;
    int _bitDepth;
    juce::SmoothedValue<float> _mix;
    juce::SmoothedValue<float> _output;
    juce::SmoothedValue<float> _resample;
    
    juce::SmoothedValue<float> _lowMix;
    juce::SmoothedValue<float> _lowMidMix;
    juce::SmoothedValue<float> _midMix;
    juce::SmoothedValue<float> _highMix;
    
    int rateDivide;
    float totalQLevels;
    float _wetSignal;
    float remainder;
    
    float _currentSampleRate;
    float _rateDivide;
    
    juce::NormalisableRange<float> _bitRateRange;
    
    viator_dsp::MultiBandProcessor<float> _mbProcessor;
};
}
#endif /* Marauder_h */

