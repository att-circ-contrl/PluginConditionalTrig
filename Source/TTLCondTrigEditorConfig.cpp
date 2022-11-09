#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

#define TRIGDEBUGPREFIX "[CondTrigConfig]  "
#include "TTLCondTrigDebug.h"

using namespace TTLConditionTrig;


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
    thisEnabled = false;
    inputChanIdx = 0;
    inputBitIdx = 0;

    thisInputLabel = "bogus";
    thisOutputLabel = "bogus";


    // Create GUI elements.
    // NOTE - Leaving decorative labels enabled. Disabling greys them out; instead just ignore clicks.

    int xpos = 0;
    int ypos = TTLCONDTRIG_YGAP;

    // Banner row.

    bannerLeftLabel = new Label("", "undefined");
    bannerLeftLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGBANNER_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(bannerLeftLabel);
    xpos += TTLCONDTRIG_CONFIGBANNER_XSIZE + TTLCONDTRIG_XGAP;

    bannerRightLabel = new Label("", "Label:");
    bannerRightLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGLABELNOTE_XSIZE, TTLCONDTRIG_YSIZE);
    bannerRightLabel->setJustificationType(Justification::centredRight);
    addAndMakeVisible(bannerRightLabel);
    xpos += TTLCONDTRIG_CONFIGLABELNOTE_XSIZE + TTLCONDTRIG_XGAP;

    bannerEditLabel = new Label("User Label", "undefined");
    bannerEditLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGLABEL_XSIZE, TTLCONDTRIG_YSIZE);
    bannerEditLabel->setEditable(true);
    bannerEditLabel->setColour(Label::ColourIds::backgroundColourId, TEXTEDIT_NORMAL);
    bannerEditLabel->setColour(Label::ColourIds::backgroundWhenEditingColourId, TEXTEDIT_ACTIVE);
    bannerEditLabel->addListener(this);
    addAndMakeVisible(bannerEditLabel);

    xpos = TTLCONDTRIG_CONFIGPANEL_XSIZE - TTLCONDTRIG_CONFIGDONE_XSIZE;
    doneButton = new UtilityButton("Done", Font("Small Text", 13, Font::plain));
    doneButton->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGDONE_XSIZE, TTLCONDTRIG_YSIZE);
    doneButton->addListener(this);
    addAndMakeVisible(doneButton);

    // Feature/enable/source row.

    connectOnImage = new Connected16Image(CONN_BACKGROUND, CONN_FOREGROUND);
    connectOffImage = new Disconnected16Image(DISCONN_BACKGROUND, DISCONN_FOREGROUND);

    xpos = TTLCONDTRIG_XGAP;
    ypos += TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP;

    enableButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    enableButton->setImages(true, true, true, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOnImage, 0.5, COLOUR_TRANSPARENT);
    enableButton->setBounds(xpos, ypos, TTLCONDTRIG_CONN_SIZE, TTLCONDTRIG_CONN_SIZE);
    enableButton->setClickingTogglesState(true);
    enableButton->addListener(this);
    addAndMakeVisible(enableButton);
    xpos += TTLCONDTRIG_CONN_SIZE;

    enabledLabel = new Label("", "Disabled");
    enabledLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGENABLELABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(enabledLabel);
    xpos += TTLCONDTRIG_CONFIGENABLELABEL_XSIZE + TTLCONDTRIG_XGAP;

    inputFeatureLabel = new Label("", "Want");
    inputFeatureLabel->setJustificationType(Justification::centredRight);
    inputFeatureLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGFEATURELABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputFeatureLabel);
    xpos += TTLCONDTRIG_CONFIGFEATURELABEL_XSIZE;

    inputFeatureBox = new ComboBox;
    inputFeatureBox->addItem("high", ConditionConfig::levelHigh + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputFeatureBox->addItem("low", ConditionConfig::levelLow + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputFeatureBox->addItem("rising", ConditionConfig::edgeRising + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputFeatureBox->addItem("falling", ConditionConfig::edgeFalling + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputFeatureBox->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGFEATURE_XSIZE, TTLCONDTRIG_YSIZE);
    inputFeatureBox->addListener(this);
    addAndMakeVisible(inputFeatureBox);
    xpos += TTLCONDTRIG_CONFIGFEATURE_XSIZE + TTLCONDTRIG_XGAP;

    inputChanLabel = new Label("", "from bank");
    inputChanLabel->setJustificationType(Justification::centredRight);
    inputChanLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGBANKLABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputChanLabel);
    xpos += TTLCONDTRIG_CONFIGBANKLABEL_XSIZE;

    inputChanBox = new ComboBox;
    inputChanBox->addItem("none", TTLCONDTRIG_CHANIDX_NONE + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputChanBox->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGBANK_XSIZE, TTLCONDTRIG_YSIZE);
    inputChanBox->addListener(this);
    addAndMakeVisible(inputChanBox);
    xpos += TTLCONDTRIG_CONFIGBANK_XSIZE + TTLCONDTRIG_XGAP;

    inputBitLabel = new Label("", "bit");
    inputBitLabel->setJustificationType(Justification::centredRight);
    inputBitLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGBITLABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputBitLabel);
    xpos += TTLCONDTRIG_CONFIGBITLABEL_XSIZE;

    inputBitBox = new ComboBox;
    inputBitBox->addItem("none", TTLCONDTRIG_BITIDX_NONE + TTLCONDTRIG_COMBOBOX_OFFSET);
    inputBitBox->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGBIT_XSIZE, TTLCONDTRIG_YSIZE);
    inputBitBox->addListener(this);
    addAndMakeVisible(inputBitBox);
    xpos += TTLCONDTRIG_CONFIGBIT_XSIZE + TTLCONDTRIG_XGAP;

    // Input timing row.

    xpos = TTLCONDTRIG_XGAP;
    ypos += TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP;

    inputTimeLeftLabel = new Label("", "Input stable for");
    inputTimeLeftLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGINTIMELEFT_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputTimeLeftLabel);
    xpos += TTLCONDTRIG_CONFIGINTIMELEFT_XSIZE + TTLCONDTRIG_XGAP;

    inputDeglitchLabel = new Label("","0");
    inputDeglitchLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGMS_XSIZE, TTLCONDTRIG_YSIZE);
    inputDeglitchLabel->setEditable(true);
    inputDeglitchLabel->setColour(Label::ColourIds::backgroundColourId, TEXTEDIT_NORMAL);
    inputDeglitchLabel->setColour(Label::ColourIds::backgroundWhenEditingColourId, TEXTEDIT_ACTIVE);
    inputDeglitchLabel->addListener(this);
    addAndMakeVisible(inputDeglitchLabel);
    xpos += TTLCONDTRIG_CONFIGMS_XSIZE;

    inputTimeMidLabel = new Label("", "ms, at least");
    inputTimeMidLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGINTIMEMID_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputTimeMidLabel);
    xpos += TTLCONDTRIG_CONFIGINTIMEMID_XSIZE + TTLCONDTRIG_XGAP;

    inputDeadtimeLabel = new Label("","0");
    inputDeadtimeLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGMS_XSIZE, TTLCONDTRIG_YSIZE);
    inputDeadtimeLabel->setEditable(true);
    inputDeadtimeLabel->setColour(Label::ColourIds::backgroundColourId, TEXTEDIT_NORMAL);
    inputDeadtimeLabel->setColour(Label::ColourIds::backgroundWhenEditingColourId, TEXTEDIT_ACTIVE);
    inputDeadtimeLabel->addListener(this);
    addAndMakeVisible(inputDeadtimeLabel);
    xpos += TTLCONDTRIG_CONFIGMS_XSIZE;

    inputTimeRightLabel = new Label("", "ms since last trigger");
    inputTimeRightLabel->setBounds(xpos, ypos, TTLCONDTRIG_CONFIGINTIMERIGHT_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputTimeRightLabel);
    xpos += TTLCONDTRIG_CONFIGINTIMERIGHT_XSIZE;

