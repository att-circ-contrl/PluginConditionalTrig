#ifndef TTLCONDTRIG_H_DEFINED
#define TTLCONDTRIG_H_DEFINED

#include <ProcessorHeaders.h>

#include "TTLCondTrigCircBuf.h"
#include "TTLCondTrigLogic.h"


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

		// Rebuild external configuration information.
		// We detect available inputs here.
		void updateSettings() override;

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
		ConditionProcessor inputConditions[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionProcessor outputConditions[TTLCONDTRIG_OUTPUTS];

		// Additional input configuration.
		bool isInputEnabled[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputChanIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputBitIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];

		// Additional output configuration.
		bool isOutputEnabled[TTLCONDTRIG_OUTPUTS];
		bool needAllInputs[TTLCONDTRIG_OUTPUTS];

		// NOTE - The editor is responsible for labels. We don't cache them.

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTrigger);
	};
}

#endif


// This is the end of the file.
