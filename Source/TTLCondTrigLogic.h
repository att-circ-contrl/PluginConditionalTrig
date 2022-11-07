#ifndef TTLCONDTRIGLOGIC_H_DEFINED
#define TTLCONDTRIGLOGIC_H_DEFINED

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
		enum FeatureType
		{
			levelHigh = 0,
			levelLow = 1,
			edgeRising = 2,
			edgeFalling = 3
		};

		// Configuration parameters. External editing is fine.
		FeatureType desiredFeature;
		int64 delayMinSamps, delayMaxSamps;
		int64 sustainSamps;
		int64 deadTimeSamps;
		int64 deglitchSamps;
		bool outputActiveHigh;

		// Constructor.
		ConditionConfig();
		// Default destructor is fine.

		// This sets a known-sane configuration state.
		void clear();
		// This forces configuration parameters to be valid and self-consistent.
		void forceSanity();
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
}

#endif


// This is the end of the file.
