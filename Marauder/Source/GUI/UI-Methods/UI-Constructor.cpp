#include "../../PluginEditor.h"

void MarauderAudioProcessorEditor::uiConstructor()
{
    // Init the plugin window size logic
    initWindow();
    
    // Settings Page and Plugin Theme init
    addAndMakeVisible(_settingsPage);
    _settingsPage.setAlwaysOnTop(true);
    setPluginTheme(static_cast<Theme>(_settingsPage.getPluginTheme()));
    
    // Header
    addAndMakeVisible(_headerComp);
    
    // IO Module
    addAndMakeVisible(_ioComp);
    
    // MB Module
    addAndMakeVisible(_multibandComp);
    
    // Bit Crusher Module
    addAndMakeVisible(_bitCrusherComp);
    
    // Delay Module
    addAndMakeVisible(_delayComp);
    
    // Filter Module
    addAndMakeVisible(_filterComp);
}

