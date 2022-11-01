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

    // Indicator lamp images.
    lampGreenImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_GREEN_FILL, LAMP_GREEN_HIGHLIGHT);
    lampAmberImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_AMBER_FILL, LAMP_AMBER_HIGHLIGHT);
    lampOffImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);

    // Raw input indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampGreenOnComponent = new ImageComponent();
    lampGreenOnComponent->setImage(*lampGreenImage);

    lampGreenOnComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampGreenOnComponent);
    lampGreenOnComponent->setEnabled(false);
    lampGreenOnComponent->setVisible(false);

    lampGreenOffComponent = new ImageComponent();
    lampGreenOffComponent->setImage(*lampOffImage);

    lampGreenOffComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampGreenOffComponent);
    lampGreenOffComponent->setEnabled(false);

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
    settingsButton->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP*2, 0, TTLCONDTRIG_WRENCH_SIZE, TTLCONDTRIG_WRENCH_SIZE);
    settingsButton->addListener(this);
    addAndMakeVisible(settingsButton);

    // Cooked input indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampAmberOnComponent = new ImageComponent();
    lampAmberOnComponent->setImage(*lampAmberImage);

    lampAmberOnComponent->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampAmberOnComponent);
    lampAmberOnComponent->setEnabled(false);
    lampAmberOnComponent->setVisible(false);

    lampAmberOffComponent = new ImageComponent();
    lampAmberOffComponent->setImage(*lampOffImage);

    lampAmberOffComponent->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampAmberOffComponent);
    lampAmberOffComponent->setEnabled(false);

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


void TTLConditionalTriggerEditorInputRow::setRawLampState(bool wantLit)
{
    lampGreenOnComponent->setVisible(wantLit);
    lampGreenOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorInputRow::setCookedLampState(bool wantLit)
{
    lampAmberOnComponent->setVisible(wantLit);
    lampAmberOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorInputRow::setRunningState(bool isRunning)
{
    // Lock out the settings button if we're running.
    settingsButton->setEnabled(!isRunning);
}


void TTLConditionalTriggerEditorInputRow::setInputEnabled(bool isEnabled)
{
    // If we're not using this input, disable the text (to grey it out) and set an appropriate label.

// FIXME - We do need to flag unused inputs.
//    if (!isEnabled)
//        setInputLabel("(unused)");

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


void TTLConditionalTriggerEditorInputPanel::setRawLampState(int inIdx, bool wantLit)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setRawLampState(wantLit);
}


void TTLConditionalTriggerEditorInputPanel::setCookedLampState(int inIdx, bool wantLit)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setCookedLampState(wantLit);
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
// GUI row for status and config buttons for one output.


// Constructor.
TTLConditionalTriggerEditorOutputRow::TTLConditionalTriggerEditorOutputRow(TTLConditionalTriggerEditor* newParent, int newOutIdx)
{
    parent = newParent;
    outIdx = newOutIdx;

    // Enable button.
    connectOnImage = new ConnectedImage(CONN_BACKGROUND, CONN_FOREGROUND);
    connectOffImage = new DisconnectedImage(DISCONN_BACKGROUND, DISCONN_FOREGROUND);
    enableButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    enableButton->setImages(true, true, true, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOnImage, 0.5, COLOUR_TRANSPARENT);
    enableButton->setBounds(TTLCONDTRIG_XGAP, 0, TTLCONDTRIG_CONN_SIZE, TTLCONDTRIG_CONN_SIZE);
    enableButton->setClickingTogglesState(true);
    enableButton->addListener(this);
    addAndMakeVisible(enableButton);

    // Output name label.
    outputNameLabel = new Label("Output Name", "undefined");
    outputNameLabel->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_XGAP*2, 0, TTLCONDTRIG_LABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(outputNameLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    outputNameLabel->setEnabled(false);

    // Settings button.
    settingsImage = new WrenchImage(WRENCH_BACKGROUND, WRENCH_FOREGROUND);
    settingsButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    settingsButton->setImages(true, true, true, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 0.5, COLOUR_TRANSPARENT);
    settingsButton->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_WRENCH_SIZE, TTLCONDTRIG_WRENCH_SIZE);
    settingsButton->addListener(this);
    addAndMakeVisible(settingsButton);

    // Indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampOnImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_GREEN_FILL, LAMP_GREEN_HIGHLIGHT);
    lampOnComponent = new ImageComponent();
    lampOnComponent->setImage(*lampOnImage);

    lampOnComponent->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*4, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOnComponent);
    lampOnComponent->setEnabled(false);
    lampOnComponent->setVisible(false);

    lampOffImage = new IndicatorLampImage(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);
    lampOffComponent = new ImageComponent();
    lampOffComponent->setImage(*lampOffImage);

    lampOffComponent->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*4, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOffComponent);
    lampOffComponent->setEnabled(false);

    // Tab button.
    tabButton = new ColorButton("", Font("Small Text", 13, Font::plain));
    tabButton->setColors(juce::Colours::black, COLOUR_BOGUS);
    tabButton->setBounds(TTLCONDTRIG_OUTROW_XSIZE - TTLCONDTRIG_TAB_XSIZE, 0, TTLCONDTRIG_TAB_XSIZE, TTLCONDTRIG_YSIZE);
    tabButton->addListener(this);
    addAndMakeVisible(tabButton);

    // Force sane state.
    setTabColour(COLOUR_BOGUS);
    setOutputEnabled(false);
    setRunningState(false);
}