#if 0
ScopedPointer<Label> outputLeftLabel, outputMidLabel,
  outputSustainLabel, outputRightLabel;
ScopedPointer<ComboBox> outputSenseBox;
ScopedPointer<Label> outputJitterLeftLabel, outputJitterLowLabel,
  OutputJitterMidLabel, OutputJitterHighLabel, OutputJitterRightLabel;
#endif
// FIXME - Constructor NYI.
}


// Button click callback.
// This is the "done" button or the "enable" button.
void TTLConditionalTriggerEditorConfigPanel::buttonClicked(Button* theButton)
{
    if (theButton == doneButton)
        // The parent pulls configuration state here, and forwards it to the plugin.
        parent->clickedConditionExit();
    else if (theButton == enableButton)
    {
        thisEnabled = !thisEnabled;
        refreshGui();
    }
}


// Label editing callback.
// This is the channel label or one of several timing values.
void TTLConditionalTriggerEditorConfigPanel::labelTextChanged(Label* theLabel)
{
    String thisTextJuce = theLabel->getText();
    std::string thisText = thisTextJuce.toStdString();
    bool isOutput = (inIdx < 0);

    // Parse the label argument as an integer. This should return 0 for invalid input.
    int64 thisMilliseconds = std::stol(thisText);
    // Convert this into a duration in samples.
    int64 thisSampCount = (int64) CoreServices::getGlobalSampleRate();
    thisSampCount = (thisSampCount * thisMilliseconds) / 1000;

    if (theLabel == bannerEditLabel)
    {
        if (isOutput)
            thisOutputLabel = thisText;
        else
            thisInputLabel = thisText;
    }
    else if (theLabel == inputDeglitchLabel)
        thisConfig.deglitchSamps = thisSampCount;
    else if (theLabel == inputDeadtimeLabel)
        thisConfig.deadTimeSamps = thisSampCount;
// FIXME - labelTextChanged() NYI.

    // Sanity check any changed configuration parameters.
    thisConfig.forceSanity();

    refreshGui();
}


