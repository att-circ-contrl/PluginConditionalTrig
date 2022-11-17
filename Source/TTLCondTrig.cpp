#include "TTLCondTrigIncludes.h"

//
// TTL conditional trigger logic plugin.


// Constructor.
TTLConditionalTrigger::TTLConditionalTrigger() : GenericProcessor("TTL Conditional Trigger")
{
T_PRINT("Constructor called.");

    // NOTE - Per Josh, we need to set the processor type here in addition to
    // reporting it from getLibInfo().
    setProcessorType(PluginProcessorType::PROCESSOR_TYPE_FILTER);


    // Condition configs should already have been initialized by their constructors, but do it explicitly just to be safe.
    // Ditto output "any vs all" switches.

    ConditionConfig dummyConfig;
    dummyConfig.clear();

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputConditions[outIdx].setConfig(dummyConfig);
        outputConditions[outIdx].resetInput(-1, false);
        outputConditions[outIdx].resetState();

        isOutputEnabled[outIdx] = false;
        needAllInputs[outIdx] = true;

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputConditions[inMatrixPtr].setConfig(dummyConfig);
            inputConditions[inMatrixPtr].resetInput(0, false);
            inputConditions[inMatrixPtr].resetState();

            isInputEnabled[inMatrixPtr] = false;
            inputChanIdx[inMatrixPtr] = TTLCONDTRIG_CHANIDX_NONE;
            inputBitIdx[inMatrixPtr] = TTLCONDTRIG_BITIDX_NONE;

            inMatrixPtr++;
        }

    }

    // Initialize the mergers, now that configuration is initialized.
    // This should already be at a reasonable state, but do it anyways.
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        rebuildMergeConfig(outIdx);

    // Set up the output serializer.
    // This always feeds from all channels even if they aren't all enabled.
    outputSerializer.clearInputList();
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputSerializer.addInput(&(outputConditions[outIdx]), outIdx);
}


// Destructor.
TTLConditionalTrigger::~TTLConditionalTrigger()
{
    // Nothing to do.
}


// Editor accessor.
AudioProcessorEditor* TTLConditionalTrigger::createEditor()
{
T_PRINT("Creating editor.");

    // NOTE - We need to set the "editor" variable in GenericProcessor.
    editor = new TTLConditionalTriggerEditor(this);
    return editor;
}


// Rebuild external configuration information.
// We create TTL outputs here.
void TTLConditionalTrigger::createEventChannels()
{
T_PRINT("createEventChannels() called.");

    // Get a properly-cast pointer to the editor. We need to talk to it.
    TTLConditionalTriggerEditor* theEditor = (TTLConditionalTriggerEditor*) (editor.get());

    // Ask the editor what the output labels should be.
    // FIXME - We'd better hope it's safe to do this here! We can't send label strings through setParameter().
    std::string outputLabels[TTLCONDTRIG_OUTPUTS];
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputLabels[outIdx] = theEditor->getOutputLabel(outIdx);

    // Create our output channels.
    // Always build these, even if they aren't used/enabled.

    // FIXME - Conflicting documentation for TTL channel length!
    // One source says TTL length is 1, another says it's the number of bytes needed to represent the number of virtual channels.
//    int evLength = 1;
    int evLength = (TTLCONDTRIG_OUTPUTS + 7)/8;
    outputEventChan = new EventChannel(EventChannel::TTL, TTLCONDTRIG_OUTPUTS, evLength, -1, this);
    outputEventChan->setName("conditional triggers");
    outputEventChan->setIdentifier("ttlcondtrig.outputs");

    // FIXME - Storing output signal names in the description, since I'm not sure virtual channels (bit-lines) _have_ names anywhere.
    std::string scratchDesc = "(";
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        scratchDesc += (outIdx > 0 ? ", " : " ");
        scratchDesc += outputLabels[outIdx];
    }
    scratchDesc += " )";
    outputEventChan->setDescription(scratchDesc);

    eventChannelArray.add(outputEventChan);
}


