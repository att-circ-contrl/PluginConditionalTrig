#ifndef TTLCONDTRIGCONSTANTS_H_DEFINED
#define TTLCONDTRIGCONSTANTS_H_DEFINED


// Magic constants for parameter indices.
// Making this an enum isn't any cleaner, since we aren't guaranteed sequential values.
#define TTLCONDTRIG_PARAM_IS_ENABLED 0
#define TTLCONDTRIG_PARAM_CHAN_IDX 1
#define TTLCONDTRIG_PARAM_BIT_IDX 2
#define TTLCONDTRIG_PARAM_DELAY_MIN 3
#define TTLCONDTRIG_PARAM_DELAY_MAX 4
#define TTLCONDTRIG_PARAM_SUSTAIN 5
#define TTLCONDTRIG_PARAM_DEADTIME 6
#define TTLCONDTRIG_PARAM_DEGLITCH 7
#define TTLCONDTRIG_PARAM_OUTSENSE 8
// This should be equal to or greater than the number of per-channel parameters.
#define TTLCONDTRIG_PARAM_STRIDE 10

#define TTLCONDTRIG_PARAM_STRIDE_INPUT TTLCONDTRIG_PARAM_STRIDE
#define TTLCONDTRIG_PARAM_STRIDE_OUTPUT (TTLCONDTRIG_INPUTS * TTLCONDTRIG_PARAM_STRIDE_INPUT)


// Magic constant for display refresh.
#define TTLCONDTRIG_DISPLAY_REFRESH_MS 20


// Magic constants for GUI geometry.


// Magic constants for GUI colours.

#define COLOUR_TRANSPARENT juce::Colours::transparentWhite

#define WRENCH_FOREGROUND juce::Colours::black
#define WRENCH_BACKGROUND juce::Colours::white

#define CONN_FOREGROUND juce::Colours::lightyellow
#define CONN_BACKGROUND juce::Colours::transparentWhite
#define DISCONN_FOREGROUND juce::Colours::black
#define DISCONN_BACKGROUND juce::Colours::transparentWhite

#define LAMP_BACKGROUND juce::Colours::transparentWhite
#define LAMP_OUTLINE juce::Colours::black
#define LAMP_ON_FILL juce::Colours::lightgreen
#define LAMP_ON_HIGHLIGHT juce::Colours::white
#define LAMP_OFF_FILL juce::Colours::darkgrey
#define LAMP_OFF_HIGHLIGHT juce::Colours::grey


#endif

// This is the end of the file.
