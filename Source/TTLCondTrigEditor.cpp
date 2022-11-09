#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

#define TRIGDEBUGPREFIX "[CondTrigEditor]  "
#include "TTLCondTrigDebug.h"

using namespace TTLConditionTrig;


//
//
// GUI tray for conditional trigger display and configuration.


// Constructor.
TTLConditionalTriggerEditor::TTLConditionalTriggerEditor(TTLConditionalTrigger* newParent) : GenericEditor(newParent, true)
{
    parent = newParent;

T_PRINT("Editor constructor called.");

    // Reset metadata.
    inBankNames.clear();
    inBankIndices.clear();
    inBankBits.clear();


    // Force configuration to sane state.
    // This gets overwritten as soon as we start polling.

    int inMatrixPtr = 0;
    std::string scratchstring;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputConfig[outIdx].clear();
        isOutputEnabled[outIdx] = false;
        needAllInputs[outIdx] = true;

        outputLampState[outIdx] = false;

        scratchstring = "Output ";
        scratchstring += ('A' + outIdx);
        outputLabels[outIdx] = scratchstring;

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputConfig[inMatrixPtr].clear();
            isInputEnabled[inMatrixPtr] = false;
            inputChanIdx[inMatrixPtr] = TTLCONDTRIG_CHANIDX_NONE;
            inputBitIdx[inMatrixPtr] = TTLCONDTRIG_BITIDX_NONE;

            inputRawLampState[inMatrixPtr] = false;
            inputCookedLampState[inMatrixPtr] = false;

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
    // Cheat. We know that we'll be constructed before acquisition starts.
    // The redraw will think that we just switched modes, and will update everything appropriately.
    wasRunningLastRedraw = true;


    // Build the GUI.

    inputStatusPanel = new TTLConditionalTriggerEditorInputPanel(this);
    addAndMakeVisible(inputStatusPanel);
    inputStatusPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_INPANEL_XSIZE, TTLCONDTRIG_INPANEL_YSIZE);

    outputStatusPanel = new TTLConditionalTriggerEditorOutputPanel(this);
    addAndMakeVisible(outputStatusPanel);
    outputStatusPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET + TTLCONDTRIG_INPANEL_XSIZE, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_OUTPANEL_XSIZE, TTLCONDTRIG_OUTPANEL_YSIZE);

    configPanel = new TTLConditionalTriggerEditorConfigPanel(this);
    addAndMakeVisible(configPanel);
    configPanel->setBounds(TTLCONDTRIG_GLOBAL_XOFFSET, TTLCONDTRIG_GLOBAL_YOFFSET, TTLCONDTRIG_CONFIGPANEL_XSIZE, TTLCONDTRIG_CONFIGPANEL_YSIZE);
    configPanel->setVisible(false);

    setDesiredWidth(TTLCONDTRIG_PLUGIN_XSIZE);


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

    // Reset metadata.
    inBankNames.clear();
    inBankIndices.clear();
    inBankBits.clear();

    // FIXME - Read the previous node's outputs, to avoid iterating across the outputs that we created, since we're called after createEventChannels().
    GenericProcessor *theSource = parent->getSourceNode();

    // This can be NULL if we were dragged into the tray first.
    if (NULL != theSource)
    {
        int chanCount = theSource->getTotalEventChannels();
// FIXME - Diagnostics.
//T_PRINT("Detected " << chanCount << " upstream event channel banks.");

        for (int cidx = 0; cidx < chanCount; cidx++)
        {
            const EventChannel* theChannel = theSource->getEventChannel(cidx);
            EventChannel::EventChannelTypes thisType = theChannel->getChannelType();
            int thisBitCount = theChannel->getNumChannels();
            // We can call the toStdString() method to convert this.
            String thisName = theChannel->getName();

// FIXME - Diagnostics.
//T_PRINT(".. Bank " << cidx << " (\"" << thisName << "\") has type " << ((int) thisType) << " with " << thisBitCount << " virtual channels.");

            if (EventChannel::TTL == thisType)
            {
                inBankNames.add(thisName);
                inBankIndices.add(cidx);
                inBankBits.add(thisBitCount);
            }
        }
    }

    // Refresh the channel/bit selection widgets and sanity-check our recorded input configurations.
    rebuildInputDialog();
    sanityCheckInputs();
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
    }

    wasRunningLastRedraw = acquisitionIsActive;

    // Pull data if not running.
    // If we're running, let process() push it to avoid race conditions.
    if (!acquisitionIsActive)
        parent->pushFullStateToDisplay();

    // Reconfigure elements that are updated while running.
    // Everything else gets updated elsewhere.
    propagateRunningElementConfig();

    // If we're not running, propagate everything else too.
    if (!acquisitionIsActive)
    {
        propagateInputPaneConfig();
        propagateOutputPaneConfig();
    }

    // Force a redraw here, rather than in half a dozen "user clicked something" functions.
    repaint();
}


