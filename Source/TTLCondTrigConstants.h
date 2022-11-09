#ifndef TTLCONDTRIGCONSTANTS_H_DEFINED
#define TTLCONDTRIGCONSTANTS_H_DEFINED


// Magic constants for logic.
#define TTLCONDTRIG_CHANIDX_NONE (-1)
#define TTLCONDTRIG_BITIDX_NONE (-1)
// JUCE hates user-supplied ID values of 0, so shift combobox option ID values up a bit.
#define TTLCONDTRIG_COMBOBOX_OFFSET 10


// Magic constants for parameter indices.
// Making this an enum isn't any cleaner, since we aren't guaranteed sequential values.
// Configuration handled by the plugin.
#define TTLCONDTRIG_PARAM_IS_ENABLED 0
#define TTLCONDTRIG_PARAM_CHAN_IDX 1
#define TTLCONDTRIG_PARAM_BIT_IDX 2
#define TTLCONDTRIG_PARAM_WANT_ALL 3
// Configuration handled by the TTL logic widgets.
#define TTLCONDTRIG_PARAM_DELAY_MIN 10
#define TTLCONDTRIG_PARAM_DELAY_MAX 11
#define TTLCONDTRIG_PARAM_SUSTAIN 12
#define TTLCONDTRIG_PARAM_DEADTIME 13
#define TTLCONDTRIG_PARAM_DEGLITCH 14
#define TTLCONDTRIG_PARAM_INFEATURE 15
#define TTLCONDTRIG_PARAM_OUTSENSE 16
// This should be greater than the largest parameter identifier.
#define TTLCONDTRIG_PARAM_STRIDE 20

#define TTLCONDTRIG_PARAM_STRIDE_INPUT TTLCONDTRIG_PARAM_STRIDE
#define TTLCONDTRIG_PARAM_STRIDE_OUTPUT (TTLCONDTRIG_INPUTS * TTLCONDTRIG_PARAM_STRIDE_INPUT)


// Magic constant for display refresh.
#define TTLCONDTRIG_DISPLAY_REFRESH_MS 20


// Magic constants for GUI geometry.

// General.

#define TTLCONDTRIG_YSIZE 16
#define TTLCONDTRIG_YGAP 4
#define TTLCONDTRIG_XGAP 8

#define TTLCONDTRIG_GLOBAL_YOFFSET 24
#define TTLCONDTRIG_GLOBAL_XOFFSET 4

// Icons.

#define TTLCONDTRIG_LAMP_SIZE TTLCONDTRIG_YSIZE
#define TTLCONDTRIG_WRENCH_SIZE TTLCONDTRIG_YSIZE
#define TTLCONDTRIG_CONN_SIZE TTLCONDTRIG_YSIZE

// I/O panels.

#define TTLCONDTRIG_TAB_XSIZE 16
#define TTLCONDTRIG_LABEL_XSIZE 60
#define TTLCONDTRIG_BOOLBUTTON_XSIZE 40
#define TTLCONDTRIG_BOOLBUTTON_YSIZE (TTLCONDTRIG_YSIZE)

#define TTLCONDTRIG_INROW_XSIZE (TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LAMP_SIZE)
#define TTLCONDTRIG_INPANEL_XSIZE (TTLCONDTRIG_XGAP + TTLCONDTRIG_INROW_XSIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_BOOLBUTTON_XSIZE + TTLCONDTRIG_XGAP)
#define TTLCONDTRIG_INPANEL_YSIZE (TTLCONDTRIG_YSIZE * 5 + TTLCONDTRIG_YGAP * 6)

#define TTLCONDTRIG_OUTROW_XSIZE (TTLCONDTRIG_XGAP + TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP + TTLCONDTRIG_TAB_XSIZE)
#define TTLCONDTRIG_OUTPANEL_XSIZE (TTLCONDTRIG_OUTROW_XSIZE)
// There's an empty row above the tabs to make this the same height as the input panel.
#define TTLCONDTRIG_OUTPANEL_YSIZE (TTLCONDTRIG_YSIZE * 5 + TTLCONDTRIG_YGAP * 6)

#define TTLCONDTRIG_PLUGIN_XSIZE (TTLCONDTRIG_INPANEL_XSIZE + TTLCONDTRIG_OUTPANEL_XSIZE + TTLCONDTRIG_GLOBAL_XOFFSET*2)

// Config panels.
// NOTE - Maximum total size is about 360, for consistency with previous panels.

#define TTLCONDTRIG_CONFIGDONE_XSIZE 40
#define TTLCONDTRIG_CONFIGBANNER_XSIZE 180
#define TTLCONDTRIG_CONFIGLABEL_XSIZE 80
#define TTLCONDTRIG_CONFIGLABELNOTE_XSIZE 36

#define TTLCONDTRIG_CONFIGENABLELABEL_XSIZE 45
#define TTLCONDTRIG_CONFIGFEATURE_XSIZE 50
#define TTLCONDTRIG_CONFIGFEATURELABEL_XSIZE 35
#define TTLCONDTRIG_CONFIGBANK_XSIZE 60
#define TTLCONDTRIG_CONFIGBANKLABEL_XSIZE 60
#define TTLCONDTRIG_CONFIGBIT_XSIZE 30
#define TTLCONDTRIG_CONFIGBITLABEL_XSIZE 25

#define TTLCONDTRIG_CONFIGMS_XSIZE 40

#define TTLCONDTRIG_CONFIGINTIMELEFT_XSIZE 80
#define TTLCONDTRIG_CONFIGINTIMEMID_XSIZE 60
#define TTLCONDTRIG_CONFIGINTIMERIGHT_XSIZE 110

#define TTLCONDTRIG_CONFIGOUTHILO_XSIZE 50
#define TTLCONDTRIG_CONFIGOUTHILOLEFT_XSIZE 80
#define TTLCONDTRIG_CONFIGOUTHILOMID_XSIZE 25
#define TTLCONDTRIG_CONFIGOUTHILORIGHT_XSIZE 25

#define TTLCONDTRIG_CONFIGPANEL_XSIZE (TTLCONDTRIG_INPANEL_XSIZE + TTLCONDTRIG_OUTPANEL_XSIZE)
#define TTLCONDTRIG_CONFIGPANEL_YSIZE (TTLCONDTRIG_YSIZE * 5 + TTLCONDTRIG_YGAP * 6)


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

#define TEXTEDIT_NORMAL juce::Colours::lightgrey
#define TEXTEDIT_ACTIVE juce::Colours::white

// This needs as many entries as we have outputs.
// Original.
//#define OUTPUT_COLOUR_LIST { juce::Colours::darksalmon, juce::Colours::goldenrod, juce::Colours::seagreen, juce::Colours::steelblue }
// Vibrant.
//#define OUTPUT_COLOUR_LIST { juce::Colours::darksalmon, juce::Colours::goldenrod, juce::Colours::forestgreen, juce::Colours::royalblue }
// Alternate.
#define OUTPUT_COLOUR_LIST { juce::Colours::darksalmon, juce::Colours::goldenrod, juce::Colours::olivedrab, juce::Colours::darkcyan }

//#define OUTPUT_BACKGROUND juce::Colours::darkgrey
//#define OUTPUT_BACKGROUND juce::Colours::midnightblue
#define OUTPUT_BACKGROUND juce::Colours::black


#endif

// This is the end of the file.
