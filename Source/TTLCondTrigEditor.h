#ifndef TTLCONDTRIGEDITOR_H_DEFINED
#define TTLCONDTRIGEDITOR_H_DEFINED

#include <EditorHeaders.h>
#include "TTLCondTrigIcons.h"

// Magic constants for GUI geometry.

// Magic constants for GUI colours.
#define COLOUR_TRANSPARENT juce::Colours::transparentWhite

// Magic constants for display refresh.
#define TTLCONDTRIG_DISPLAY_REFRESH_MS 20

namespace TTLConditionTrig
{
	class TTLConditionalTrigger;
	class ConditionConfig;


	// GUI tray for conditional trigger display and configuration.
	// NOTE - GenericEditor already inherits from Timer.
	class TTLConditionalTriggerEditor : public GenericEditor
	{
	public:
		// Constructor and destructor.
		TTLConditionalTriggerEditor(TTLConditionalTrigger* newParent);
		~TTLConditionalTriggerEditor();

		// Plugin hooks.
		// FIXME - UpdateSettings() goes here.
		// We could alternatively get the channel list via push.

		// Timer hooks.
		void timerCallback();

		// Accessors.
		// NOTE - The plugin has to push data to us, rather than us pulling it.
		// Most of the config state only gets updated when not running. Output enable is the exception.

		// NOTE - Pushing arguments by value rather than by reference to avoid multithreading issues.
		void pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig, std::string newLabel);
		void pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, std::string newLabel);
		// NOTE - Passing arrays by value involves shenanigans, but the caller's arrays should persist until this call returns, so we'll be okay.
		void pushRunningStateToEditor(bool (&rawInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&outputState)[TTLCONDTRIG_OUTPUTS], bool (&outputsEnabled)[TTLCONDTRIG_OUTPUTS]);

		// These are normally just called by the timer callback.
		void doConfigStateRedraw();
		void doRunningStateRedraw();

	protected:
		TTLConditionalTrigger* parent;

		ScopedPointer<Image> settingsImage;
		ScopedPointer<Image> connectImage;
		ScopedPointer<Image> disconnectImage;
		ScopedPointer<Image> lampOnImage;
		ScopedPointer<Image> lampOffImage;

		// We have our own local copies of configuration state.
		ConditionConfig inputConfig[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionConfig outputConfig[TTLCONDTRIG_OUTPUTS];
		std::string inputLabels[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		std::string outputLabels[TTLCONDTRIG_OUTPUTS];

		// We also have a cached copy of the input and output.
		bool inputLampState[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		bool outputLampState[TTLCONDTRIG_OUTPUTS];

// FIXME - Testing.
ScopedPointer<ImageButton> dummyButton;

		// FIXME - State information to render goes here.

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTriggerEditor);
	};
}


#endif

// This is the end of the file.
