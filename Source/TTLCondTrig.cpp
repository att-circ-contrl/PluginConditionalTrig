#include "TTLCondTrigEditor.h"
#include "TTLCondTrig.h"

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

    label = "unnamed";

    isEnabled = false;

    delay_min_samps = 0;
    delay_max_samps = 0;

    sustain_samps = 10;

    dead_time_samps = 100;

    deglitch_samps = 0;

    output_active_high = true;

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

    // NOTE - We're clearing pending state but keeping the record of previous input.
    resetState();
}


ConditionConfig ConditionProcessor::getConfig()
{
    return config;
}


// This overwrites our record of the previous input levels without causing an event update.
// This is used for initialization at the start of acquisition.
void ConditionProcessor::resetInput(int64 resetTime, bool resetInput)
{
    prevTime = resetTime;
    prevLevel = resetInput;
}


// State reset. This clears active events after a configuration change.
void ConditionProcessor::resetState()
{
    pendingTimes.clear();
    pendingLevels.clear();
}


// Input processing. This schedules future output in response to input events.
void ConditionProcessor::handleInput(int64 inputTime, bool inputLevel)
{
// FIXME - handleInput NYI.
}


// State accessors.

bool ConditionProcessor::hasPendingOutput()
{
    return (pendingTimes.count() > 0);
}


int64 ConditionProcessor::getNextOutputTime()
{
    // NOTE - This will return a safe value (0) if we don't have output.
    return pendingTimes.snoop();
}


bool ConditionProcessor::getNextOutputLevel()
{
    // NOTE - This will return a safe value (false) if we don't have output.
    return pendingLevels.snoop();
}


// This removes the next queued output event, after we've read it.
void ConditionProcessor::acknowledgeOutput()
{
    // Discard return values.
    pendingTimes.dequeue();
    pendingLevels.dequeue();
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


    // These should already have been initialized by their constructors, but do it explicitly just to be safe.

    ConditionConfig dummyConfig;
    dummyConfig.clear();

    int inMatrixPtr = 0;
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputConditions[outIdx].setConfig(dummyConfig);
        outputConditions[outIdx].resetInput(0, false);
        outputConditions[outIdx].resetState();

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

T_PRINT( "setParameter() called setting " << parameterIndex << " to: " << integerValue );

// FIXME - setParameter() NYI.
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


// Accessor for modifying state.
// This is a wrapper for setParameter.
void TTLConditionalTrigger::setParamByChan(int outputIdx, int inputIdx, int paramIdx, long newValue)
{
  int packedIdx = paramIdx;
  packedIdx += inputIdx * TTLCONDTRIG_STRIDE_INPUT;
  packedIdx += outputIdx * TTLCONDTRIG_STRIDE_OUTPUT;

  setParameter(packedIdx, newValue);
}


// This propagates state to the display.
// It's called by process() and can also be called manually.
void TTLConditionalTrigger::pushStateToDisplay()
{
    // NOTE - The editor is already stored as the GenericProcessor class variable "editor".

// FIXME - pushStateToDisplay() NYI.
}


// This is the end of the file.