// Combo box selection callback.
// This is input feature, input bank/bit, or output sense.
void TTLConditionalTriggerEditorConfigPanel::comboBoxChanged(ComboBox* theBox)
{
    int selectedId = theBox->getSelectedId() - TTLCONDTRIG_COMBOBOX_OFFSET;

    if (theBox == inputFeatureBox)
    {
        thisConfig.desiredFeature = (ConditionConfig::FeatureType) selectedId;
        thisConfig.forceSanity();
    }
    else if (theBox == inputChanBox)
    {
        inputChanIdx = selectedId;
        rebuildBitSelect();
    }
    else if (theBox == inputBitBox)
    {
        inputBitIdx = selectedId;
    }
// FIXME - comboBoxChanged() NYI.
}


// This updates the contents of the channel selection combo box, and stores channel metadata.
void TTLConditionalTriggerEditorConfigPanel::rebuildChannelSelect(StringArray &newInBankNames, Array<int> &newInBankIndices, Array<int> &newInBankBits)
{
T_PRINT("rebuildChannelSelect() called.");

    // These will copy by value.
    inBankIndices = newInBankIndices;
    inBankBits = newInBankBits;

    // Rebuild the channel select combobox.
    inputChanBox->clear(dontSendNotification);
    inputChanBox->addItem("none", TTLCONDTRIG_CHANIDX_NONE + TTLCONDTRIG_COMBOBOX_OFFSET);
    for (int chanIdx = 0; chanIdx < inBankIndices.size(); chanIdx++)
    {
        int thisChanNum = inBankIndices[chanIdx];
        std::string thisChanLabel = std::to_string(thisChanNum) + "-";
        thisChanLabel += newInBankNames[chanIdx].toStdString();
        inputChanBox->addItem(thisChanLabel, thisChanNum + TTLCONDTRIG_COMBOBOX_OFFSET);
    }

    // Make sure we're still pointing towards a valid channel.
    if (!( inBankIndices.indexOf(inputChanIdx) >= 0 ))
        inputChanIdx = TTLCONDTRIG_CHANIDX_NONE;

    // Rebuild the bit select combobox.
    rebuildBitSelect();
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

    thisEnabled = newEnabled;
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

    // Propagate channel selection changes.
    rebuildBitSelect();
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

ConditionConfig TTLConditionalTriggerEditorConfigPanel::getConfig()
{
    return thisConfig;
}

bool TTLConditionalTriggerEditorConfigPanel::getEnabled()
{
    return thisEnabled;
}

int TTLConditionalTriggerEditorConfigPanel::getInputChan()
{
    return inputChanIdx;
}

int TTLConditionalTriggerEditorConfigPanel::getInputBit()
{
    return inputBitIdx;
}


// This rebuilds the bit-select combobox to match the selected input bank.

void TTLConditionalTriggerEditorConfigPanel::rebuildBitSelect()
{
T_PRINT("rebuildBitSelect() called.");

    inputBitBox->clear(dontSendNotification);
    inputBitBox->addItem("none", TTLCONDTRIG_BITIDX_NONE + TTLCONDTRIG_COMBOBOX_OFFSET);

    int lutIdx = inBankIndices.indexOf(inputChanIdx);
// FIXME - Diagnostics.
T_PRINT("chan idx " << inputChanIdx << "   bit idx " << inputBitIdx << " lut idx " << lutIdx);
    if (lutIdx >= 0)
    {
        int bitCount = inBankBits[lutIdx];
T_PRINT("Input bank " << inputChanIdx << " has " << bitCount << " bits.");

        for (int bitIdx = 0; bitIdx < bitCount; bitIdx++)
            inputBitBox->addItem(std::to_string(bitIdx), bitIdx + TTLCONDTRIG_COMBOBOX_OFFSET);

        // Make sure our bit selection is still a valid ID.
        if ((inputBitIdx < 0) || (inputBitIdx >= bitCount))
            inputBitIdx = TTLCONDTRIG_BITIDX_NONE;
    }
}


// This updates the contents of state-sensitive GUI elements.

void TTLConditionalTriggerEditorConfigPanel::refreshGui()
{
T_PRINT("refreshGui() called.");

    std::string scratchstr;
    bool isOutput = (inIdx < 0);

    int64 sampRate = (int64) CoreServices::getGlobalSampleRate();

    // Banner row.

    if (isOutput)
        scratchstr = "Configuring output " + std::to_string(outIdx) + ".";
    else
    {
        scratchstr = "Configuring input " + std::to_string(inIdx) + " for output ";
        scratchstr += ('A' + outIdx);
        scratchstr += ".";
    }
    bannerLeftLabel->setText(scratchstr, dontSendNotification);

    if (isOutput)
        bannerEditLabel->setText(thisOutputLabel, dontSendNotification);
    else
        bannerEditLabel->setText(thisInputLabel, dontSendNotification);

    // Feature/enable/source row.

    enableButton->setToggleState(thisEnabled, dontSendNotification);
    enabledLabel->setText( (thisEnabled ? "Enabled" : "Disabled"), dontSendNotification );

    inputFeatureBox->setSelectedId( thisConfig.desiredFeature + TTLCONDTRIG_COMBOBOX_OFFSET, dontSendNotification );
    inputChanBox->setSelectedId( inputChanIdx + TTLCONDTRIG_COMBOBOX_OFFSET, dontSendNotification );
    inputBitBox->setSelectedId( inputBitIdx + TTLCONDTRIG_COMBOBOX_OFFSET, dontSendNotification );

    inputChanLabel->setVisible(editingInput);
    inputChanLabel->setEnabled(editingInput);
    inputChanBox->setVisible(editingInput);
    inputChanBox->setEnabled(editingInput);
    inputBitLabel->setVisible(editingInput);
    inputBitLabel->setEnabled(editingInput);
    inputBitBox->setVisible(editingInput);
    inputBitBox->setEnabled(editingInput);

    // Input timing row.

    inputDeglitchLabel->setText( std::to_string((thisConfig.deglitchSamps * 1000) / sampRate), dontSendNotification );
    inputDeadtimeLabel->setText( std::to_string((thisConfig.deadTimeSamps * 1000) / sampRate), dontSendNotification );

    // Force a redraw.
    repaint();

    // FIXME - refreshGui() NYI.
}


//
// This is the end of the file.
