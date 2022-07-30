/*
  ==============================================================================

    MultiBandProcessor.cpp
    Created: 29 Jul 2022 7:24:58pm
    Author:  Landon Viator

  ==============================================================================
*/

#include "MultiBandProcessor.h"

template <typename SampleType>
MultiBandProcessor<SampleType>::MultiBandProcessor()
{
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::prepare(const juce::dsp::ProcessSpec& spec) noexcept
{
    _currentSampleRate = spec.sampleRate;
    
    _lowBandsFilter.prepare(spec);
    _lowBandsFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    _lowBandsFilter.setCutoffFrequency(100.0);
    
    _midBandsFilter.prepare(spec);
    _midBandsFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    _midBandsFilter.setCutoffFrequency(500.0);
    
    _highBandsFilter.prepare(spec);
    _highBandsFilter.setType(juce::dsp::LinkwitzRileyFilter<float>::Type::highpass);
    _highBandsFilter.setCutoffFrequency(5000.0);
    
    reset();
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::toggleBand1(bool bandOn)
{
    _band1 = bandOn;
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::toggleBand2(bool bandOn)
{
    _band2 = bandOn;
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::toggleBand3(bool bandOn)
{
    _band3 = bandOn;
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::toggleBand4(bool bandOn)
{
    _band4 = bandOn;
}

template <typename SampleType>
void MultiBandProcessor<SampleType>::reset() noexcept
{
    if (_currentSampleRate > 0)
    {
        _mix.reset(_currentSampleRate, 0.02);
        _mix.setTargetValue(1.0);
        _output.reset(_currentSampleRate, 0.02);
        _output.setTargetValue(0.0);
    }
}


template <typename SampleType>
void MultiBandProcessor<SampleType>::setOutput(SampleType newOutput)
{
   _output.setTargetValue(newOutput);
}

template class MultiBandProcessor<float>;
template class MultiBandProcessor<double>;