// XML configuration saving.
// The only thing the editor is responsible for is input and output labels.
void TTLConditionalTriggerEditor::saveCustomParameters(XmlElement* xml)
{
T_PRINT("saveCustomParameters() called.");

    // The editor owns input and output label state. Everything else is owned by the plugin.
    xml->setAttribute("Type", "TTLCondTrigEditor");

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        XmlElement* thisOutputTag = xml->createNewChildElement("OutputLabel");
        thisOutputTag->setAttribute("OutIndex", outIdx);
        thisOutputTag->setAttribute("Label", outputLabels[outIdx]);

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            XmlElement* thisInputTag = thisOutputTag->createNewChildElement("InputLabel");
            thisInputTag->setAttribute("InIndex", inIdx);
            thisInputTag->setAttribute("Label", inputLabels[inMatrixPtr]);
            inMatrixPtr++;
        }
    }
}


// XML configuration loading.
// The only thing the editor is responsible for is input and output labels.
void TTLConditionalTriggerEditor::loadCustomParameters(XmlElement* xml)
{
T_PRINT("loadCustomParameters() called.");

    // The editor owns input and output label state. Everything else is owned by the plugin.

    forEachXmlChildElementWithTagName(*xml, outputElement, "OutputLabel")
    {
        int outIdx = outputElement->getIntAttribute("OutIndex");
        if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        {
            String theLabel = outputElement->getStringAttribute("Label", "(undefined)");
            outputLabels[outIdx] = theLabel.toStdString();
// FIXME - Diagnostics.
//T_PRINT(".. Setting output " << outIdx << " label to \"" << theLabel << "\".");
        }
        else
        {
            T_PRINT("###  Asked to set label for out-of-range output " << outIdx << ".");
        }

        forEachXmlChildElementWithTagName(*outputElement, inputElement, "InputLabel")
        {
            // Shadow the parent's outIdx value. Child's takes priority.
            int inIdx = inputElement->getIntAttribute("InIndex");
            if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS) && (inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
            {
                String theLabel = inputElement->getStringAttribute("Label", "(undefined)");
                inputLabels[inIdx + outIdx * TTLCONDTRIG_INPUTS] = theLabel.toStdString();
// FIXME - Diagnostics.
//T_PRINT(".. Setting input " << outIdx << ":" << inIdx << " label to \"" << theLabel << "\".");
            }
            else
            {
                T_PRINT("###  Asked to set label for out-of-range input with inIdx " << inIdx << " and outIdx " << outIdx << ".");
            }
        }
    }
}


// Accessor to push input configuration state to the editor.
void TTLConditionalTriggerEditor::pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig, bool wantEnabled, int newChanIdx, int newBitIdx)
{
    if ( (inMatrixIdx >= 0) && (inMatrixIdx < (TTLCONDTRIG_INPUTS*TTLCONDTRIG_OUTPUTS)) )
    {
        inputConfig[inMatrixIdx] = newConfig;
        isInputEnabled[inMatrixIdx] = wantEnabled;
        inputChanIdx[inMatrixIdx] = newChanIdx;
        inputBitIdx[inMatrixIdx] = newBitIdx;
    }
}


