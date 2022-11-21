#ifndef TTLCONDTRIG_H_DEFINED
#define TTLCONDTRIG_H_DEFINED

#include <ProcessorHeaders.h>

#include "TTLTools.h"


// Magic constants for data geometry.
#define TTLCONDTRIG_INPUTS 4
#define TTLCONDTRIG_OUTPUTS 4

// Magic constant: maximum number of pending TTL events in a single bit-line.
// Making this a power of 2 _should_ be faster but isn't vital.
#define TTLCONDTRIG_EVENT_BUF_SIZE 16384


// Class declarations.
namespace TTLConditionTrig
{
	class TTLConditionalTrigger : public GenericProcessor
	{
	public:
		// Constructor and destructor.
		TTLConditionalTrigger();
		~TTLConditionalTrigger();

		// We do have a custom editor.
		bool hasEditor() const { return true; }
		AudioProcessorEditor* createEditor() override;

		// We don't rebuild input geometry information; the editor does.
		// We create outputs here.
		void createEventChannels() override;

		// Hook indicating acquisition is about to start.
		bool enable() override;

		// Processing loop.
		void process(AudioSampleBuffer& buffer) override;

		// TTL event handling.
		void handleEvent(const EventChannel* eventInfo, const MidiMessage& event, int samplePosition) override;

		// Hook for modifying state variables.
		// Variables used by "process" should only be modified here.
		void setParameter(int parameterIndex, float newValue) override;

		// XML configuration hooks.
		void saveCustomParametersToXml(XmlElement* parentElement) override;
		void loadCustomParametersFromXml() override;

		// Accessors for querying and modifying state.
		// We need to do this indirectly for thread safety.

		// This is a wrapper for setParameter.
		void setInputParamByChan(int outputIdx, int inputIdx, int paramIdx, long newValue);
		void setOutputParamByChan(int outputIdx, int paramIdx, long newValue);

		// This propagates state to the display.
		// The "running" version is called by process(). The full version is called by the editor when we're not running.
		void pushRunningStateToDisplay();
		void pushFullStateToDisplay();

	protected:
		// There shouldn't be anything dynamically allocated in here, and thread safety should already by guaranteed by setParameter().
		// So, C++ arrays rather than Array/OwnedArray should be fine.
		TTLTools::ConditionProcessor inputConditions[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		TTLTools::LogicMerger outputMergers[TTLCONDTRIG_OUTPUTS];
		TTLTools::ConditionProcessor outputConditions[TTLCONDTRIG_OUTPUTS];
		TTLTools::MuxMerger outputSerializer;

		// Additional input configuration.
		bool isInputEnabled[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputChanIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputBitIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];

		// Additional output configuration.
		bool isOutputEnabled[TTLCONDTRIG_OUTPUTS];
		bool needAllInputs[TTLCONDTRIG_OUTPUTS];

		// Initialization state.
		bool initHappened;
		bool enableHappened;

		// NOTE - The editor is responsible for labels. We don't cache them.

		// NOTE - This is owned by Open Ephys, not us. So it can't be a ScopedPointer.
		EventChannel *outputEventChan;

		// Helper functions for saving/loading condition configurations.
		void saveLogicToXml(XmlElement* theTag, TTLTools::ConditionConfig theConfig);
		TTLTools::ConditionConfig loadLogicFromXml(XmlElement* theTag);

		// Helper function for rebuilding outputMerger configurations.
		void rebuildMergeConfig(int outIdx);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTrigger);
	};
}

#endif


// This is the end of the file.
