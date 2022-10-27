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

// Magic constants for parameter indices.
// Making this an enum isn't any cleaner, since we aren't guaranteed sequential values.
#define TTLCONDTRIG_PARAM_IS_ENABLED 0
#define TTLCONDTRIG_PARAM_CHAN_IDX 1
#define TTLCONDTRIG_PARAM_BIT_IDX 2
#define TTLCONDTRIG_PARAM_DELAY_MIN 3
#define TTLCONDTRIG_PARAM_DELAY_MAX 4
#define TTLCONDTRIG_PARAM_SUSTAIN 5
#define TTLCONDTRIG_PARAM_DEADTIME 6
#define TTLCONDTRIG_PARAM_DEGLITCH 7
#define TTLCONDTRIG_PARAM_OUTSENSE 8
// This should be equal to or greater than the number of per-channel parameters.
#define TTLCONDTRIG_PARAM_STRIDE 10

#define TTLCONDTRIG_STRIDE_INPUT TTLCONDTRIG_PARAM_STRIDE
#define TTLCONDTRIG_STRIDE_OUTPUT (TTLCONDTRIG_INPUTS * TTLCONDTRIG_STRIDE_INPUT)

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
		int64 delay_min_samps, delay_max_samps;
		int64 sustain_samps;
		int64 dead_time_samps;
		int64 deglitch_samps;
		bool output_active_high;
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
		void resetInput(int64 resetTime, bool resetInput);

		void handleInput(int64 inputTime, bool inputLevel);

		bool hasPendingOutput();
		int64 getNextOutputTime();
		bool getNextOutputLevel();
                void acknowledgeOutput();

	protected:
		ConditionConfig config;
		int64 prevInputTime;
		bool prevInputLevel;
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
		void setParamByChan(int outputIdx, int inputIdx, int paramIdx, long newValue);

		// This propagates state to the display.
		// It's called by process() and can also be called manually.
		void pushStateToDisplay();

	protected:
		// There shouldn't be anything dynamically allocated in here, and thread safety should already by guaranteed by setParameter().
		// So, C++ arrays rather than Array/OwnedArray should be fine.
		ConditionProcessor inputConditions[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionProcessor outputConditions[TTLCONDTRIG_OUTPUTS];

		// We're declared non-copyable, so internal dynamic allocation is fine for these.
		std::string inputLabels[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		std::string outputLabels[TTLCONDTRIG_OUTPUTS];

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTrigger);
	};
}

#endif


// This is the end of the file.
