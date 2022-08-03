#include "Globals.h"

juce::Colour m_bgColor;
juce::Colour m_bgLighterColor;
juce::Colour m_textAccentColor;
juce::Colour m_bgTransparent;
juce::Colour m_mainCompColor = juce::Colour::fromRGB(168, 132, 222);
juce::Colour m_mainCompFillColor;
juce::Colour m_textColor = juce::Colours::whitesmoke;

/** ============== Audio Parameters ============== */

const juce::String presetID = "presets";
const juce::String presetName = "Presets";

// IO
const juce::String inputID = "input";
const juce::String inputName = "Input";

const juce::String outputID = "output";
const juce::String outputName = "Output";

const juce::String phaseID = "phase";
const juce::String phaseName = "Phase";

const juce::String hqID = "hq";
const juce::String hqName = "High Quality";

// Color Menu
const juce::String colorID   = "colorMenu";
const juce::String colorName = "Color Theme";

// Bands
const juce::String band1ID = "band 1";
const juce::String band1Name = "Band 1";

const juce::String band2ID = "band 2";
const juce::String band2Name = "Band 2";

const juce::String band3ID = "band 3";
const juce::String band3Name = "Band 3";

const juce::String band4ID = "band 4";
const juce::String band4Name = "Band 4";

const juce::String band1MixID = "band 1 mix";
const juce::String band1MixName  = "Band 1 Mix";

const juce::String band2MixID = "band 2 mix";
const juce::String band2MixName  = "Band 2 Mix";

const juce::String band3MixID = "band 3 mix";
const juce::String band3MixName  = "Band 3 Mix";

const juce::String band4MixID = "band 4 mix";
const juce::String band4MixName  = "Band 4 Mix";

const juce::String band1CutoffID = "band 1 cutoff";
const juce::String band1CutoffName = "Band 1 Cutoff";

const juce::String band2CutoffID = "band 2 cutoff";
const juce::String band2CutoffName = "Band 2 Cutoff";

const juce::String band3CutoffID = "band 3 cutoff";
const juce::String band3CutoffName = "Band 3 Cutoff";

extern const juce::String bitDepth1ID = "bitDepth1";
extern const juce::String bitDepth1Name = "Bit Depth 1";

extern const juce::String resampleRateID = "resampleRate";
extern const juce::String resampleRateName = "Resample Rate";

extern const juce::String driveID = "drive";
extern const juce::String driveName = "Drive";

extern const juce::String masterMixID = "masterMix";
extern const juce::String masterMixName = "Master Mix";