// Processing loop.
void TTLConditionalTrigger::process(AudioSampleBuffer& buffer)
{
    int64 thisTimeSamples = CoreServices::getGlobalTimestamp();

// FIXME: Generate a test pattern.
// This generates phantom input events, instead of using real events.
#if TRIGDEBUG_WANTINPUTPATTERN
{
    int64 sampRate = (int64) CoreServices::getGlobalSampleRate();

    // We want to sweep 16 inputs in 2 seconds. So, 1/8 sec. quantum.
    int64 thisTimeTicks = (thisTimeSamples * 8) / sampRate;
    // Turn each set of inputs on, then off.
    thisTimeTicks %= 2 * TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS;
    int outIdx = thisTimeTicks / (2*TTLCONDTRIG_INPUTS);
    int inIdx = thisTimeTicks % (2*TTLCONDTRIG_INPUTS);
    bool wantRaise = ( inIdx < TTLCONDTRIG_INPUTS );
    inIdx %= TTLCONDTRIG_INPUTS;
    int inMatrixIdx = inIdx + outIdx * TTLCONDTRIG_INPUTS;

    if (isInputEnabled[inMatrixIdx])
        inputConditions[inMatrixIdx].handleInput(thisTimeSamples, wantRaise);
}
#else
    // Process input events.
    checkForEvents();
#endif

    // Advance input condition processing to the present time.
    for (int inMatrixIdx = 0; inMatrixIdx < (TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS); inMatrixIdx++)
        if (isInputEnabled[inMatrixIdx])
            inputConditions[inMatrixIdx].advanceToTime(thisTimeSamples);

    // Merge input condition outputs and feed them to the output condition processors.
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputMergers[outIdx].processPendingInputUntil(thisTimeSamples);
        while (outputMergers[outIdx].hasPendingOutput())
        {
            int64 thisTime = outputMergers[outIdx].getNextOutputTime();
            bool thisLevel = outputMergers[outIdx].getNextOutputLevel();
            outputMergers[outIdx].acknowledgeOutput();

            if (isOutputEnabled[outIdx])
                outputConditions[outIdx].handleInput(thisTime, thisLevel);
        }
    }

    // Advance output condition processing to the present time.
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        if (isOutputEnabled[outIdx])
            outputConditions[outIdx].advanceToTime(thisTimeSamples);


    // Generate output events.
    // We have to serialize them so that they're properly time-ordered.

    outputSerializer.processPendingInputUntil(thisTimeSamples);
    while (outputSerializer.hasPendingOutput())
    {
        int64 thisTime = outputSerializer.getNextOutputTime();
        bool thisLevel = outputSerializer.getNextOutputLevel();
        int outIdx = outputSerializer.getNextOutputTag();
        outputSerializer.acknowledgeOutput();

        // NOTE - Sample number is relative to the start of the buffer fragment in this polling interval.
        // FIXME - Keeping sample number as zero!
        int thisSampleNum = 0;

        if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
            if (isOutputEnabled[outIdx] && (NULL != outputEventChan))
            {
                // FIXME - Blithely assume no more than 16 outputs.
                uint16 dataValue = (thisLevel ? 1 : 0);
                dataValue <<= outIdx;

                TTLEventPtr thisEv = TTLEvent::createTTLEvent(outputEventChan, thisTime, &dataValue, sizeof(dataValue), outIdx);
                addEvent(outputEventChan, thisEv, thisSampleNum);
            }
    }

    // Propagate state updates to the GUI.
    pushRunningStateToDisplay();
}


// TTL event handling.
void TTLConditionalTrigger::handleEvent(const EventChannel *eventInfo, const MidiMessage& event, int samplePosition)
{
// FIXME - handleEvent() NYI.
}


