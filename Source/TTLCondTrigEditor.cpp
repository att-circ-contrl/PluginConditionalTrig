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
//
// GUI row for status and config buttons for one input.


// Constructor.
TTLConditionalTriggerEditorInputRow::TTLConditionalTriggerEditorInputRow(TTLConditionalTriggerEditor* newParent, int newInIdx)
{
    parent = newParent;
    inIdx = newInIdx;

    // Indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampOnImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_ON_FILL, LAMP_ON_HIGHLIGHT);
    lampOnComponent = new ImageComponent();
    lampOnComponent->setImage(*lampOnImage);

    lampOnComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOnComponent);
    lampOnComponent->setEnabled(false);
    lampOnComponent->setVisible(false);

    lampOffImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);
    lampOffComponent = new ImageComponent();
    lampOffComponent->setImage(*lampOffImage);

    lampOffComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOffComponent);
    lampOffComponent->setEnabled(false);

    // Input name label.
    inputNameLabel = new Label("Input Name", "undefined");
    inputNameLabel->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP, 0, TTLCONDTRIG_LABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputNameLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    inputNameLabel->setEnabled(false);

    // Settings button.
    settingsImage = new WrenchImage(WRENCH_BACKGROUND, WRENCH_FOREGROUND);
    settingsButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    settingsButton->setImages(true, true, true, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 0.5, COLOUR_TRANSPARENT);
    settingsButton->setBounds(TTLCONDTRIG_INROW_XSIZE - TTLCONDTRIG_WRENCH_SIZE, 0, TTLCONDTRIG_WRENCH_SIZE, TTLCONDTRIG_WRENCH_SIZE);
    settingsButton->addListener(this);
    addAndMakeVisible(settingsButton);

    // Force sane state.
    setInputEnabled(false);
    setRunningState(false);
}


// Callback for button presses. This is just the settings button.
void TTLConditionalTriggerEditorInputRow::buttonClicked(Button *theButton)
{
    // There's only one button to click.
    parent->clickedInputSettings(inIdx);
}


// Accessors.

void TTLConditionalTriggerEditorInputRow::setInputLabel(std::string newLabel)
{
    inputNameLabel->setText(newLabel, dontSendNotification);
}


void TTLConditionalTriggerEditorInputRow::setLampState(bool wantLit)
{
    lampOnComponent->setVisible(wantLit);
    lampOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorInputRow::setRunningState(bool isRunning)
{
    // Lock out the settings button if we're running.
    settingsButton->setEnabled(!isRunning);
}


void TTLConditionalTriggerEditorInputRow::setInputEnabled(bool isEnabled)
{
    // If we're not using this input, disable the text (to grey it out) and set an appropriate label.

    if (!isEnabled)
        setInputLabel("(unused)");

    inputNameLabel->setEnabled(isEnabled);
}


//
//
// GUI panel for configuring a set of inputs associated with an output.


// Constructor.
TTLConditionalTriggerEditorInputPanel::TTLConditionalTriggerEditorInputPanel(TTLConditionalTriggerEditor* newParent)
{
    backgroundColour = COLOUR_BOGUS;

    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
    {
        inputRows[inIdx] = new TTLConditionalTriggerEditorInputRow(newParent, inIdx);
        inputRows[inIdx]->setBounds(TTLCONDTRIG_XGAP, (TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP) * (inIdx+1) + TTLCONDTRIG_YGAP, TTLCONDTRIG_INROW_XSIZE, TTLCONDTRIG_YSIZE);
        addAndMakeVisible(inputRows[inIdx]);
    }

    bannerLabel = new Label("Panel Title", "undefined");
    bannerLabel->setBounds(TTLCONDTRIG_XGAP, TTLCONDTRIG_YGAP, TTLCONDTRIG_INPANEL_XSIZE - (TTLCONDTRIG_XGAP * 2), TTLCONDTRIG_YSIZE);
    addAndMakeVisible(bannerLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    bannerLabel->setEnabled(false);

    setOutputLabel("undefined");

    setOpaque(true);
}


// Redraw hook.
void TTLConditionalTriggerEditorInputPanel::paint(Graphics& g)
{
    // Flat background underneath child components.
    g.fillAll(backgroundColour);
}


// Accessors.

void TTLConditionalTriggerEditorInputPanel::setOutputLabel(std::string newLabel)
{
    std::string scratchstring;

    scratchstring = "Inputs for ";
    scratchstring += newLabel;
    bannerLabel->setText(scratchstring, dontSendNotification);
}


void TTLConditionalTriggerEditorInputPanel::setInputLabel(int inIdx, std::string newLabel)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
       inputRows[inIdx]->setInputLabel(newLabel);
}


void TTLConditionalTriggerEditorInputPanel::setLampState(int inIdx, bool wantLit)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
       inputRows[inIdx]->setLampState(wantLit);
}


void TTLConditionalTriggerEditorInputPanel::setFillColour(Colour newColour)
{
    backgroundColour = newColour;
}


void TTLConditionalTriggerEditorInputPanel::setRunningState(bool isRunning)
{
    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        inputRows[inIdx]->setRunningState(isRunning);
}


void TTLConditionalTriggerEditorInputPanel::setInputEnabled(int inIdx, bool isEnabled)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
       inputRows[inIdx]->setInputEnabled(isEnabled);
}


//
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

// FIXME - Editor constructor NYI/IPR.

    inputStatusPanel = new TTLConditionalTriggerEditorInputPanel(this);
    addAndMakeVisible(inputStatusPanel);
    inputStatusPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_INPANEL_XSIZE, TTLCONDTRIG_INPANEL_YSIZE);

    setDesiredWidth(TTLCONDTRIG_INPANEL_XSIZE + TTLCONDTRIG_GLOBAL_XOFFSET*2);

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
    // Check the "acquisitionIsActive" state variable from GenericEditor.

    // If we just changed running/not-running state, update the GUI.
    if (acquisitionIsActive != wasRunningLastRedraw)
    {
        inputStatusPanel->setRunningState(acquisitionIsActive);
// FIXME - Set running/not-running for output status panel here too.
    }

    wasRunningLastRedraw = acquisitionIsActive;

    // Pull data if not running.
    // If we're running, let process() push it to avoid race conditions.
    if (!acquisitionIsActive)
        parent->pushStateToDisplay();

    // Only fully redraw configuration state when we're not running.
    if (!acquisitionIsActive)
        doConfigStateRedraw();

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


// Accessor for switching to editing conditions for an input.
void TTLConditionalTriggerEditor::clickedInputSettings(int idxClicked)
{
T_PRINT("clickedInputSettings() called for input " << idxClicked << ".");
// FIXME - clickedInputSettings() NYI.
}


// Accessor for switching to editing conditions for an ouput.
void TTLConditionalTriggerEditor::clickedOutputSettings(int idxClicked)
{
T_PRINT("clickedOutputSettings() called for output " << idxClicked << ".");
// FIXME - clickedOutputSettings() NYI.
}


// Accessor for switching which output's inputs are displayed.
void TTLConditionalTriggerEditor::clickedOutputTab(int idxClicked)
{
T_PRINT("clickedOutputTab() called for output " << idxClicked << ".");
// FIXME - clickedOutputTab() NYI.
}


// Accessor for leaving the condition editor.
void TTLConditionalTriggerEditor::clickedConditionExit()
{
T_PRINT("clickedConditionExit() called.");
// FIXME - clickedConditionExit() NYI.
}


//
// This is the end of the file.
