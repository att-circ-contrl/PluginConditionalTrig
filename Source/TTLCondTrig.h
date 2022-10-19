#ifndef TTLCONDTRIG_H_DEFINED
#define TTLCONDTRIG_H_DEFINED

#include <ProcessorHeaders.h>

// Magic constants for data geometry.
#define TTLCONDTRIG_INPUTS 4
#define TTLCONDTRIG_OUTPUTS 4

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
#define TTLCONDTRIG_STRIDE_OUTPUT (TTLCONDTRIG_INPUTS * TTLCONDTRIG_PARAM_STRIDE)

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
		void setParamByChan(int outputIdx, int inputIdx, int paramIdx, long newValue);

		// This propagates state to the display.
		// It's called by process() and can also be called manually.
		void pushStateToDisplay();

	protected:
		Array<bool> inputEnabled;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTrigger);
	};
}

#endif


// This is the end of the file.
