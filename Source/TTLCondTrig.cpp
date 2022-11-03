#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigConstants.h"

using namespace TTLConditionTrig;

// Diagnostic tattle macros.
#define TRIGTATTLE
#ifdef TRIGTATTLE
#define T_DEBUG(x) do { x } while(false);
#else
#define T_DEBUG(x) {}
#endif
// Flushing should already happen with std::endl, but force it anyways.
#define T_PRINT(x) T_DEBUG(std::cout << "[CondTrig]  " << x << std::endl << std::flush;)


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
// We detect available inputs here.
void TTLConditionalTrigger::updateSettings()
{
T_PRINT("udpateSettings() called.");

    // Push input TTL geometry to the editor.

    // Pull output TTL information from the editor.
    // FIXME - We'd better hope it's safe to do this here! We can't send label strings through setParameter().

// FIXME - updateSettings() NYI.
// We need to get a list of inputs, pass them to the GUI, and specify our outputs.
// FIXME - We need to pull user-defined label information from the GUI. We'd better hope it's safe to do so from here!
}


// Processing loop.
void TTLConditionalTrigger::process(AudioSampleBuffer& buffer)
{
    // Process input events.
    checkForEvents();

// FIXME: Generate a test pattern.
// This generates phantom input events.
#if 1
    int64 thisTimeSamples = CoreServices::getGlobalTimestamp();
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

    inputConditions[inMatrixIdx].handleInput(thisTimeSamples, wantRaise);
#endif

    // Now that all of these have been processed, generate output events in temporal order.
// FIXME - process() event generation NYI.

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

    // FIXME - Make it easier to debug screwups.
    if ( (regIdx < 0) || (regIdx >= TTLCONDTRIG_PARAM_STRIDE) || (inIdx < 0) || (inIdx >= TTLCONDTRIG_INPUTS) || (outIdx < 0) || (outIdx >= TTLCONDTRIG_OUTPUTS) || (matrixIdx < 0) || (matrixIdx >= (TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS)) )
    {
T_PRINT("###  Indices out of range! Bailing out.");
        return;
    }


    // Update the requested configuration.
    // NOTE - We could special-case "enable" and call the lightweight accessor, but since that's triggered by a button click, it really isn't a big deal to do it with a full config read/write.

    ConditionConfig thisConfig;

    if (isInput)
        thisConfig = inputConditions[matrixIdx].getConfig();
    else if (isOutput)
        thisConfig = outputConditions[outIdx].getConfig();

    switch(regIdx)
    {
    // Configuration handled by the plugin.
    case TTLCONDTRIG_PARAM_IS_ENABLED:
        if (isInput)
            isInputEnabled[matrixIdx] = booleanValue;
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
            needAllInputs[outIdx] = booleanValue;
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


    if (isInput)
        inputConditions[matrixIdx].setConfig(thisConfig);
    else if (isOutput)
        outputConditions[matrixIdx].setConfig(thisConfig);
}


// XML configuration saving.
void TTLConditionalTrigger::saveCustomParametersToXml(XmlElement* parentElement)
{
T_PRINT("saveCustomParametersToXml() called.");

// FIXME - saveCustomParametersToXml() NYI.
}


// XML configuration loading.
// NOTE - This reads from the "parametersAsXml" variable, rather than passing an argument.
void TTLConditionalTrigger::loadCustomParametersFromXml()
{
T_PRINT("loadCustomParametersFromXml() called.");

// FIXME - loadCustomParametersFromXml() NYI.
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
        outputState[outIdx] = outputConditions[outIdx].getLastAcknowledgedOutput();

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            // NOTE - These are unconfigured defaults (no channel, active-high).
            // FIXME - A configured but disabled input _should_ show the input TTL value and, if active-_low_, a high output. But we can't easily test whether an input was ever configured.
            rawInputs[inMatrixPtr] = false;
            cookedInputs[inMatrixPtr] = false;

            if (isInputEnabled[inMatrixPtr])
            {
                rawInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastInput();
                cookedInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastAcknowledgedOutput();
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


// This is the end of the file.