// Accessor to push output configuration state to the editor.
void TTLConditionalTriggerEditor::pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, bool wantEnabled, bool newNeedAllInputs)
{
    if ( (outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS) )
    {
        outputConfig[outIdx] = newConfig;
        isOutputEnabled[outIdx] = wantEnabled;
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
        isOutputEnabled[outIdx] = outputsEnabled[outIdx];

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
    std::string retVal = "bogus";

    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        retVal = outputLabels[outIdx];

    return retVal;
}


// Accessor for switching to editing conditions for an input.
void TTLConditionalTriggerEditor::clickedInputSettings(int idxClicked)
{
T_PRINT("clickedInputSettings() called for input " << idxClicked << ".");

    inputSelectIdx = idxClicked;
    int inMatrixPtr = inputSelectIdx + outputSelectIdx * TTLCONDTRIG_INPUTS;

    configPanel->setEditingState(inputSelectIdx, outputSelectIdx, inputConfig[inMatrixPtr], isInputEnabled[inMatrixPtr], inputChanIdx[inMatrixPtr], inputBitIdx[inMatrixPtr], inputLabels[inMatrixPtr], outputLabels[outputSelectIdx]);

    inputStatusPanel->setVisible(false);
    inputStatusPanel->setEnabled(false);
    outputStatusPanel->setVisible(false);
    outputStatusPanel->setEnabled(false);

    configPanel->setVisible(true);
    configPanel->setEnabled(true);
}


// Accessor for toggling "want any"/"want all".
void TTLConditionalTriggerEditor::clickedAnyAll()
{
T_PRINT("clickedAnyAll() called.");
    bool wantAll = !needAllInputs[outputSelectIdx];
    parent->setOutputParamByChan(outputSelectIdx, TTLCONDTRIG_PARAM_WANT_ALL, wantAll);
}


// Accessor for switching to editing conditions for an output.
void TTLConditionalTriggerEditor::clickedOutputEnableToggle(int idxClicked)
{
T_PRINT("clickedOutputEnableToggle() called for output " << idxClicked << ".");

    if ((idxClicked >= 0) && (idxClicked < TTLCONDTRIG_OUTPUTS))
    {
        bool newEnabled = !isOutputEnabled[idxClicked];
T_PRINT("Setting output enable for " << idxClicked << " to " << newEnabled << ".");

        parent->setOutputParamByChan(idxClicked, TTLCONDTRIG_PARAM_IS_ENABLED, (newEnabled ? 1 : 0));
    }
}


// Accessor for switching to editing conditions for an output.
void TTLConditionalTriggerEditor::clickedOutputSettings(int idxClicked)
{
T_PRINT("clickedOutputSettings() called for output " << idxClicked << ".");

    std::string bogusString = "bogus";

    // NOTE - "outputSelectIdx" is the tab we're viewing, not the index clicked.
    configPanel->setEditingState(-1, idxClicked, outputConfig[idxClicked], isOutputEnabled[idxClicked], -1, -1, bogusString, outputLabels[idxClicked]);

    inputStatusPanel->setVisible(false);
    inputStatusPanel->setEnabled(false);
    outputStatusPanel->setVisible(false);
    outputStatusPanel->setEnabled(false);

    configPanel->setVisible(true);
    configPanel->setEnabled(true);
}


// Accessor for switching which output's inputs are displayed.
void TTLConditionalTriggerEditor::clickedOutputTab(int idxClicked)
{
T_PRINT("clickedOutputTab() called for output " << idxClicked << ".");

    if ((idxClicked >= 0) && (idxClicked < TTLCONDTRIG_OUTPUTS))
    {
        outputSelectIdx = idxClicked;
        propagateInputPaneConfig();
    }
}


// Accessor for leaving the condition editor.
void TTLConditionalTriggerEditor::clickedConditionExit()
{
T_PRINT("clickedConditionExit() called.");

    // Figure out what we were just editing.
    int inIdx = configPanel->getInIdxEdited();
    int outIdx = configPanel->getOutIdxEdited();
    bool isInput = (inIdx >= 0);
    int inMatrixPtr = 0;
    if (isInput)
        inMatrixPtr = inIdx + outIdx * TTLCONDTRIG_INPUTS;
// Diagnostics.
T_PRINT("Got inIdx " << (isInput ? "*" : "") << inIdx << ", outIdx " << (isInput ? "" : "*") << outIdx << ", matrixIdx " << inMatrixPtr << ".");

    // Pull any changes to the label configuration (we own this).

    if (isInput)
        inputLabels[inMatrixPtr] = configPanel->getInputLabel();
    else
        outputLabels[outIdx] = configPanel->getOutputLabel();

    // Pull logic configuration changes and push them to the plugin (the plugin owns them).

    ConditionConfig thisConfig = configPanel->getConfig();

    if (isInput)
    {
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_IS_ENABLED, configPanel->getEnabled());
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_CHAN_IDX, configPanel->getInputChan());
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_BIT_IDX, configPanel->getInputBit());

        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_INFEATURE, thisConfig.desiredFeature);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_DELAY_MIN, thisConfig.delayMinSamps);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_DELAY_MAX, thisConfig.delayMaxSamps);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_SUSTAIN, thisConfig.sustainSamps);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_DEADTIME, thisConfig.deadTimeSamps);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_DEGLITCH, thisConfig.deglitchSamps);
        parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_OUTSENSE, (thisConfig.outputActiveHigh ? 1 : 0));
    }
    else
    {
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_IS_ENABLED, configPanel->getEnabled());

        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_INFEATURE, thisConfig.desiredFeature);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_DELAY_MIN, thisConfig.delayMinSamps);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_DELAY_MAX, thisConfig.delayMaxSamps);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_SUSTAIN, thisConfig.sustainSamps);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_DEADTIME, thisConfig.deadTimeSamps);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_DEGLITCH, thisConfig.deglitchSamps);
        parent->setOutputParamByChan(outIdx, TTLCONDTRIG_PARAM_OUTSENSE, (thisConfig.outputActiveHigh ? 1 : 0));
    }

    // Update GUI state.

    configPanel->setVisible(false);
    configPanel->setEnabled(false);

    inputStatusPanel->setVisible(true);
    inputStatusPanel->setEnabled(true);
    outputStatusPanel->setVisible(true);
    outputStatusPanel->setEnabled(true);
}


