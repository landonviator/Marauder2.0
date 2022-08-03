#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MarauderAudioProcessor::MarauderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, _treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
, _oversamplingModule(2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR)
#endif
{
    
    variableTree.setProperty("glowslider", 1, nullptr);
    variableTree.setProperty("gradienttoggle", 1, nullptr);
    variableTree.setProperty("glowtoggle", 0, nullptr);
    variableTree.setProperty("gradienttoggle", 1, nullptr);
    
    _treeState.addParameterListener(inputID, this);
    _treeState.addParameterListener(outputID, this);
    _treeState.addParameterListener(phaseID, this);
    _treeState.addParameterListener(hqID, this);
    _treeState.addParameterListener(presetID, this);
    
    // Bands
    _treeState.addParameterListener(band1ID, this);
    _treeState.addParameterListener(band2ID, this);
    _treeState.addParameterListener(band3ID, this);
    _treeState.addParameterListener(band4ID, this);
    _treeState.addParameterListener(band1MixID, this);
    _treeState.addParameterListener(band2MixID, this);
    _treeState.addParameterListener(band3MixID, this);
    _treeState.addParameterListener(band4MixID, this);
    _treeState.addParameterListener(band1CutoffID, this);
    _treeState.addParameterListener(band2CutoffID, this);
    _treeState.addParameterListener(band3CutoffID, this);
    
    // Crusher
    _treeState.addParameterListener(bitDepth1ID, this);
    _treeState.addParameterListener(resampleRateID, this);
    _treeState.addParameterListener(driveID, this);
    _treeState.addParameterListener(masterMixID, this);
}

MarauderAudioProcessor::~MarauderAudioProcessor()
{
    _treeState.removeParameterListener(inputID, this);
    _treeState.removeParameterListener(outputID, this);
    _treeState.removeParameterListener(phaseID, this);
    _treeState.removeParameterListener(hqID, this);
    _treeState.removeParameterListener(presetID, this);
    
    // Bands
    _treeState.removeParameterListener(band1ID, this);
    _treeState.removeParameterListener(band2ID, this);
    _treeState.removeParameterListener(band3ID, this);
    _treeState.removeParameterListener(band4ID, this);
    _treeState.removeParameterListener(band1MixID, this);
    _treeState.removeParameterListener(band2MixID, this);
    _treeState.removeParameterListener(band3MixID, this);
    _treeState.removeParameterListener(band4MixID, this);
    _treeState.removeParameterListener(band1CutoffID, this);
    _treeState.removeParameterListener(band2CutoffID, this);
    _treeState.removeParameterListener(band3CutoffID, this);
    
    // Crusher
    _treeState.removeParameterListener(bitDepth1ID, this);
    _treeState.removeParameterListener(resampleRateID, this);
    _treeState.removeParameterListener(driveID, this);
    _treeState.removeParameterListener(masterMixID, this);
}

juce::AudioProcessorValueTreeState::ParameterLayout MarauderAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Presets
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID { presetID, 1 }, presetName, 0, 9, 0));
    
    // IO
    auto ioRange = juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f);
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { inputID, 1 }, inputName, ioRange, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { outputID, 1 }, outputName, ioRange, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { phaseID, 1 }, phaseName, false));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { hqID, 1 }, hqName, false));
    
    // Colors
    params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID { colorID, 1 }, colorName, 0, 9, 0));
    
    // Bands
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { band1ID, 1 }, band1Name, true));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { band2ID, 1 }, band2Name, true));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { band3ID, 1 }, band3Name, true));
    params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { band4ID, 1 }, band4Name, true));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band1MixID, 1 }, band1MixName, 0.0f, 1.0f, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band2MixID, 1 }, band2MixName, 0.0f, 1.0f, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band3MixID, 1 }, band3MixName, 0.0f, 1.0f, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band4MixID, 1 }, band4MixName, 0.0f, 1.0f, 1.0f));
    
    auto cutoffRange = juce::NormalisableRange<float> (20.0f, 20000.0f, 0.1f, 0.2f);
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band1CutoffID, 1 }, band1CutoffName, cutoffRange, 100.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band2CutoffID, 1 }, band2CutoffName, cutoffRange, 500.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { band3CutoffID, 1 }, band3CutoffName, cutoffRange, 2000.0f));
    
    // Crusher
    auto bitDepthRange = juce::NormalisableRange<float> (1.0f, 16.0f, 1.0f);
    bitDepthRange.setSkewForCentre(5.0);
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { bitDepth1ID, 1 }, bitDepth1Name, bitDepthRange, 24.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { resampleRateID, 1 }, resampleRateName, juce::NormalisableRange<float> (1.0f, 50.0f, 1.0f), 50.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { driveID, 1 }, driveName, 0.0f, 20.0f, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { masterMixID, 1 }, masterMixName, 0.0f, 1.0f, 1.0f));
    
    return { params.begin(), params.end() };
}

void MarauderAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    if (parameterID == hqID)
    {
        hqToggle = static_cast<bool>(_treeState.getRawParameterValue(hqID)->load());
            
        // Adjust samplerate of filters when oversampling
        if (hqToggle)
        {
            spec.sampleRate = getSampleRate() * _oversamplingModule.getOversamplingFactor();
            _inputGainModule.prepare(spec);
            _outputGainModule.prepare(spec);
            _marauder.prepare(spec);
        }

        else
        {
            spec.sampleRate = getSampleRate();
            _inputGainModule.prepare(spec);
            _outputGainModule.prepare(spec);
            _marauder.prepare(spec);
        }
    }
            
    updateParameters();
}

