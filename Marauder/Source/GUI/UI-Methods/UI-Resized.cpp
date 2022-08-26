#include "../../PluginEditor.h"

void MarauderAudioProcessorEditor::uiResized()
{
    // Position header
    const auto headerBottomMargin = getHeight() * 0.08;
    _headerComp.setBounds(0, 0, getWidth(), headerBottomMargin);
    
    // Only show settings page on resize if the toggle is on
    _settingsPage.setVisible(_headerComp.showSettings());
    _settingsPage.setEnabled(_headerComp.showSettings());
    if (_headerComp.showSettings())
    {
        _settingsPage.setBounds(getWidth() * 0.7f, getHeight() * 0.08f, getWidth() * 0.3f, getHeight());
    }
    
    // IO Module
    _ioComp.setBounds(0, headerBottomMargin, getWidth() * 0.25, getHeight() - headerBottomMargin);
    _ioComp.resized();
    
    // Delay Module
    _delayComp.setBounds(_ioComp.getX() + _ioComp.getWidth(), headerBottomMargin, getWidth() * 0.18, getHeight() - headerBottomMargin);
    _delayComp.resized();
    
    // MB Module
    _multibandComp.setBounds(_delayComp.getX() + _delayComp.getWidth(), headerBottomMargin, getWidth() * 0.25, getHeight() - headerBottomMargin);
    _multibandComp.resized();
    
    // Bit Crusher Module
    _bitCrusherComp.setBounds(_multibandComp.getX() + _multibandComp.getWidth() * 1.12, _multibandComp.getY() * 1.27, getWidth() * 0.25, _multibandComp.getHeight() * 0.6);
    _bitCrusherComp.resized();
    
    
    // Filter Module
    _filterComp.setBounds(_bitCrusherComp.getX(),
                          _bitCrusherComp.getY() + _bitCrusherComp.getHeight() * 1.03,
                          getWidth() * 0.28,
                          getHeight() * 0.35);
    _filterComp.resized();
    
    // Save Plugin Window Size
    saveWindowSize();
}

