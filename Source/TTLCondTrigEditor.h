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
		void setRawLampState(bool wantLit);
		void setCookedLampState(bool wantLit);

		void setRunningState(bool isRunning);
		// NOTE - This is what propagates label text to the GUI.
		void setInputEnabled(bool isEnabled);

	protected:
		TTLConditionalTriggerEditor *parent;
		int inIdx;
		std::string textInputName;

		ScopedPointer<Image> settingsImage;
		ScopedPointer<Image> lampGreenImage;
		ScopedPointer<Image> lampAmberImage;
		ScopedPointer<Image> lampOffImage;

		ScopedPointer<ImageButton> settingsButton;

		// It's less expensive to have two images and make only one visible than it is to change the image on one component.
		ScopedPointer<ImageComponent> lampGreenOnComponent;
		ScopedPointer<ImageComponent> lampGreenOffComponent;
		ScopedPointer<ImageComponent> lampAmberOnComponent;
		ScopedPointer<ImageComponent> lampAmberOffComponent;

		ScopedPointer<Label> inputNameLabel;
	};


	// GUI panel for configuring a set of inputs associated with an output.
	class TTLConditionalTriggerEditorInputPanel : public Component, Button::Listener
	{
	public:
		TTLConditionalTriggerEditorInputPanel(TTLConditionalTriggerEditor *newParent);

		void buttonClicked(Button* theButton);

		void paint(Graphics& g);

		void setOutputLabel(std::string newLabel);
		void setInputLabel(int inIdx, std::string newLabel);
		void setRawLampState(int inIdx, bool wantLit);
		void setCookedLampState(int inIdx, bool wantLit);
		void setAnyAllState(bool wantAll);
		void setFillColour(Colour newColour);

		void setRunningState(bool isRunning);
		void setInputEnabled(int inIdx, bool isEnabled);

	protected:
		TTLConditionalTriggerEditor *parent;

		Colour backgroundColour;

		ScopedPointer<TTLConditionalTriggerEditorInputRow> inputRows[TTLCONDTRIG_INPUTS];
		ScopedPointer<Label> bannerLabel;
		ScopedPointer<Label> textWantLabel;
		ScopedPointer<Label> textAnyAllLabel;
		ScopedPointer<UtilityButton> wantAnyAllButton;
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

		void paint(Graphics& g);

		void setOutputLabel(int outIdx, std::string newLabel);
		void setLampState(int outIdx, bool wantLit);
		void setTabColour(int outIdx, Colour newColour);
		Colour getBackgroundColour(int outIdx);

		void setRunningState(bool isRunning);
		void setOutputEnabled(int outIdx, bool isEnabled);

	protected:
		ScopedPointer<TTLConditionalTriggerEditorOutputRow> outputRows[TTLCONDTRIG_OUTPUTS];
	};


	// GUI panel for adjusting one input or output's settings.
	class TTLConditionalTriggerEditorConfigPanel : public Component, Button::Listener, Label::Listener, ComboBox::Listener
	{
	public:
		TTLConditionalTriggerEditorConfigPanel(TTLConditionalTriggerEditor *newParent);

		void buttonClicked(Button* theButton);
		void labelTextChanged(Label* theLabel);
		void comboBoxChanged(ComboBox* theBox);

		void rebuildChannelSelect(StringArray &newInBankNames, Array<int> &newInBankIndices, Array<int> &newInBankBits);
		// Use an input index of "-1" when editing outputs.
		void setEditingState(int newInIdx, int newOutIdx, ConditionConfig &newConfig, bool newEnabled, int newChanIdx, int newBitIdx, std::string &newInputLabel, std::string &newOutputLabel);

		// These are to let the parent pull state when editing has finished.
		std::string getInputLabel();
		std::string getOutputLabel();
		int getInIdxEdited();
		int getOutIdxEdited();

	protected:
		TTLConditionalTriggerEditor *parent;
		int inIdx, outIdx;
		bool editingInput;

		Array<int> inBankIndices;
		Array<int> inBankBits;

		ConditionConfig thisConfig;
		bool inputEnabled;
		int inputChanIdx;
		int inputBitIdx;
		std::string thisInputLabel, thisOutputLabel;

		ScopedPointer<Label> bannerLeftLabel, bannerEditLabel, bannerRightLabel;
		ScopedPointer<UtilityButton> doneButton;
		ScopedPointer<Label> inputFeatureLabel, inputBitLabel, inputChanLabel;
		ScopedPointer<ComboBox> inputFeatureBox, inputBitBox, inputChanBox;
		ScopedPointer<Label> inputTimeLeftLabel, inputDeglitchLabel, inputTimeMidLabel, inputDeadtimeLabel, InputTimeRightLabel;
		ScopedPointer<Label> outputLeftLabel, outputMidLabel, outputSustainLabel, outputRightLabel;
		ScopedPointer<ComboBox> outputSenseBox;
		ScopedPointer<Label> outputJitterLeftLabel, outputJitterLowLabel, OutputJitterMidLabel, OutputJitterHighLabel, OutputJitterRightLabel;
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
		// We pull the list of TTL inputs here.
		void updateSettings() override;

		// Timer hooks.
		void timerCallback();

		// XML configuration hooks.
		void saveCustomParameters(XmlElement* xml) override;
		void loadCustomParameters(XmlElement* xml) override;

		// Accessors.
		// NOTE - The plugin has to push data to us, rather than us pulling it.
		// Most of the config state only gets updated when not running. Output enable is the exception.
		// NOTE - Pushing arguments by value rather than by reference to avoid multithreading issues.
		void pushInputConfigToEditor(int inMatrixIdx, ConditionConfig newConfig, bool wantEnabled, int newChanIdx, int newBitIdx);
		void pushOutputConfigToEditor(int outIdx, ConditionConfig newConfig, bool wantEnabled, bool newNeedAllInputs);
		// NOTE - Passing arrays by value involves shenanigans, but the caller's arrays should persist until this call returns, so we'll be okay.
		void pushRunningStateToEditor(bool (&rawInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&cookedInputs)[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS], bool (&outputState)[TTLCONDTRIG_OUTPUTS], bool (&outputsEnabled)[TTLCONDTRIG_OUTPUTS]);
		// NOTE - The plugin pulls label strings from us, since we generate them and they can't be pushed via setParameter().
                std::string getOutputLabel(int outIdx);

		// Accessors for changing GUI state.
		// These are called by input or output GUI widgets.
		void clickedInputSettings(int idxClicked);
		void clickedAnyAll();
		void clickedOutputEnableToggle(int idxClicked);
		void clickedOutputSettings(int idxClicked);
		void clickedOutputTab(int idxClicked);
		void clickedConditionExit();

	protected:
		TTLConditionalTrigger* parent;

		// Metadata for input channels.
		StringArray inBankNames;
		Array<int> inBankIndices;
		Array<int> inBankBits;

		// We have our own local copies of configuration state.
		ConditionConfig inputConfig[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		ConditionConfig outputConfig[TTLCONDTRIG_OUTPUTS];
		bool isInputEnabled[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputChanIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		int inputBitIdx[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		bool isOutputEnabled[TTLCONDTRIG_OUTPUTS];
		bool needAllInputs[TTLCONDTRIG_OUTPUTS];
		// We have the master copies of the user-supplied labels.
		std::string inputLabels[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		std::string outputLabels[TTLCONDTRIG_OUTPUTS];

		// We also have a cached copy of the input and output.
		bool inputRawLampState[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		bool inputCookedLampState[TTLCONDTRIG_INPUTS * TTLCONDTRIG_OUTPUTS];
		bool outputLampState[TTLCONDTRIG_OUTPUTS];

		// GUI state variables.
		int outputSelectIdx; // Which output tab we've selected.
		int inputSelectIdx; // Which input we're editing.
		bool wasRunningLastRedraw; // To check for enable/disable updates.

		// GUI elements.

		ScopedPointer<TTLConditionalTriggerEditorInputPanel> inputStatusPanel;
		ScopedPointer<TTLConditionalTriggerEditorOutputPanel> outputStatusPanel;
		ScopedPointer<TTLConditionalTriggerEditorConfigPanel> configPanel;


		// Internal accessors.

		// These may be called by the timer callback or after GUI events that change configuration.
		void propagateRunningElementConfig();
		void propagateInputPaneConfig();
		void propagateOutputPaneConfig();

		// These are called after possible input geometry changes.
		void rebuildInputDialog();
		void sanityCheckInputs();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLConditionalTriggerEditor);
	};
}


#endif

// This is the end of the file.