// Hook for modifying state variables.
// Variables used by "process" should only be modified here.
void TTLConditionalTrigger::setParameter(int parameterIndex, float newValue)
{
    long integerValue = (long) newValue;
    bool booleanValue = (integerValue > 0);

T_PRINT( "setParameter() called setting " << parameterIndex << " to: " << integerValue );

    // Map the raw parameter index back into multidimensional indices.
    // The output configuration is treated as a set of extra inputs appended to the end of the virtual register file.

    bool isInput = false;
    bool isOutput = false;

    int regIdx = 0;
    int inIdx = 0;
    int outIdx = 0;
    int matrixIdx = 0;

    if (parameterIndex >= 0)
    {
        regIdx = parameterIndex % TTLCONDTRIG_PARAM_STRIDE;
        matrixIdx = (parameterIndex - regIdx) / TTLCONDTRIG_PARAM_STRIDE;
        inIdx = matrixIdx % TTLCONDTRIG_INPUTS;
        outIdx = (matrixIdx - inIdx) / TTLCONDTRIG_INPUTS;

        if (outIdx < TTLCONDTRIG_OUTPUTS)
            isInput = true;
        else
        {
            // This should be one of the appended output configurations, but do a range check just in case.
            matrixIdx -= TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS;
            inIdx = 0;
            outIdx = matrixIdx;
            isOutput = (outIdx < TTLCONDTRIG_OUTPUTS);
        }
    }

T_PRINT("Got regIdx " << regIdx << ", inIdx " << (isInput ? "*" : "") << inIdx << ", outIdx " << (isOutput ? "*" : "") << outIdx << ", matrixIdx " << matrixIdx << ".");

    // Range check - Make it easier to debug screwups.
    if ( (regIdx < 0) || (regIdx >= TTLCONDTRIG_PARAM_STRIDE) || (inIdx < 0) || (inIdx >= TTLCONDTRIG_INPUTS) || (outIdx < 0) || (outIdx >= TTLCONDTRIG_OUTPUTS) || (matrixIdx < 0) || (matrixIdx >= (TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS)) )
    {
T_PRINT("###  Indices out of range! Bailing out.");
        return;
    }


    // Update the requested configuration.
    // This is called in response to a button-click, so it's okay to perform expensive updates here.

    ConditionConfig thisConfig;
    bool needMergeUpdate = false;

    if (isInput)
        thisConfig = inputConditions[matrixIdx].getConfig();
    else if (isOutput)
        thisConfig = outputConditions[outIdx].getConfig();

    switch(regIdx)
    {
    // Configuration handled by the plugin.
    case TTLCONDTRIG_PARAM_IS_ENABLED:
        if (isInput)
        {
            isInputEnabled[matrixIdx] = booleanValue;
            needMergeUpdate = true;
        }
        else
            isOutputEnabled[outIdx] = booleanValue;
        break;
    case TTLCONDTRIG_PARAM_CHAN_IDX:
        if (isInput)
            inputChanIdx[matrixIdx] = integerValue;
        break;
    case TTLCONDTRIG_PARAM_BIT_IDX:
        if (isInput)
            inputBitIdx[matrixIdx] = integerValue;
        break;
    case TTLCONDTRIG_PARAM_WANT_ALL:
        if (isOutput)
        {
            needAllInputs[outIdx] = booleanValue;
            needMergeUpdate = true;
        }
        break;
    // Configuration handled by the TTL logic widgets.
    case TTLCONDTRIG_PARAM_DELAY_MIN:
        thisConfig.delayMinSamps = integerValue; break;
    case TTLCONDTRIG_PARAM_DELAY_MAX:
        thisConfig.delayMaxSamps = integerValue; break;
    case TTLCONDTRIG_PARAM_SUSTAIN:
        thisConfig.sustainSamps = integerValue; break;
    case TTLCONDTRIG_PARAM_DEADTIME:
        thisConfig.deadTimeSamps = integerValue; break;
    case TTLCONDTRIG_PARAM_DEGLITCH:
        thisConfig.deglitchSamps = integerValue; break;
    case TTLCONDTRIG_PARAM_INFEATURE:
        // FIXME - Blithely assuming the value maps to a valid enum!
        thisConfig.desiredFeature = (ConditionConfig::FeatureType) integerValue;
        break;
    case TTLCONDTRIG_PARAM_OUTSENSE:
        thisConfig.outputActiveHigh = booleanValue; break;
    default:
        break;
    }


    thisConfig.forceSanity();
    if (isInput)
        inputConditions[matrixIdx].setConfig(thisConfig);
    else if (isOutput)
        outputConditions[matrixIdx].setConfig(thisConfig);


    if (needMergeUpdate)
        // outIdx indicates which merger we're updating.
        rebuildMergeConfig(outIdx);
}


// XML configuration saving.
void TTLConditionalTrigger::saveCustomParametersToXml(XmlElement* parentElement)
{
T_PRINT("saveCustomParametersToXml() called.");

    // The editor owns input and output labels. We own everything else.

    parentElement->setAttribute("Type", "TTLCondTrig");

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        XmlElement* thisOutputTag = parentElement->createNewChildElement("TTLOutput");

        thisOutputTag->setAttribute("OutIndex", outIdx);
        thisOutputTag->setAttribute("IsEnabled", isOutputEnabled[outIdx]);
        thisOutputTag->setAttribute("NeedAllInputs", needAllInputs[outIdx]);

        XmlElement* thisOutputConfig = thisOutputTag->createNewChildElement("LogicConfig");
        saveLogicToXml(thisOutputConfig, outputConditions[outIdx].getConfig());

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            XmlElement* thisInputTag = thisOutputTag->createNewChildElement("TTLInput");

            thisInputTag->setAttribute("InIndex", inIdx);
            thisInputTag->setAttribute("IsEnabled", isInputEnabled[inMatrixPtr]);
            thisInputTag->setAttribute("TTLChan", inputChanIdx[inMatrixPtr]);
            thisInputTag->setAttribute("TTLBit", inputBitIdx[inMatrixPtr]);

            XmlElement* thisInputConfig = thisInputTag->createNewChildElement("LogicConfig");
            saveLogicToXml(thisInputConfig, inputConditions[inMatrixPtr].getConfig());

            inMatrixPtr++;
        }
    }
}


