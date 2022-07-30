#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MarauderAudioProcessorEditor::MarauderAudioProcessorEditor (MarauderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, _headerComp(audioProcessor)
, _settingsPage(audioProcessor)
, _ioComp(audioProcessor, _settingsPage)
{
    uiConstructor();
}

MarauderAudioProcessorEditor::~MarauderAudioProcessorEditor()
{
    
}

//==============================================================================
void MarauderAudioProcessorEditor::paint (juce::Graphics& g)
{
    uiPaint(g);
}

void MarauderAudioProcessorEditor::resized()
{
    uiResized();
}


