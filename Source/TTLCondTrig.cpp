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
// TTL conditional trigger logic.


// Constructor.
TTLConditionalTrigger::TTLConditionalTrigger() : GenericProcessor("TTL Conditional Trigger")
{
T_PRINT("Constructor called.");

    inputEnabled.clear();
    inputEnabled.insertMultiple(0, false, TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS);

    // NOTE - Per Josh, we need to set the processor type here in addition to
    // reporting it from getLibInfo().
    setProcessorType(PluginProcessorType::PROCESSOR_TYPE_FILTER);
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
}


// Processing loop.
void TTLConditionalTrigger::process(AudioSampleBuffer& buffer)
{
    // Nothing to do! Just handle events.
    checkForEvents();
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