// This updates configuration of elements that change while running.
// The timer callback calls this.
void TTLConditionalTriggerEditor::propagateRunningElementConfig()
{
    // Elements that are updated are input and output lamp states and output enable toggles.

    int inMatrixPtr = outputSelectIdx * TTLCONDTRIG_INPUTS;
    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
    {
        inputStatusPanel->setRawLampState(inIdx, inputRawLampState[inMatrixPtr]);
        inputStatusPanel->setCookedLampState(inIdx, inputCookedLampState[inMatrixPtr]);
        inMatrixPtr++;
    }

    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputStatusPanel->setLampState(outIdx, outputLampState[outIdx]);
        outputStatusPanel->setOutputEnabled(outIdx, isOutputEnabled[outIdx]);
    }
}


// This updates non-running configuration for the input pane. Mostly this happens when an output tab is clicked, but it'll also reflect label changes.
void TTLConditionalTriggerEditor::propagateInputPaneConfig()
{
    inputStatusPanel->setFillColour(outputStatusPanel->getBackgroundColour(outputSelectIdx));

    int inMatrixPtr = outputSelectIdx * TTLCONDTRIG_INPUTS;
    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
    {
        inputStatusPanel->setInputLabel(inIdx, inputLabels[inMatrixPtr]);
        inputStatusPanel->setInputEnabled(inIdx, isInputEnabled[inMatrixPtr]);
        inMatrixPtr++;
    }

    inputStatusPanel->setOutputLabel(outputLabels[outputSelectIdx]);
    inputStatusPanel->setAnyAllState(needAllInputs[outputSelectIdx]);
}


// This updates non-running configuration for the output pane. Mostly this reflects label changes.
void TTLConditionalTriggerEditor::propagateOutputPaneConfig()
{
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputStatusPanel->setOutputLabel(outIdx, outputLabels[outIdx]);
}


// This tells the settings dialog to rebuild its input selectors after input geometry is re-detected.
void TTLConditionalTriggerEditor::rebuildInputDialog()
{
    configPanel->rebuildChannelSelect(inBankNames, inBankIndices, inBankBits);
}


// This checks to make sure that input channels and bit numbers are still valid after input geometry changes.
// If they're no longer valid, it tells the plugin to disabled them (preserving values in case the input geometry change is transient).
void TTLConditionalTriggerEditor::sanityCheckInputs()
{
T_PRINT("sanityCheckInputs() called.");

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            if (isInputEnabled[inMatrixPtr])
            {
                int thisChanIdx = inputChanIdx[inMatrixPtr];
                int thisBitIdx = inputBitIdx[inMatrixPtr];
                int thisLUTIdx = inBankIndices.indexOf(thisChanIdx);

                bool isOk = true;
                if (thisLUTIdx < 0)
                    isOk = false;
                else if (thisBitIdx >= inBankBits[thisLUTIdx])
                    isOk = false;

                if (!isOk)
                {
// NOTE - This might get spammy.
T_PRINT(".. Disabling input " << inMatrixPtr << " as signal " << thisChanIdx << ":" << thisBitIdx << " no longer exists.");
                    isInputEnabled[inMatrixPtr] = 0;
                    parent->setInputParamByChan(outIdx, inIdx, TTLCONDTRIG_PARAM_IS_ENABLED, false);
                }
            }

            inMatrixPtr++;
        }
}


//
// This is the end of the file.