// Callback for button presses.
// This is the settings button, the enable button, or the output selection tab.
void TTLConditionalTriggerEditorOutputRow::buttonClicked(Button *theButton)
{
    if (theButton == tabButton)
        parent->clickedOutputTab(outIdx);
    else if (theButton == settingsButton)
        parent->clickedOutputSettings(outIdx);
    else
        parent->clickedOutputEnableToggle(outIdx);
}


// Redraw hook.
void TTLConditionalTriggerEditorOutputRow::paint(Graphics& g)
{
    // Flat background underneath child components.
    g.fillAll(currentBackgroundColour);
}


// Accessors.

void TTLConditionalTriggerEditorOutputRow::setOutputLabel(std::string newLabel)
{
    outputNameLabel->setText(newLabel, dontSendNotification);
}


void TTLConditionalTriggerEditorOutputRow::setLampState(bool wantLit)
{
    lampOnComponent->setVisible(wantLit);
    lampOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorOutputRow::setTabColour(Colour newColour)
{
    // The colour we're passed is the saturated version used for the tab itself.
    // Background and disabled colours are derived from this.

    // Get 50% and 75% versions of the original colour.
    // These are scaled (darkened); to get saturated, add an offset too.

    uint32 scratch = newColour.getARGB();
    uint32 dark50 = (scratch & 0x00fefefe) >> 1;
    uint32 dark75 = (scratch & 0x00ffffff) - ((scratch & 0x00fcfcfc) >> 2);


    // Get various derived colours for readability.
    // Unused ones will be optimized out.
    // There's no "set from uint32" accessor, so we have to create new instances.

    Colour colourSaturated = newColour;

    Colour colour75Light(dark75 + 0xff404040);
    Colour colour75Grey(dark75 + 0xff202020);
    Colour colour75Dark(dark75 + 0xff000000);

    Colour colour50Light(dark50 + 0xff404040);
    Colour colour50Grey(dark50 + 0xff202020);
    Colour colour50Dark(dark50 + 0xff000000);

    // Assign decent-looking colours.
    // NOTE - It turns out that changing colour between enabled/disabled doesn't help.
    tabColourLight = colour75Dark;
    tabColourDark = tabColourLight;
    backColourLight = colourSaturated;
    backColourDark = backColourLight;

    // This gets overwritten when setOutputEnabled() is called.
    currentBackgroundColour = COLOUR_BOGUS;

    // FIXME - Call setOutputEnabled() to propagate colour values to buttons.
    setOutputEnabled(false);
}


Colour TTLConditionalTriggerEditorOutputRow::getBackgroundColour()
{
    // This returns the "active selection" version, irrespective of whether or not we are selected.
    return backColourLight;
}


void TTLConditionalTriggerEditorOutputRow::setRunningState(bool isRunning)
{
    // Lock out the settings button if we're running.
    // We can still switch tabs and enable/disable this output.
    settingsButton->setEnabled(!isRunning);
}


void TTLConditionalTriggerEditorOutputRow::setOutputEnabled(bool isEnabled)
{
    // If we're not using this output, disable the text (to grey it out).
    outputNameLabel->setEnabled(isEnabled);

    // Force the control button to the correct state.
    enableButton->setToggleState(isEnabled, dontSendNotification);

    // Set colours.
    currentBackgroundColour = (isEnabled ? backColourLight : backColourDark);
    tabButton->setColors(juce::Colours::black, (isEnabled ? tabColourLight : tabColourDark));

    // Force a redraw, since otherwise only some of the elements update.
    repaint();
}


//
//
// GUI panel for configuring all outputs.


// Constructor.
TTLConditionalTriggerEditorOutputPanel::TTLConditionalTriggerEditorOutputPanel(TTLConditionalTriggerEditor* newParent)
{
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputRows[outIdx] = new TTLConditionalTriggerEditorOutputRow(newParent, outIdx);
        outputRows[outIdx]->setBounds(TTLCONDTRIG_XGAP, (TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP) * (outIdx+1) + TTLCONDTRIG_YGAP, TTLCONDTRIG_OUTROW_XSIZE, TTLCONDTRIG_YSIZE);
        addAndMakeVisible(outputRows[outIdx]);
    }
}


