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
                
                // Wet signals
                float lowBand = _mbProcessor.getLowBand();
                float lowMidBand = _mbProcessor.getLowMidBand();
                float midBand = _mbProcessor.getMidBand();
                float highBand = _mbProcessor.getHighBand();
                
                float _wetLowSignal = lowBand * 0.5 + 0.5;
                float _wetLowMidSignal = lowMidBand * 0.5 + 0.5;
                float _wetMidSignal = midBand * 0.5 + 0.5;
                float _wetHighSignal = highBand * 0.5 + 0.5;
                
                _wetLowSignal = totalQLevels * _wetLowSignal;
                _wetLowMidSignal = totalQLevels * _wetLowMidSignal;
                _wetMidSignal = totalQLevels * _wetMidSignal;
                _wetHighSignal = totalQLevels * _wetHighSignal;

                _wetLowSignal = (std::round(_wetLowSignal) / totalQLevels) * 2 - 1;
                _wetLowMidSignal = (std::round(_wetLowMidSignal) / totalQLevels) * 2 - 1;
                _wetMidSignal = (std::round(_wetMidSignal) / totalQLevels) * 2 - 1;
                _wetHighSignal = (std::round(_wetHighSignal) / totalQLevels) * 2 - 1;

                float lowRemainder = std::fmodf(_wetLowSignal, 1.0 / totalQLevels);
                float lowMidRemainder = std::fmodf(_wetLowMidSignal, 1.0 / totalQLevels);
                float midRemainder = std::fmodf(_wetMidSignal, 1.0 / totalQLevels);
                float highRemainder = std::fmodf(_wetHighSignal, 1.0 / totalQLevels);

                _wetLowSignal -= lowRemainder;
                _wetLowMidSignal -= lowMidRemainder;
                _wetMidSignal -= midRemainder;
                _wetHighSignal -= highRemainder;
                
                // Resampler
                if (rateDivide > 1)
                {
                    if (sample % rateDivide != 0)
                    {
                        _wetLowSignal = data[ch][sample - sample % rateDivide] * 0.25;
                        _wetLowMidSignal = data[ch][sample - sample % rateDivide] * 0.25;
                        _wetMidSignal = data[ch][sample - sample % rateDivide] * 0.25;
                        _wetHighSignal = data[ch][sample - sample % rateDivide] * 0.25;
                    }
                }
                
                // Lofi distortion
                if (_wetLowSignal < 0)
                {
                    _wetLowSignal *= juce::jmap(_drive.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
                }
                
                if (_wetLowMidSignal < 0)
                {
                    _wetLowMidSignal *= juce::jmap(_drive.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
                }
                
                if (_wetMidSignal < 0)
                {
                    _wetMidSignal *= juce::jmap(_drive.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
                }
                
                if (_wetHighSignal < 0)
                {
                    _wetHighSignal *= juce::jmap(_drive.getNextValue(), 0.0f, 20.0f, 1.0f, -1.0f);
                }
                
                float lowOutput = (1.0 - _lowMix.getNextValue()) * lowBand + _wetLowSignal * _lowMix.getNextValue();
                float lowMidOutput = (1.0 - _lowMidMix.getNextValue()) * lowMidBand + _wetLowMidSignal * _lowMidMix.getNextValue();
                float midOutput = (1.0 - _midMix.getNextValue()) * midBand + _wetMidSignal * _midMix.getNextValue();
                float highOutput = (1.0 - _highMix.getNextValue()) * highBand + _wetHighSignal * _highMix.getNextValue();
                
                data[ch][sample] = (1.0 - _mix.getNextValue()) * data[ch][sample] +
                (lowOutput + lowMidOutput + midOutput + highOutput) * _mix.getNextValue();
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