// XML configuration loading.
// NOTE - This reads from the "parametersAsXml" variable, rather than passing an argument.
void TTLConditionalTrigger::loadCustomParametersFromXml()
{
T_PRINT("loadCustomParametersFromXml() called.");

    // The editor owns input and output labels. We own everything else.

    forEachXmlChildElementWithTagName(*parametersAsXml, thisOutputTag, "TTLOutput")
    {
        int outIdx = thisOutputTag->getIntAttribute("OutIndex");

        if ((outIdx < 0) || (outIdx >= TTLCONDTRIG_OUTPUTS))
        {
T_PRINT("###  Asked to load configuration for out-of-range output " << outIdx << ".");
        }
        else
        {
            isOutputEnabled[outIdx] = thisOutputTag->getBoolAttribute("IsEnabled");
            needAllInputs[outIdx] = thisOutputTag->getBoolAttribute("NeedAllInputs");

            // There should be exactly one output configuration tag.
            forEachXmlChildElementWithTagName(*thisOutputTag, thisOutputLogicTag, "LogicConfig")
            {
                ConditionConfig thisConfig = loadLogicFromXml(thisOutputLogicTag);
                outputConditions[outIdx].setConfig(thisConfig);
            }

            forEachXmlChildElementWithTagName(*thisOutputTag, thisInputTag, "TTLInput")
            {
                int inIdx = thisInputTag->getIntAttribute("InIndex");

                if ((inIdx < 0) || (inIdx >= TTLCONDTRIG_INPUTS))
                {
T_PRINT("###  Asked to load configuration for out-of-range input " << outIdx << ":" << inIdx << ".");
                }
                else
                {
                    int inMatrixPtr = inIdx + outIdx * TTLCONDTRIG_INPUTS;

                    isInputEnabled[inMatrixPtr] = thisInputTag->getBoolAttribute("IsEnabled");
                    inputChanIdx[inMatrixPtr] = thisInputTag->getIntAttribute("TTLChan");
                    inputBitIdx[inMatrixPtr] = thisInputTag->getIntAttribute("TTLBit");

                    // There should be exactly one input configuration tag.
                    forEachXmlChildElementWithTagName(*thisInputTag, thisInputLogicTag, "LogicConfig")
                    {
                        ConditionConfig thisConfig = loadLogicFromXml(thisInputLogicTag);
                        inputConditions[inMatrixPtr].setConfig(thisConfig);
                    }
                }
            }
        }
    }

    // Now that inputs and outputs are configured, rebuild the merge state.
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        rebuildMergeConfig(outIdx);
}


// Accessor for modifying input configuration state.
// This is a wrapper for setParameter.
void TTLConditionalTrigger::setInputParamByChan(int outputIdx, int inputIdx, int paramIdx, long newValue)
{
    int packedIdx = paramIdx;
    packedIdx += inputIdx * TTLCONDTRIG_PARAM_STRIDE_INPUT;
    packedIdx += outputIdx * TTLCONDTRIG_PARAM_STRIDE_OUTPUT;

    setParameter(packedIdx, newValue);
}


// Accessor for modifying output configuration state.
// This is a wrapper for setParameter.
void TTLConditionalTrigger::setOutputParamByChan(int outputIdx, int paramIdx, long newValue)
{
    int packedIdx = paramIdx;
    // The output configuration is treated as a set of inputs appended to the end of the virtual register file.
    packedIdx += outputIdx * TTLCONDTRIG_PARAM_STRIDE_INPUT;
    packedIdx += TTLCONDTRIG_OUTPUTS * TTLCONDTRIG_PARAM_STRIDE_OUTPUT;

    setParameter(packedIdx, newValue);
}


// This propagates state to the display.
// This version only propagates things that should update while we're running.
void TTLConditionalTrigger::pushRunningStateToDisplay()
{
    // NOTE - The editor is already stored as the GenericProcessor class variable "editor".
    // We do need to re-cast it.
    TTLConditionalTriggerEditor* theEditor = (TTLConditionalTriggerEditor*) (editor.get());

    // Aggregate and push the relevant state elements.
    bool rawInputs[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
    bool cookedInputs[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
    bool outputState[TTLCONDTRIG_OUTPUTS];

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputState[outIdx] = outputConditions[outIdx].getLastAcknowledgedLevel();

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            // NOTE - These are unconfigured defaults (no channel, active-high).
            // FIXME - A configured but disabled input _should_ show the input TTL value and, if active-_low_, a high output. But we can't easily test whether an input was ever configured.
            rawInputs[inMatrixPtr] = false;
            cookedInputs[inMatrixPtr] = false;

            if (isInputEnabled[inMatrixPtr])
            {
                rawInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastInputLevel();
                cookedInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastAcknowledgedLevel();
            }

            inMatrixPtr++;
        }
    }

    theEditor->pushRunningStateToEditor(rawInputs, cookedInputs, outputState, isOutputEnabled);
}


