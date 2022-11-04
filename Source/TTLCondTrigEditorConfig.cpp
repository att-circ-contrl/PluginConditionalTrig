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

// FIXME - setEditingState() NYI!
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


//
// This is the end of the file.
