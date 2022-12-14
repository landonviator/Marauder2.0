/*
  ==============================================================================

    SettingsPage.h
    Created: 13 Jun 2022 3:11:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../PluginProcessor.h"
#include "../../Parameters/Globals.h"

//==============================================================================
/*
*/
class SettingsPage  : public juce::Component
{
public:
    SettingsPage(MarauderAudioProcessor&);
    ~SettingsPage() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    enum class Theme
    {
        kDracula,
        kPrimeDark,
        kTokyoLofi,
        kWinter,
        kFoxy,
        kPurp,
        kDull
    };
    Theme getPluginTheme();
    bool getGlowState();
    bool getGradientState();
    float getCurrentGlowValue();
    bool getUIType();

private:
    
    MarauderAudioProcessor& audioProcessor;
    using buttonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    
    /** Menus */
    juce::ComboBox m_themeMenu;
    void setMenuProps(juce::ComboBox& menu);
    void setThemeMenuProps();
    
    /** Theme */
    Theme m_pluginTheme = Theme::kDracula;
    void setPluginTheme(Theme newTheme);
    
    /** =============== Buttons ===============*/
    
    // Glow
    juce::TextButton m_glowToggle;
    void setGlowButtonProps();
    juce::Slider m_glowSlider;
    void setGlowSliderProps();
    float _currentGlowValue = 1.0;
    
    // Gradient
    juce::TextButton m_gradientToggle;
    void setGradientToggleProps();
    
    // Discord link
    juce::TextButton m_discord;
    void setDiscordBtnProps();
    juce::HyperlinkButton m_discordLink;
    juce::URL m_discordUrl {"https://discord.gg/zcmjABjVn8"};
    
    // Patreon link
    juce::TextButton m_patreon;
    void setPatreonBtnProps();
    juce::HyperlinkButton m_patreonLink;
    juce::URL m_patreonUrl {"https://www.patreon.com/ViatorDSP?fan_landing=true"};
    
    // Twitch link
    juce::TextButton m_twitch;
    void setTwitchBtnProps();
    juce::HyperlinkButton m_twitchLink;
    juce::URL m_twitchUrl {"https://www.twitch.tv/dr_bruisin"};
    
    // UI Style
    juce::TextButton _skueomorphic;
    juce::TextButton _flat;
    std::unique_ptr<buttonAttachment> _skueomorphicAttach;
    std::unique_ptr<buttonAttachment> _flatAttach;
    
    void setTextButtonProps(juce::TextButton& button);
    
    std::vector<juce::TextButton*> buttons =
    {
        &m_glowToggle, &m_gradientToggle,
        &m_discord, &m_patreon, &m_twitch,
        &_skueomorphic, &_flat
    };
    
    /** Groups */
    juce::GroupComponent m_themeGroup;
    juce::GroupComponent _uiTypeGroup;
    void setGroupProps(juce::GroupComponent& group);
    std::vector<juce::GroupComponent*> groups =
    {
        &m_themeGroup, &_uiTypeGroup
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPage)
};