void MarauderAudioProcessor::updateParameters()
{
    _inputGainModule.setGainDecibels(_treeState.getRawParameterValue(inputID)->load());
    _outputGainModule.setGainDecibels(_treeState.getRawParameterValue(outputID)->load());
    
    _marauder.toggleBand1(_treeState.getRawParameterValue(band1ID)->load());
    _marauder.toggleBand2(_treeState.getRawParameterValue(band2ID)->load());
    _marauder.toggleBand3(_treeState.getRawParameterValue(band3ID)->load());
    _marauder.toggleBand4(_treeState.getRawParameterValue(band4ID)->load());
    _marauder.setLowCutoff(_treeState.getRawParameterValue(band1CutoffID)->load());
    _marauder.setMidCutoff(_treeState.getRawParameterValue(band2CutoffID)->load());
    _marauder.setHighCutoff(_treeState.getRawParameterValue(band3CutoffID)->load());
    _marauder.setLowMix(_treeState.getRawParameterValue(band1MixID)->load());
    _marauder.setLowMidMix(_treeState.getRawParameterValue(band2MixID)->load());
    _marauder.setMidMix(_treeState.getRawParameterValue(band3MixID)->load());
    _marauder.setHighMix(_treeState.getRawParameterValue(band4MixID)->load());
    
    _marauder.setBitDepth(_treeState.getRawParameterValue(bitDepth1ID)->load());
    auto rate = juce::jmap(_treeState.getRawParameterValue(resampleRateID)->load(), 1.0f, 50.0f, 50.0f, 1.0f);
    _marauder.setResampledRate(rate);
    
    _aliasFilter.setCutoff(_treeState.getRawParameterValue(resampleRateID)->load() * 882 * 0.5);
    _artifactFilter.setCutoff(_treeState.getRawParameterValue(resampleRateID)->load() * 882 * 0.5);
    _marauder.setDrive(_treeState.getRawParameterValue(driveID)->load());
    _marauder.setMasterMix(_treeState.getRawParameterValue(masterMixID)->load());
}

//==============================================================================
const juce::String MarauderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MarauderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MarauderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MarauderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MarauderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MarauderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MarauderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MarauderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MarauderAudioProcessor::getProgramName (int index)
{
    return {};
}

void MarauderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MarauderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize spec for dsp modules
    hqToggle = static_cast<bool>(_treeState.getRawParameterValue(hqID)->load());
        
    // Oversampling
    _oversamplingModule.initProcessing(samplesPerBlock);
    
    if (hqToggle)
    {
        spec.sampleRate = sampleRate * _oversamplingModule.getOversamplingFactor();
    }
        
    else
    {
        spec.sampleRate = sampleRate;
    }
    
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    
    // DSP
    _inputGainModule.prepare(spec);
    _outputGainModule.prepare(spec);
    _marauder.prepare(spec);
    _aliasFilter.prepare(spec);
    _artifactFilter.prepare(spec);
    
    // Init params
    updateParameters();
}

void MarauderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MarauderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MarauderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

//    if (hqToggle)
//    {
//        hqProcessBlock(buffer);
//    }
//
//    else
//    {
//        normalProcessBlock(buffer);
//    }
    
    _aliasFilter.processBuffer(buffer);
    _marauder.processBuffer(buffer);
    _artifactFilter.processBuffer(buffer);
}

void MarauderAudioProcessor::hqProcessBlock(juce::AudioBuffer<float> &buffer)
{
    // Blocks
    juce::dsp::AudioBlock<float> block {buffer};
    juce::dsp::AudioBlock<float> upSampledBlock (buffer);

    // Up sample
    upSampledBlock = _oversamplingModule.processSamplesUp(upSampledBlock);
    
    // Input
    _inputGainModule.process(juce::dsp::ProcessContextReplacing<float>(upSampledBlock));
    
    // Output
    _outputGainModule.process(juce::dsp::ProcessContextReplacing<float>(upSampledBlock));
    
    // Apply phase invert
    if (_treeState.getRawParameterValue(phaseID)->load())
    {
        viator_utils::utils::invertBlock(upSampledBlock);
    }
    
    _oversamplingModule.processSamplesDown(block);
    
    // Clip output
    viator_utils::utils::hardClipBlock(block);
}

void MarauderAudioProcessor::normalProcessBlock(juce::AudioBuffer<float> &buffer)
{
    // Block
    juce::dsp::AudioBlock<float> block {buffer};

    // Input
    _inputGainModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Output
    _outputGainModule.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // Apply phase invert
    if (_treeState.getRawParameterValue(phaseID)->load())
    {
        viator_utils::utils::invertBlock(block);
    }
    
    // Clip output
    viator_utils::utils::hardClipBlock(block);
}

//==============================================================================
bool MarauderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MarauderAudioProcessor::createEditor()
{
    //return new MarauderAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void MarauderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save params
    _treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    _treeState.state.writeToStream (stream);
}

void MarauderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Recall params
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        _treeState.state = tree;
        
        // Window Size
        windowWidth = variableTree.getProperty("width");
        windowHeight = variableTree.getProperty("height");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MarauderAudioProcessor();
}
