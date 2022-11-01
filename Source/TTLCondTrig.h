#ifndef TTLCONDTRIG_H_DEFINED
#define TTLCONDTRIG_H_DEFINED

#include <ProcessorHeaders.h>

// NOTE - C++ compiles templated classes on-demand.
// The header both declares and implements them. Extra copies get pruned at link-time.
#include "TTLCondTrigTemplates.h"


// Magic constants for data geometry.
#define TTLCONDTRIG_INPUTS 4
#define TTLCONDTRIG_OUTPUTS 4

// Magic constant: maximum number of pending TTL events in a single bit-line.
// Making this a power of 2 _should_ be faster but isn't vital.
#define TTLCONDTRIG_EVENT_BUF_SIZE 16384


// Class declarations.
namespace TTLConditionTrig
{
	// Configuration for processing conditions on one signal.
	// Nothing in here is dynamically allocated, so copy-by-value is fine.
	class ConditionConfig
	{
	public:
		// Configuration parameters. External editing is fine.
		bool isEnabled;
		int64 delayMinSamps, delayMaxSamps;
		int64 sustainSamps;
		int64 deadTimeSamps;
		int64 deglitchSamps;
		bool outputActiveHigh;
		// Only valid for conditions on inputs.
		int chanIdx;
		int bitIdx;

		// Constructor.
		ConditionConfig();
		// Default destructor is fine.

		// This sets a known-sane configuration state.
		void clear();
	};


	// Condition processing for one TTL signal.
	class ConditionProcessor
	{
	public:
		// Constructor.
		ConditionProcessor();
		// Default destructor is fine.

		// Accessors.

		void setConfig(ConditionConfig &newConfig);
		ConditionConfig getConfig();
		void resetState();
		// Lightweight enable query/toggle (vs having to check the full configuration).
		bool isEnabled();
		void setEnabled(bool wantEnabled);

		void resetInput(int64 resetTime, bool newInput);
		void handleInput(int64 inputTime, bool inputLevel);

		bool hasPendingOutput();
		int64 getNextOutputTime();
		bool getNextOutputLevel();
                void acknowledgeOutput();

		// These are mostly to simplify display polling.
		bool getLastInput();
		bool getLastAcknowledgedOutput();

	protected:
		ConditionConfig config;
		int64 prevInputTime;
		bool prevInputLevel;
		bool prevAcknowledgedOutput;
		CircBuf<int64,TTLCONDTRIG_EVENT_BUF_SIZE> pendingOutputTimes;
		CircBuf<bool,TTLCONDTRIG_EVENT_BUF_SIZE> pendingOutputLevels;
	};


	// The plugin itself.
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
		// It's called by process() and can also be called manually.
		void pushStateToDisplay();

	protected:
		// There shouldn't be anything dynamically allocated in here, and thread safety should already by guaranteed by setParameter().
		// So, C++ arrays rather than Array/OwnedArray should be fine.
		ConditionProcessor inputConditions[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionProcessor outputConditions[TTLCONDTRIG_OUTPUTS];

		// This is the "need all"/"need any" switch for each output.
		bool needAllInputs[TTLCONDTRIG_OUTPUTS];

		// NOTE - The editor is responsible for labels. We don't cache them.

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTrigger);
	};
}

#endif


// This is the end of the file.
