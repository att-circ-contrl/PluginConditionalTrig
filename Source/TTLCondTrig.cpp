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
// Configuration for processing conditions on one signal.


// Constructor.
ConditionConfig::ConditionConfig()
{
    // Initialize to safe defaults.
    clear();
}


// This sets a known-sane configuration state.
void ConditionConfig::clear()
{
    // Set sane defaults.

    isEnabled = false;

    delayMinSamps = 0;
    delayMaxSamps = 0;

    sustainSamps = 10;

    deadTimeSamps = 100;

    deglitchSamps = 0;

    outputActiveHigh = true;

    chanIdx = 0;
    bitIdx = 0;
}


// Default destructor is fine.


//
// Condition processing for one TTL signal.


// Constructor.
ConditionProcessor::ConditionProcessor()
{
    // The constructor should already have done this, but do it anyways.
    config.clear();

    // Initialize. Use a dummy timestamp and input level.
    resetInput(0, false);
    resetState();
}


// Configuration accessors.

void ConditionProcessor::setConfig(ConditionConfig &newConfig)
{
    config = newConfig;

    // NOTE - We're clearing pending output state but keeping the record of previous input.
    resetState();
}


ConditionConfig ConditionProcessor::getConfig()
{
    return config;
}


// State reset. This clears active events after a configuration change.
void ConditionProcessor::resetState()
{
    pendingOutputTimes.clear();
    pendingOutputLevels.clear();
    prevAcknowledgedOutput = !(config.outputActiveHigh);
}


// Lightweight enable query/toggle accessors.

bool ConditionProcessor::isEnabled()
{
    return config.isEnabled;
}


void ConditionProcessor::setEnabled(bool wantEnabled)
{
    config.isEnabled = wantEnabled;
    // NOTE - We're clearing pending output state but keeping the record of previous input.
    resetState();
}


// This overwrites our record of the previous input levels without causing an event update.
// This is used for initialization at the start of acquisition.
void ConditionProcessor::resetInput(int64 resetTime, bool newInput)
{
    prevInputTime = resetTime;
    prevInputLevel = newInput;
}


// Input processing. This schedules future output in response to input events.
void ConditionProcessor::handleInput(int64 inputTime, bool inputLevel)
{
// FIXME - handleInput NYI.
}


// State accessors.

bool ConditionProcessor::hasPendingOutput()
{
    return (pendingOutputTimes.count() > 0);
}


int64 ConditionProcessor::getNextOutputTime()
{
    // NOTE - This will return a safe value (0) if we don't have output.
    return pendingOutputTimes.snoop();
}


bool ConditionProcessor::getNextOutputLevel()
{
    // NOTE - This will return a safe value (false) if we don't have output.
    return pendingOutputLevels.snoop();
}


// This removes the next queued output event, after we've read it.
void ConditionProcessor::acknowledgeOutput()
{
    // Save whatever the last output was.
    // This will return a safe value (false) if we don't have pending output.
    prevAcknowledgedOutput = pendingOutputLevels.snoop();

    // Discard return values.
    pendingOutputTimes.dequeue();
    pendingOutputLevels.dequeue();
}


// Display polling accessors.

bool ConditionProcessor::getLastInput()
{
    return prevInputLevel;
}


bool ConditionProcessor::getLastAcknowledgedOutput()
{
    return prevAcknowledgedOutput;
}


//
// TTL conditional trigger logic.


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

        needAllInputs[outIdx] = true;

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            inputConditions[inMatrixPtr].setConfig(dummyConfig);
            inputConditions[inMatrixPtr].resetInput(0, false);
            inputConditions[inMatrixPtr].resetState();

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

    // Now that all of these have been processed, generate output events in temporal order.
// FIXME - process() event generation NYI.

    // Propagate state updates to the GUI.
    pushStateToDisplay();
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

T_PRINT("Got regIdx " << regIdx << ", inIdx " << inIdx << ", outIdx " << outIdx << ", matrixIdx " << matrixIdx << ".");

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
    case TTLCONDTRIG_PARAM_IS_ENABLED:
        thisConfig.isEnabled = booleanValue; break;
    case TTLCONDTRIG_PARAM_CHAN_IDX:
        thisConfig.chanIdx = integerValue; break;
    case TTLCONDTRIG_PARAM_BIT_IDX:
        thisConfig.bitIdx = integerValue; break;
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
// It's called by process() and can also be called manually.
void TTLConditionalTrigger::pushStateToDisplay()
{
    // NOTE - The editor is already stored as the GenericProcessor class variable "editor".
    // We do need to re-cast it.
    TTLConditionalTriggerEditor* theEditor = (TTLConditionalTriggerEditor*) (editor.get());

    // NOTE - We're only pushing full config state when we _aren't_ running.
    // Check the "isEnabled" variable to determine this.

    if (!isEnabled)
    {
        // We're not running. Propagate the config state.
        // NOTE - This should rarely actually change anything, since we got our config from the UI.
        // The exception is if we loaded configuration from XML or otherwise got it from somewhere other than the user clicking buttons.
        int inMatrixPtr = 0;
        for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        {
            theEditor->pushOutputConfigToEditor(outIdx, outputConditions[outIdx].getConfig(), needAllInputs[outIdx]);
            for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
            {
                theEditor->pushInputConfigToEditor(inMatrixPtr, inputConditions[inMatrixPtr].getConfig());
                inMatrixPtr++;
            }
        }
    }

    // Always propagate running state.

    bool rawInputs[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
    bool cookedInputs[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
    bool outputState[TTLCONDTRIG_OUTPUTS];
    bool outputsEnabled[TTLCONDTRIG_OUTPUTS];

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputState[outIdx] = outputConditions[outIdx].getLastAcknowledgedOutput();
        outputsEnabled[outIdx] = outputConditions[outIdx].isEnabled();

        for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        {
            // NOTE - These are unconfigured defaults (no channel, active-high).
            // FIXME - A configured but disabled input _should_ show the input TTL value and, if active-_low_, a high output. But we can't easily test whether an input was ever configured.
            rawInputs[inMatrixPtr] = false;
            cookedInputs[inMatrixPtr] = false;

            if (inputConditions[inMatrixPtr].isEnabled())
            {
                rawInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastInput();
                cookedInputs[inMatrixPtr] = inputConditions[inMatrixPtr].getLastAcknowledgedOutput();
            }

            inMatrixPtr++;
        }
    }

    theEditor->pushRunningStateToEditor(rawInputs, cookedInputs, outputState, outputsEnabled);
}


// This is the end of the file.
