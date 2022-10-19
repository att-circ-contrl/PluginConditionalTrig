#ifndef TTLCONDTRIGEDITOR_H_DEFINED
#define TTLCONDTRIGEDITOR_H_DEFINED

#include <EditorHeaders.h>

// Magic constants for GUI geometry.

// Magic constants for GUI colours.
#define WRENCH_FOREGROUND juce::Colours::black
#define WRENCH_BACKGROUND juce::Colours::white
#define WRENCH_TRANSPARENT juce::Colours::transparentWhite

// Magic constants for display refresh.
#define TTLCONDTRIG_DISPLAY_REFRESH_MS 20

namespace TTLConditionTrig
{
	class TTLConditionalTrigger;


	// Settings button image.
	class WrenchImage : public Image
	{
	public:
		// Constructor.
		WrenchImage();
		// Inherited destructor is fine.
	};


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
		// FIXME - Placeholder. This needs to take arguments.
		void pushStateToEditor();
		void doStateUpdateRedraw();

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