// This propagates state to the display.
// This version is called when we're not running, and propagates everything.
void TTLConditionalTrigger::pushFullStateToDisplay()
{
    // NOTE - The editor is already stored as the GenericProcessor class variable "editor".
    // We do need to re-cast it.
    TTLConditionalTriggerEditor* theEditor = (TTLConditionalTriggerEditor*) (editor.get());

    // We're not running. Push the full config state.
    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        theEditor->pushOutputConfigToEditor(outIdx, outputConditions[outIdx].getConfig(), isOutputEnabled[outIdx], needAllInputs[outIdx]);
        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            theEditor->pushInputConfigToEditor(inMatrixPtr, inputConditions[inMatrixPtr].getConfig(), isInputEnabled[inMatrixPtr], inputChanIdx[inMatrixPtr], inputBitIdx[inMatrixPtr]);
            inMatrixPtr++;
        }
    }

    // Propagate the running state as well.
    pushRunningStateToDisplay();
}


// This saves a condition configuration structure as attributes in an XML tag.
void TTLConditionalTrigger::saveLogicToXml(XmlElement* theTag, ConditionConfig theConfig)
{
    // NOTE - Attribute values are clamped to "int" range. Live with this.
    theTag->setAttribute("Feature", theConfig.desiredFeature);
    theTag->setAttribute("DelayMin", (int) theConfig.delayMinSamps);
    theTag->setAttribute("DelayMax", (int) theConfig.delayMaxSamps);
    theTag->setAttribute("SustainTime", (int) theConfig.sustainSamps);
    theTag->setAttribute("DeadTime", (int) theConfig.deadTimeSamps);
    theTag->setAttribute("DeglitchTime", (int) theConfig.deglitchSamps);
    theTag->setAttribute("ActiveHigh", theConfig.outputActiveHigh);
}


// This rebuilds a condition configuration structure from attributes in an XML tag.
ConditionConfig TTLConditionalTrigger::loadLogicFromXml(XmlElement* theTag)
{
    ConditionConfig theConfig;

    theConfig.clear();

    // Ignore anything we don't understand and leave anything not set as its default value.

    if (theTag->hasAttribute("Feature"))
        theConfig.desiredFeature = (ConditionConfig::FeatureType) (theTag->getIntAttribute("Feature"));

    if (theTag->hasAttribute("DelayMin"))
        theConfig.delayMinSamps = theTag->getIntAttribute("DelayMin");

    if (theTag->hasAttribute("DelayMax"))
        theConfig.delayMaxSamps = theTag->getIntAttribute("DelayMax");

    if (theTag->hasAttribute("SustainTime"))
        theConfig.sustainSamps = theTag->getIntAttribute("SustainTime");

    if (theTag->hasAttribute("DeadTime"))
        theConfig.deadTimeSamps = theTag->getIntAttribute("SustainTime");

    if (theTag->hasAttribute("DeglitchTime"))
        theConfig.deglitchSamps = theTag->getIntAttribute("DeglitchTime");

    if (theTag->hasAttribute("ActiveHigh"))
        theConfig.outputActiveHigh = theTag->getBoolAttribute("ActiveHigh");

    theConfig.forceSanity();

    return theConfig;
}


// This rebuilds an output merger's configuration to reflect input enable and output any/all state.
void TTLConditionalTrigger::rebuildMergeConfig(int outIdx)
{
T_PRINT("rebuildMergeConfig() called for output " << outIdx << ".");

    outputMergers[outIdx].setMergeMode( needAllInputs[outIdx] ? LogicMerger::mergeAnd : LogicMerger::mergeOr );

    outputMergers[outIdx].clearInputList();
    int inMatrixPtr = outIdx * TTLCONDTRIG_INPUTS;
    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        if (isInputEnabled[inMatrixPtr + inIdx])
            outputMergers[outIdx].addInput( &(inputConditions[inMatrixPtr + inIdx]) );

    outputMergers[outIdx].resetState();
}


// This is the end of the file.
