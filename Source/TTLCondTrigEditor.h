#ifndef TTLCONDTRIGEDITOR_H_DEFINED
#define TTLCONDTRIGEDITOR_H_DEFINED

#include <EditorHeaders.h>

namespace TTLConditionTrig
{
	class TTLConditionalTrigger;
	class ConditionConfig;
	class TTLConditionalTriggerEditor;


	// GUI elements for configuring one input.
	class TTLConditionalTriggerEditorInputRow : public Component, Button::Listener
	{
	public:
		TTLConditionalTriggerEditorInputRow(TTLConditionalTriggerEditor *newParent, int newInIdx);

		void buttonClicked(Button* theButton);

		void setInputLabel(std::string newLabel);
		void setLampState(bool wantLit);

		void setRunningState(bool isRunning);
		void setInputEnabled(bool isEnabled);

	protected:
		TTLConditionalTriggerEditor *parent;
		int inIdx;

		ScopedPointer<Image> settingsImage;
		ScopedPointer<Image> lampOnImage;
		ScopedPointer<Image> lampOffImage;

		ScopedPointer<ImageButton> settingsButton;

		// It's less expensive to have two images and make only one visible than it is to change the image on one component.
		ScopedPointer<ImageComponent> lampOnComponent;
		ScopedPointer<ImageComponent> lampOffComponent;

		ScopedPointer<Label> inputNameLabel;
	};


	// GUI panel for configuring a set of inputs associated with an output.
	class TTLConditionalTriggerEditorInputPanel : public Component
	{
	public:
		TTLConditionalTriggerEditorInputPanel(TTLConditionalTriggerEditor *newParent);

		void paint(Graphics& g);

		void setOutputLabel(std::string newLabel);
		void setInputLabel(int inIdx, std::string newLabel);
		void setLampState(int inIdx, bool wantLit);
		void setFillColour(Colour newColour);

		void setRunningState(bool isRunning);
		void setInputEnabled(int inIdx, bool isEnabled);

	protected:
		Colour backgroundColour;

		ScopedPointer<TTLConditionalTriggerEditorInputRow> inputRows[TTLCONDTRIG_INPUTS];
		ScopedPointer<Label> bannerLabel;
	};




	// GUI elements for configuring one output.
	class TTLConditionalTriggerEditorOutputRow : public Component, Button::Listener
	{
	public:
		TTLConditionalTriggerEditorOutputRow(TTLConditionalTriggerEditor *newParent, int newOutIdx);

		void buttonClicked(Button* theButton);

		void paint(Graphics& g);

		void setOutputLabel(std::string newLabel);
		void setLampState(bool wantLit);
		void setTabColour(Colour newColour);
		Colour getBackgroundColour();

		void setRunningState(bool isRunning);
		void setOutputEnabled(bool isEnabled);

	protected:
		TTLConditionalTriggerEditor *parent;
		int outIdx;
		Colour tabColourLight, tabColourDark, backColourLight, backColourDark;
		Colour currentBackgroundColour;

		ScopedPointer<Image> settingsImage;
		ScopedPointer<Image> connectOnImage;
		ScopedPointer<Image> connectOffImage;
		ScopedPointer<Image> lampOnImage;
		ScopedPointer<Image> lampOffImage;

		ScopedPointer<ImageButton> settingsButton;
		ScopedPointer<ImageButton> enableButton;
		ScopedPointer<ColorButton> tabButton;

		// It's less expensive to have two images and make only one visible than it is to change the image on one component.
		ScopedPointer<ImageComponent> lampOnComponent;
		ScopedPointer<ImageComponent> lampOffComponent;

		ScopedPointer<Label> outputNameLabel;
	};


	// GUI panel for configuring a set of inputs associated with an output.
	class TTLConditionalTriggerEditorOutputPanel : public Component
	{
	public:
		TTLConditionalTriggerEditorOutputPanel(TTLConditionalTriggerEditor *newParent);

		void setOutputLabel(int outIdx, std::string newLabel);
		void setLampState(int outIdx, bool wantLit);
		void setTabColour(int outIdx, Colour newColour);
		Colour getBackgroundColour(int outIdx);

		void setRunningState(bool isRunning);
		void setOutputEnabled(int outIdx, bool isEnabled);

	protected:
		ScopedPointer<TTLConditionalTriggerEditorOutputRow> outputRows[TTLCONDTRIG_OUTPUTS];
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
		// Most of the config state only gets updated when not running. Output enable is the exception.

		// NOTE - Pushing arguments by value rather than by reference to avoid multithreading issues.
		void pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig, std::string newLabel);
		void pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, bool newNeedAllInputs, std::string newLabel);
		// NOTE - Passing arrays by value involves shenanigans, but the caller's arrays should persist until this call returns, so we'll be okay.
		void pushRunningStateToEditor(bool (&rawInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&outputState)[TTLCONDTRIG_OUTPUTS], bool (&outputsEnabled)[TTLCONDTRIG_OUTPUTS]);

		// These are normally just called by the timer callback.
		void doConfigStateRedraw();
		void doRunningStateRedraw();

		// Accessors for changing GUI state.
		// These are called by input or output GUI widgets.
		void clickedInputSettings(int idxClicked);
		void clickedOutputEnableToggle(int idxClicked);
		void clickedOutputSettings(int idxClicked);
		void clickedOutputTab(int idxClicked);
		void clickedConditionExit();

	protected:
		TTLConditionalTrigger* parent;

		// We have our own local copies of configuration state.
		ConditionConfig inputConfig[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionConfig outputConfig[TTLCONDTRIG_OUTPUTS];
		bool needAllInputs[TTLCONDTRIG_OUTPUTS];
		std::string inputLabels[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		std::string outputLabels[TTLCONDTRIG_OUTPUTS];

		// We also have a cached copy of the input and output.
		bool inputLampState[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		bool outputLampState[TTLCONDTRIG_OUTPUTS];

		// GUI state variables.
		int outputSelectIdx; // Which output tab we've selected.
		int inputSelectIdx; // Which input we're editing.
		bool editingInput; // Are we adjusting input settings?
		bool editingOutput; // Are we adjusting output settings?
		bool wasRunningLastRedraw; // To check for enable/disable updates.

		// GUI elements.

		ScopedPointer<TTLConditionalTriggerEditorInputPanel> inputStatusPanel;
		ScopedPointer<TTLConditionalTriggerEditorOutputPanel> outputStatusPanel;
//		ScopedPointer<Component> conditionEditPanel;

		// FIXME - State information to render goes here.

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTriggerEditor);
	};
}


#endif

// This is the end of the file.