// Accessors.

void TTLConditionalTriggerEditorOutputPanel::setOutputLabel(int outIdx, std::string newLabel)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setOutputLabel(newLabel);
}


void TTLConditionalTriggerEditorOutputPanel::setLampState(int outIdx, bool wantLit)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setLampState(wantLit);
}


void TTLConditionalTriggerEditorOutputPanel::setTabColour(int outIdx, Colour newColour)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setTabColour(newColour);
}


Colour TTLConditionalTriggerEditorOutputPanel::getBackgroundColour(int outIdx)
{
    Colour retVal = COLOUR_BOGUS;

    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        retVal = outputRows[outIdx]->getBackgroundColour();

    return retVal;
}


void TTLConditionalTriggerEditorOutputPanel::setRunningState(bool isRunning)
{
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputRows[outIdx]->setRunningState(isRunning);
}


void TTLConditionalTriggerEditorOutputPanel::setOutputEnabled(int outIdx, bool isEnabled)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setOutputEnabled(isEnabled);
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
    std::string scratchstring;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputConfig[outIdx].clear();
        needAllInputs[outIdx] = true;

//        outputLabels[outIdx] = "unnamed";
        scratchstring = "Output ";
        scratchstring += ('A' + outIdx);
        outputLabels[outIdx] = scratchstring;

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputConfig[inMatrixPtr].clear();

//            inputLabels[inMatrixPtr] = "unnamed";
            scratchstring = "Input ";
            scratchstring += ('A' + outIdx);
            scratchstring += std::to_string(inIdx);
            inputLabels[inMatrixPtr] = scratchstring;

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

    inputStatusPanel = new TTLConditionalTriggerEditorInputPanel(this);
    addAndMakeVisible(inputStatusPanel);
    inputStatusPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_INPANEL_XSIZE, TTLCONDTRIG_INPANEL_YSIZE);

// FIXME - Any-or-all buttons NYI.

    outputStatusPanel = new TTLConditionalTriggerEditorOutputPanel(this);
    addAndMakeVisible(outputStatusPanel);
    outputStatusPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET + TTLCONDTRIG_INPANEL_XSIZE + TTLCONDTRIG_BOOLBUTTON_XSIZE + TTLCONDTRIG_XGAP*2, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_OUTPANEL_XSIZE, TTLCONDTRIG_OUTPANEL_YSIZE);

// FIXME - Condition settings dialog NYI.

    setDesiredWidth(TTLCONDTRIG_INPANEL_XSIZE + TTLCONDTRIG_BOOLBUTTON_XSIZE + TTLCONDTRIG_OUTPANEL_XSIZE + TTLCONDTRIG_XGAP*2 + TTLCONDTRIG_GLOBAL_XOFFSET*2);


    // Set up coloured tabs.
    Colour scratchColours[TTLCONDTRIG_OUTPUTS] = OUTPUT_COLOUR_LIST;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputStatusPanel->setTabColour(outIdx, scratchColours[outIdx]);


    // Propagate output labels.
    // Input labels are propagated when we select a tab.
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputStatusPanel->setOutputLabel(outIdx, outputLabels[outIdx]);


    // Select a tab for initial display.
    clickedOutputTab(0);


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


// Signal chain update hook.
// We pull the list of TTL channels here.
void TTLConditionalTriggerEditor::updateSettings()
{
T_PRINT("updateSettings() called.");
// FIXME - updateSettings() NYI.
}


