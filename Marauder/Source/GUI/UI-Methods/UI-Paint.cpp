#include "../../PluginEditor.h"

void MarauderAudioProcessorEditor::uiPaint(juce::Graphics &g)
{
    // Init Plugin Theme First
    setPluginTheme(static_cast<Theme>(_settingsPage.getPluginTheme()));
    
    // Init Background with or without Gradient
    if (_settingsPage.getGradientState())
    {
        g.setGradientFill(juce::ColourGradient::vertical(m_bgColor.darker(0.75f), getHeight(), m_bgColor.brighter(0.02f), getHeight() * 0.4));
        g.fillRect(getLocalBounds());
    }
    
    else
    {
        g.fillAll(m_bgColor);
    }
    
    _settingsPage.repaint();
    
    // Update IO Colors
    _ioComp.repaint();
    
    // Title/version
    g.setColour(m_mainCompColor.withAlpha(0.8f));
    g.setFont(juce::Font("Helvetica", getWidth() * 0.015, juce::Font::FontStyleFlags::plain));
    g.drawText("Plugin v0.0.1", getLocalBounds().removeFromBottom(getHeight() * 0.075), juce::Justification::centred);
    
    g.setFont(juce::Font("Helvetica",
                         getWidth() * 0.015,
                         juce::Font::FontStyleFlags::bold));
    
    g.drawText("Input Output Module",
               getWidth() * -0.12,
               getHeight() * 0.07,
               getWidth() * 0.5,
               getHeight() * 0.1,
               juce::Justification::centred);
    
    g.drawText("String Module",
               getWidth() * 0.09,
               getHeight() * 0.07,
               getWidth() * 0.5,
               getHeight() * 0.1,
               juce::Justification::centred);
    
    g.drawText("Multiband Bit Crusher",
               getWidth() * 0.44,
               getHeight() * 0.07,
               getWidth() * 0.5,
               getHeight() * 0.1,
               juce::Justification::centred);
}
