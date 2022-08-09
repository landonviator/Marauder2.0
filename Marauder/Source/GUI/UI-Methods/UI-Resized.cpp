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
    
    const int padding = 4;
    
    // IO Module
    _ioComp.setBounds(0, headerBottomMargin, getWidth() * 0.25, getHeight() - headerBottomMargin);
    _ioComp.resized();
    
    // MB Module
    _multibandComp.setBounds(_ioComp.getX() + _ioComp.getWidth() + padding, headerBottomMargin + padding, getWidth() * 0.2, getHeight() - headerBottomMargin * 2);
    _multibandComp.resized();
    
    // Bit Crusher Module
    _bitCrusherComp.setBounds(_multibandComp.getX() + _multibandComp.getWidth() + padding, headerBottomMargin + padding, getWidth() * 0.35, _multibandComp.getHeight() * 0.66);
    _bitCrusherComp.resized();
    
    // Delay Module
    _delayComp.setBounds(_bitCrusherComp.getX(), _bitCrusherComp.getY() + _bitCrusherComp.getHeight() * 1.02, getWidth() * 0.35, _multibandComp.getHeight() * 0.33);
    _delayComp.resized();
    
    // Filter Module
    _filterComp.setBounds(_delayComp.getX() + _delayComp.getWidth(), _bitCrusherComp.getY(), getWidth() * 0.19, _multibandComp.getHeight());
    _filterComp.resized();
    
    // Save Plugin Window Size
    saveWindowSize();
}

