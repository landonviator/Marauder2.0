#include "Marauder.h"

template <typename SampleType>
viator_dsp::Marauder<SampleType>::Marauder()
{
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    _currentSampleRate = spec.sampleRate;
    _mbProcessor.prepare(spec);
    
    reset();
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::reset() noexcept
{
    if (_currentSampleRate > 0)
    {
        _drive.reset(_currentSampleRate, 0.02);
        _drive.setTargetValue(1.0);
        _resample.reset(_currentSampleRate, 0.02);
        _resample.setTargetValue(50.0);
        _mix.reset(_currentSampleRate, 0.02);
        _mix.setTargetValue(1.0);
        _output.reset(_currentSampleRate, 0.02);
        _output.setTargetValue(0.0);
    }
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setBitDepth(SampleType newBitDepth)
{
    _bitDepth = newBitDepth;
    
    // Reduce bit depth
    totalQLevels = viator_utils::FastMath::fastPow(2, _bitDepth);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setResampledRate(SampleType newResampleRate)
{
    _resample.setTargetValue(newResampleRate);
    rateDivide = static_cast<int> (newResampleRate);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::toggleBand1(bool bandOn)
{
    _mbProcessor.toggleBand1(bandOn);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::toggleBand2(bool bandOn)
{
    _mbProcessor.toggleBand2(bandOn);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::toggleBand3(bool bandOn)
{
    _mbProcessor.toggleBand3(bandOn);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::toggleBand4(bool bandOn)
{
    _mbProcessor.toggleBand4(bandOn);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setLowCutoff(SampleType newCutoff)
{
    _mbProcessor.setLowCutoff(newCutoff);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setMidCutoff(SampleType newCutoff)
{
    _mbProcessor.setMidCutoff(newCutoff);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setHighCutoff(SampleType newCutoff)
{
    _mbProcessor.setHighCutoff(newCutoff);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setLowMix(SampleType newMix)
{
    _lowMix.setTargetValue(newMix);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setLowMidMix(SampleType newMix)
{
    _lowMidMix.setTargetValue(newMix);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setMidMix(SampleType newMix)
{
    _midMix.setTargetValue(newMix);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setHighMix(SampleType newMix)
{
    _highMix.setTargetValue(newMix);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setDrive(SampleType newDrive)
{
    _drive.setTargetValue(newDrive);
}

template <typename SampleType>
void viator_dsp::Marauder<SampleType>::setMasterMix(SampleType newMix)
{
    _mix.setTargetValue(newMix);
}

template class viator_dsp::Marauder<float>;
template class viator_dsp::Marauder<double>;