// Timer callback.
void TTLConditionalTriggerEditor::timerCallback()
{
    // Check the "acquisitionIsActive" state variable from GenericEditor.

    // If we just changed running/not-running state, update the GUI.
    if (acquisitionIsActive != wasRunningLastRedraw)
    {
        inputStatusPanel->setRunningState(acquisitionIsActive);
        outputStatusPanel->setRunningState(acquisitionIsActive);
// FIXME - Set running/not-running for the "any/all" buttons here too.
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


// XML configuration saving.
// The only thing the editor is responsible for is input and output labels.
void TTLConditionalTriggerEditor::saveCustomParameters(XmlElement* xml)
{
T_PRINT("saveCustomParameters() called.");

// FIXME - saveCustomParameters() NYI.
}


// XML configuration loading.
// The only thing the editor is responsible for is input and output labels.
void TTLConditionalTriggerEditor::loadCustomParameters(XmlElement* xml)
{
T_PRINT("loadCustomParameters() called.");

// FIXME - loadCustomParameters() NYI.
}


// Accessor to push input configuration state to the editor.
void TTLConditionalTriggerEditor::pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig)
{
    if ( (inMatrixIdx >= 0) && (inMatrixIdx < (TTLCONDTRIG_INPUTS*TTLCONDTRIG_OUTPUTS)) )
        inputConfig[inMatrixIdx] = newConfig;
}


// Accessor to push output configuration state to the editor.
void TTLConditionalTriggerEditor::pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, bool newNeedAllInputs)
{
    if ( (outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS) )
    {
        outputConfig[outIdx] = newConfig;
        needAllInputs[outIdx] = newNeedAllInputs;
    }
}


// Accessor to push monitoring state (and output enable) to the editor.
// NOTE - Passing arrays by value involves shenanigans, but the caller's arrays should persist until this call returns, so we'll be okay.
void TTLConditionalTriggerEditor::pushRunningStateToEditor(bool (&rawInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&cookedInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&outputState)[TTLCONDTRIG_OUTPUTS], bool (&outputsEnabled)[TTLCONDTRIG_OUTPUTS])
{
    // Copy enable (configuration) and I/O state (lamp state).
    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputLampState[outIdx] = outputState[outIdx];
        outputConfig[outIdx].isEnabled = outputsEnabled[outIdx];

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputRawLampState[inMatrixPtr] = rawInputs[inMatrixPtr];
            inputCookedLampState[inMatrixPtr] = cookedInputs[inMatrixPtr];
            inMatrixPtr++;
        }
    }
}


// Accessor to let the plugin pull output labels.
// NOTE - We'd better hope this is safe! We can't push strings via setParameter().
std::string TTLConditionalTriggerEditor::getOutputLabel(int outIdx)
{
    std::string retVal = "";

    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        retVal = outputLabels[outIdx];

    return retVal;
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

    // Force a manual repaint. Otherwise the tab ColorButton objects don't change with mouse focus.
//    repaint();
}


// Accessor for switching to editing conditions for an input.
void TTLConditionalTriggerEditor::clickedInputSettings(int idxClicked)
{
T_PRINT("clickedInputSettings() called for input " << idxClicked << ".");
// FIXME - clickedInputSettings() NYI.
}


// Accessor for switching to editing conditions for an ouput.
void TTLConditionalTriggerEditor::clickedOutputEnableToggle(int idxClicked)
{
T_PRINT("clickedOutputEnableToggle() called for output " << idxClicked << ".");

// FIXME - clickedOutputEnableToggle() NYI.

    if ((idxClicked >= 0) && (idxClicked < TTLCONDTRIG_OUTPUTS))
    {
        bool newEnabled = !outputConfig[idxClicked].isEnabled;
T_PRINT("Setting output enable for " << idxClicked << " to " << newEnabled << ".");

        parent->setOutputParamByChan(idxClicked, TTLCONDTRIG_PARAM_IS_ENABLED, (newEnabled ? 1 : 0));
// FIXME - Placeholder. Should do this through the plugin.
outputConfig[idxClicked].isEnabled = newEnabled;
        outputStatusPanel->setOutputEnabled(idxClicked, newEnabled);
    }
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

    if ((idxClicked >= 0) && (idxClicked < TTLCONDTRIG_OUTPUTS))
    {
        outputSelectIdx = idxClicked;
        inputStatusPanel->setFillColour(outputStatusPanel->getBackgroundColour(idxClicked));

        // Propagate the selected tab's input labels and output label.
        int inMatrixPtr = idxClicked * TTLCONDTRIG_INPUTS;
        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
            inputStatusPanel->setInputLabel(inIdx, inputLabels[inMatrixPtr + inIdx]);
        inputStatusPanel->setOutputLabel(outputLabels[idxClicked]);

        // Force a redraw, since many elements will otherwise be stale.
        repaint();
    }
}


// Accessor for leaving the condition editor.
void TTLConditionalTriggerEditor::clickedConditionExit()
{
T_PRINT("clickedConditionExit() called.");
// FIXME - clickedConditionExit() NYI.
}


//
// This is the end of the file.
