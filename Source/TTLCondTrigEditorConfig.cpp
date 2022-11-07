#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

using namespace TTLConditionTrig;

// Diagnostic tattle macros.
#define TRIGEDITTATTLE
#ifdef TRIGEDITTATTLE
#define T_DEBUG(x) do { x } while(false);
#else
#define T_DEBUG(x) {}
#endif
// Flushing should already happen with std::endl, but force it anyways.
#define T_PRINT(x) T_DEBUG(std::cout << "[CondTrigEditor]  " << x << std::endl << std::flush;)


//
//
// GUI panel for adjusting one input or output's settings.


// Constructor.
TTLConditionalTriggerEditorConfigPanel::TTLConditionalTriggerEditorConfigPanel(TTLConditionalTriggerEditor *newParent)
{
    parent = newParent;

    // Initialize to sane state.

    inIdx = 0;
    outIdx = 0;
    editingInput = true;

    inBankIndices.clear();
    inBankBits.clear();

    thisConfig.clear();
    inputEnabled = false;
    inputChanIdx = 0;
    inputBitIdx = 0;

    thisInputLabel = "bogus";
    thisOutputLabel = "bogus";


    // Create GUI elements.
    // NOTE - Leaving decorative labels enabled. Disabling greys them out; instead just ignore clicks.

    bannerLeftLabel = new Label("Config Banner", "undefined");
    bannerLeftLabel->setBounds(0, TTLCONDTRIG_YGAP, TTLCONDTRIG_CONFIGBANNER_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(bannerLeftLabel);

    bannerRightLabel = new Label("", "Label:");
    bannerRightLabel->setBounds(TTLCONDTRIG_CONFIGBANNER_XSIZE + TTLCONDTRIG_XGAP, TTLCONDTRIG_YGAP, TTLCONDTRIG_CONFIGLABELNOTE_XSIZE, TTLCONDTRIG_YSIZE);
    bannerRightLabel->setJustificationType(Justification::centredRight);
    addAndMakeVisible(bannerRightLabel);

    bannerEditLabel = new Label("User Label", "undefined");
    bannerEditLabel->setBounds(TTLCONDTRIG_CONFIGBANNER_XSIZE + TTLCONDTRIG_CONFIGLABELNOTE_XSIZE + TTLCONDTRIG_XGAP*2, TTLCONDTRIG_YGAP, TTLCONDTRIG_CONFIGLABEL_XSIZE, TTLCONDTRIG_YSIZE);
    bannerEditLabel->setEditable(true);
    bannerEditLabel->setColour(Label::ColourIds::backgroundColourId, TEXTEDIT_NORMAL);
    bannerEditLabel->setColour(Label::ColourIds::backgroundWhenEditingColourId, TEXTEDIT_ACTIVE);
    addAndMakeVisible(bannerEditLabel);

    doneButton = new UtilityButton("Done", Font("Small Text", 13, Font::plain));
    doneButton->setBounds(TTLCONDTRIG_CONFIGPANEL_XSIZE - TTLCONDTRIG_CONFIGDONE_XSIZE, TTLCONDTRIG_YGAP, TTLCONDTRIG_CONFIGDONE_XSIZE, TTLCONDTRIG_YSIZE);
    doneButton->addListener(this);
    addAndMakeVisible(doneButton);

#if 0
ScopedPointer<Label> bannerLeftLabel, bannerEditLabel, bannerRightLabel;
ScopedPointer<UtilityButton> doneButton;
ScopedPointer<Label> inputFeatureLabel, inputBitLabel, inputChanLabel;
ScopedPointer<ComboBox> inputFeatureBox, inputBitBox, inputChanBox;
ScopedPointer<Label> inputTimeLeftLabel, inputDeglitchLabel,
  inputTimeMidLabel, inputDeadtimeLabel, InputTimeRightLabel;
ScopedPointer<Label> outputLeftLabel, outputMidLabel,
  outputSustainLabel, outputRightLabel;
ScopedPointer<ComboBox> outputSenseBox;
ScopedPointer<Label> outputJitterLeftLabel, outputJitterLowLabel,
  OutputJitterMidLabel, OutputJitterHighLabel, OutputJitterRightLabel;
#endif
// FIXME - Constructor NYI.
}


// Button click callback.
// This is the "done" button.
void TTLConditionalTriggerEditorConfigPanel::buttonClicked(Button* theButton)
{
// FIXME - Need to push new config state here.
// The parent pulls label information, but we need to push everything else to the plugin.
    parent->clickedConditionExit();
// FIXME - buttonClicked() NYI.
}


// Label editing callback.
// This is the channel label or one of several timing values.
void TTLConditionalTriggerEditorConfigPanel::labelTextChanged(Label* theLabel)
{
// FIXME - labelTextChanged() NYI.
}


// Combo box selection callback.
// This is input feature, input bank/bit, or output sense.
void TTLConditionalTriggerEditorConfigPanel::comboBoxChanged(ComboBox* theBox)
{
// FIXME - comboBoxChanged() NYI.
}


// This updates the contents of the channel selection combo box, and stores channel metadata.
void TTLConditionalTriggerEditorConfigPanel::rebuildChannelSelect(StringArray &newInBankNames, Array<int> &newInBankIndices, Array<int> &newInBankBits)
{
    // These will copy by value.
    inBankIndices = newInBankIndices;
    inBankBits = newInBankBits;

// FIXME - rebuildChannelSelect() NYI.
}


// This initializes editing.
// Use an input index of "-1" when editing outputs.
void TTLConditionalTriggerEditorConfigPanel::setEditingState(int newInIdx, int newOutIdx, ConditionConfig &newConfig, bool newEnabled, int newChanIdx, int newBitIdx, std::string &newInputLabel, std::string &newOutputLabel)
{
T_PRINT("setEditingState() called for config " << newOutIdx << ":" << newInIdx << ".");
    // Store new configuration information.

    inIdx = newInIdx;
    outIdx = newOutIdx;

    thisConfig = newConfig;

    inputEnabled = newEnabled;
    inputChanIdx = newChanIdx;
    inputBitIdx = newBitIdx;

    thisInputLabel = newInputLabel;
    thisOutputLabel = newOutputLabel;

    editingInput = true;
    if (inIdx < 0)
    {
        // We're editing an output.
        inIdx = -1;
        editingInput = false;
    }

    if ((outIdx < 0) || (outIdx >= TTLCONDTRIG_OUTPUTS))
        outIdx = 0;

    if (inIdx >= TTLCONDTRIG_INPUTS)
        inIdx = 0;

    // Update GUI state to reflect the new configuration.
    refreshGui();
}


// These are to let the parent pull state when editing has finished.

std::string TTLConditionalTriggerEditorConfigPanel::getInputLabel()
{
    return thisInputLabel;
}

std::string TTLConditionalTriggerEditorConfigPanel::getOutputLabel()
{
    return thisOutputLabel;
}

int TTLConditionalTriggerEditorConfigPanel::getInIdxEdited()
{
    // This will be -1 if we were editing an output.
    return inIdx;
}

int TTLConditionalTriggerEditorConfigPanel::getOutIdxEdited()
{
    return outIdx;
}


// This updates the contents of state-sensitive GUI elements.

void TTLConditionalTriggerEditorConfigPanel::refreshGui()
{
T_PRINT("refreshGui() called.");

    std::string scratchstr;
    bool isOutput = (inIdx < 0);

    // Banner row.

    if (isOutput)
        scratchstr = "Configuring output " + std::to_string(outIdx) + ".";
    else
        scratchstr = "Configuring input " + std::to_string(inIdx) + " for output " + std::to_string(outIdx) + ".";
    bannerLeftLabel->setText(scratchstr, dontSendNotification);

    if (isOutput)
        bannerEditLabel->setText(thisOutputLabel, dontSendNotification);
    else
        bannerEditLabel->setText(thisInputLabel, dontSendNotification);

    // FIXME - refreshGui() NYI.
}


//
// This is the end of the file.
