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
		// FIXME - Placeholder. This needs to take arguments.
		void pushConfigStateToEditor();
		void pushRunningStateToEditor();
		void doConfigStateRedraw();
		void doRunningStateRedraw();

	private:
		TTLConditionalTrigger* parent;

		ScopedPointer<Image> settingImage;
// FIXME - Testing.
ScopedPointer<ImageButton> dummyButton;

		// FIXME - State information to render goes here.

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTriggerEditor);
	};
}


#endif

// This is the end of the file.
