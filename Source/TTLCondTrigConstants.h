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

#define TTLCONDTRIG_YSIZE 16
#define TTLCONDTRIG_YGAP 4
#define TTLCONDTRIG_XGAP 8

#define TTLCONDTRIG_GLOBAL_YOFFSET 24
#define TTLCONDTRIG_GLOBAL_XOFFSET 4

#define TTLCONDTRIG_LAMP_SIZE TTLCONDTRIG_YSIZE
#define TTLCONDTRIG_WRENCH_SIZE TTLCONDTRIG_YSIZE
#define TTLCONDTRIG_CONN_SIZE TTLCONDTRIG_YSIZE
#define TTLCONDTRIG_TAB_XSIZE 16

#define TTLCONDTRIG_LABEL_XSIZE 60
#define TTLCONDTRIG_BOOLBUTTON_XSIZE 20
#define TTLCONDTRIG_BOOLBUTTON_YSIZE (TTLCONDTRIG_YSIZE * 2 + TTLCONDTRIG_YGAP)

#define TTLCONDTRIG_INROW_XSIZE (TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LAMP_SIZE)
#define TTLCONDTRIG_INPANEL_XSIZE (TTLCONDTRIG_XGAP + TTLCONDTRIG_INROW_XSIZE + TTLCONDTRIG_XGAP)
#define TTLCONDTRIG_INPANEL_YSIZE (TTLCONDTRIG_YSIZE * 5 + TTLCONDTRIG_YGAP * 6)

#define TTLCONDTRIG_OUTROW_XSIZE (TTLCONDTRIG_XGAP + TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_TAB_XSIZE)
#define TTLCONDTRIG_OUTPANEL_XSIZE (TTLCONDTRIG_OUTROW_XSIZE+TTLCONDTRIG_XGAP)
// There's an empty row above the tabs to make this the same height as the input panel.
#define TTLCONDTRIG_OUTPANEL_YSIZE (TTLCONDTRIG_YSIZE * 5 + TTLCONDTRIG_YGAP * 6)


// Magic constants for GUI colours.

#define COLOUR_TRANSPARENT juce::Colours::transparentWhite

#define COLOUR_BOGUS juce::Colours::orange

#define WRENCH_FOREGROUND juce::Colours::black
#define WRENCH_BACKGROUND juce::Colours::transparentWhite

#define CONN_FOREGROUND juce::Colours::lightyellow
#define CONN_BACKGROUND juce::Colours::transparentWhite
#define DISCONN_FOREGROUND juce::Colours::black
#define DISCONN_BACKGROUND juce::Colours::transparentWhite

#define LAMP_BACKGROUND juce::Colours::transparentWhite
#define LAMP_OUTLINE juce::Colours::black
#define LAMP_GREEN_FILL juce::Colours::lightgreen
#define LAMP_GREEN_HIGHLIGHT juce::Colours::white
#define LAMP_AMBER_FILL juce::Colours::gold
#define LAMP_AMBER_HIGHLIGHT juce::Colours::white
#define LAMP_OFF_FILL juce::Colours::darkgrey
#define LAMP_OFF_HIGHLIGHT juce::Colours::grey

// This needs as many entries as we have outputs.
#define OUTPUT_COLOUR_LIST { juce::Colours::darksalmon, juce::Colours::goldenrod, juce::Colours::seagreen, juce::Colours::steelblue }


#endif

// This is the end of the file.
