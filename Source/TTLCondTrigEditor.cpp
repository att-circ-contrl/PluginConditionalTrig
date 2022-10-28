#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

using namespace TTLConditionTrig;

// Diagnostic tattle macros.
#define TRIGEDITTATTLE
#ifdef TRIGEDITTATTLE
#define T_DEBUG(x) do { x } while(false);
#else
#define T_DEBUG(x) {}
#endif
// Flushing should already happen with std::endl, but force it anyways.
#define T_PRINT(x) T_DEBUG(std::cout << "[CondTrigEditor]  " << x << std::endl << std::flush;)


//
// GUI tray for conditional trigger display and configuration.

// Constructor.
TTLConditionalTriggerEditor::TTLConditionalTriggerEditor(TTLConditionalTrigger* newParent) : GenericEditor(newParent, true)
{
    parent = newParent;

T_PRINT("Editor constructor called.");

    // Force configuration to sane state.
    // This gets overwritten as soon as we start polling.

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputConfig[outIdx].clear();
        needAllInputs[outIdx] = true;
        outputLabels[outIdx] = "unnamed";

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputConfig[inMatrixPtr].clear();
            inputLabels[inMatrixPtr] = "unnamed";
            inMatrixPtr++;
        }
    }


    // Initialize configuration for the GUI itself.

    outputSelectIdx = 0;
    inputSelectIdx = 0;
    editingInput = false;
    editingOutput = false;
    // Cheat. We know that we'll be constructed before acquisition starts.
    // The redraw will think that we just switched modes, and will update everything appropriately.
    wasRunningLastRedraw = true;


    // Build the GUI.

    settingsImage = new WrenchImage(WRENCH_BACKGROUND, WRENCH_FOREGROUND);
    connectImage = new ConnectedImage(CONN_BACKGROUND, CONN_FOREGROUND);
    disconnectImage = new DisconnectedImage(DISCONN_BACKGROUND, DISCONN_FOREGROUND);
    lampOnImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_ON_FILL, LAMP_ON_HIGHLIGHT);
    lampOffImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);


// FIXME - Testing.
#if 1
dummyButton = new ImageButton("Foo");

// Images are normal, hover-over, and pressed.
// Tuples are image, image opacity, and overlay colour.
// Settings button test.
//dummyButton->setImages(true, true, true, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 0.5, COLOUR_TRANSPARENT);
// Connection button test.
//dummyButton->setImages(true, true, true, *disconnectImage, 1.0, COLOUR_TRANSPARENT, *disconnectImage, 1.0, COLOUR_TRANSPARENT, *connectImage, 1.0, COLOUR_TRANSPARENT);
// Lamp image test.
dummyButton->setImages(true, true, true, *lampOffImage, 1.0, COLOUR_TRANSPARENT, *lampOffImage, 1.0, COLOUR_TRANSPARENT, *lampOnImage, 1.0, COLOUR_TRANSPARENT);
// Set this if we're testing a toggle button.
dummyButton->setClickingTogglesState(true);

dummyButton->addListener(this);
addAndMakeVisible(dummyButton);
//dummyButton->setCentreRelative(0.5,0.5);
dummyButton->setBounds(30, 30, 60, 60);
#endif


// FIXME - Editor constructor NYI.


    // Start the state refresh timer.
    // NOTE - The redraw timer should be running even if we're not acquiring data.
    startTimer(TTLCONDTRIG_DISPLAY_REFRESH_MS);

T_PRINT("Editor constructor finished.");
}


// Destructor.
TTLConditionalTriggerEditor::~TTLConditionalTriggerEditor()
{
    // "OwnedArray" and "ScopedPointer" take care of de-allocation for us.

    // Clean up the timer. Parent constructor probably does this but be safe.
    stopTimer();
}


// Timer callback.
void TTLConditionalTriggerEditor::timerCallback()
{
    // Pull data if not running.
    // If we're running, let process() push it to avoid race conditions.

    // Check the "acquisitionIsActive" state variable from GenericEditor.
    if (!acquisitionIsActive)
    {
        parent->pushStateToDisplay();
        // Only fully redraw configuration state when we're not running.
        doConfigStateRedraw();
    }

    // Always redraw the elements that are updated while running.
    doRunningStateRedraw();
}


// Accessor to push input configuration state to the editor.
void TTLConditionalTriggerEditor::pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig, std::string newLabel)
{
    if ( (inMatrixIdx >= 0) && (inMatrixIdx < (TTLCONDTRIG_INPUTS*TTLCONDTRIG_OUTPUTS)) )
    {
        inputConfig[inMatrixIdx] = newConfig;
        inputLabels[inMatrixIdx] = newLabel;
    }
}


// Accessor to push output configuration state to the editor.
void TTLConditionalTriggerEditor::pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, bool newNeedAllInputs, std::string newLabel)
{
    if ( (outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS) )
    {
        outputConfig[outIdx] = newConfig;
        needAllInputs[outIdx] = newNeedAllInputs;
        outputLabels[outIdx] = newLabel;
    }
}


// Accessor to push monitoring state (and output enable) to the editor.
// NOTE - Passing arrays by value involves shenanigans, but the caller's arrays should persist until this call returns, so we'll be okay.
void TTLConditionalTriggerEditor::pushRunningStateToEditor(bool (&rawInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&outputState)[TTLCONDTRIG_OUTPUTS], bool (&outputsEnabled)[TTLCONDTRIG_OUTPUTS])
{
    // Copy enable (configuration) and I/O state (lamp state).
    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputLampState[outIdx] = outputState[outIdx];
        outputConfig[outIdx].isEnabled = outputsEnabled[outIdx];

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputLampState[inMatrixPtr] = rawInputs[inMatrixPtr];
            inMatrixPtr++;
        }
    }
}


// Redraw function. Should be called from the timer, not the plugin.
// This redraws all configuration-sensitive elements.
void TTLConditionalTriggerEditor::doConfigStateRedraw()
{
// FIXME - doConfigStateRedraw() NYI.
}


// Redraw function. Should be called from the timer, not the plugin.
// This only redraws elements that are visible/accessible while running.
void TTLConditionalTriggerEditor::doRunningStateRedraw()
{
// FIXME - doRunningStateRedraw() NYI.
}


//
// This is the end of the file.
