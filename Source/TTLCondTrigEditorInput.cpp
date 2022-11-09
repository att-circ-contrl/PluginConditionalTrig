#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

#define TRIGDEBUGPREFIX "[CondTrigInput]  "
#include "TTLCondTrigDebug.h"

using namespace TTLConditionTrig;


//
//
// GUI row for status and config buttons for one input.


// Constructor.
TTLConditionalTriggerEditorInputRow::TTLConditionalTriggerEditorInputRow(TTLConditionalTriggerEditor* newParent, int newInIdx)
{
    parent = newParent;
    inIdx = newInIdx;
    textInputName = "undefined";

    // Indicator lamp images.
    lampGreenImage = new IndicatorLamp16Image(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_GREEN_FILL, LAMP_GREEN_HIGHLIGHT);
    lampAmberImage = new IndicatorLamp16Image(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_AMBER_FILL, LAMP_AMBER_HIGHLIGHT);
    lampOffImage = new IndicatorLamp16Image(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);

    // Raw input indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampGreenOnComponent = new ImageComponent();
    lampGreenOnComponent->setImage(*lampGreenImage);

    lampGreenOnComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampGreenOnComponent);
    lampGreenOnComponent->setEnabled(false);
    lampGreenOnComponent->setVisible(false);

    lampGreenOffComponent = new ImageComponent();
    lampGreenOffComponent->setImage(*lampOffImage);

    lampGreenOffComponent->setBounds(0, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampGreenOffComponent);
    lampGreenOffComponent->setEnabled(false);

    // Input name label.
    inputNameLabel = new Label("Input Name", textInputName);
    inputNameLabel->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_XGAP, 0, TTLCONDTRIG_LABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(inputNameLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    inputNameLabel->setEnabled(false);

    // Settings button.
    settingsImage = new Wrench16Image(WRENCH_BACKGROUND, WRENCH_FOREGROUND);
    settingsButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    settingsButton->setImages(true, true, true, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 0.5, COLOUR_TRANSPARENT);
    settingsButton->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP*2, 0, TTLCONDTRIG_WRENCH_SIZE, TTLCONDTRIG_WRENCH_SIZE);
    settingsButton->addListener(this);
    addAndMakeVisible(settingsButton);

    // Cooked input indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampAmberOnComponent = new ImageComponent();
    lampAmberOnComponent->setImage(*lampAmberImage);

    lampAmberOnComponent->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampAmberOnComponent);
    lampAmberOnComponent->setEnabled(false);
    lampAmberOnComponent->setVisible(false);

    lampAmberOffComponent = new ImageComponent();
    lampAmberOffComponent->setImage(*lampOffImage);

    lampAmberOffComponent->setBounds(TTLCONDTRIG_LAMP_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampAmberOffComponent);
    lampAmberOffComponent->setEnabled(false);

    // Force sane state.
    setInputEnabled(false);
    setRunningState(false);
}


// Callback for button presses. This is just the settings button.
void TTLConditionalTriggerEditorInputRow::buttonClicked(Button *theButton)
{
    // There's only one button to click.
    parent->clickedInputSettings(inIdx);
}


// Accessors.

void TTLConditionalTriggerEditorInputRow::setInputLabel(std::string newLabel)
{
    textInputName = newLabel;
    // NOTE - Don't update the label here. Let setInputEnabled() do it.
//    inputNameLabel->setText(newLabel, dontSendNotification);
}


void TTLConditionalTriggerEditorInputRow::setRawLampState(bool wantLit)
{
    lampGreenOnComponent->setVisible(wantLit);
    lampGreenOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorInputRow::setCookedLampState(bool wantLit)
{
    lampAmberOnComponent->setVisible(wantLit);
    lampAmberOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorInputRow::setRunningState(bool isRunning)
{
    // Lock out the settings button if we're running.
    settingsButton->setEnabled(!isRunning);
}


// NOTE - This is where input label text gets propagated to the GUI.
void TTLConditionalTriggerEditorInputRow::setInputEnabled(bool isEnabled)
{
    // If we're not using this input, disable the text (to grey it out) and set an appropriate label.
    inputNameLabel->setText( (isEnabled ? textInputName : "(unused)"), dontSendNotification );
    inputNameLabel->setEnabled(isEnabled);
}


//
//
// GUI panel for configuring a set of inputs associated with an output.


// Constructor.
TTLConditionalTriggerEditorInputPanel::TTLConditionalTriggerEditorInputPanel(TTLConditionalTriggerEditor* newParent)
{
    parent = newParent;

    backgroundColour = COLOUR_BOGUS;

    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
    {
        inputRows[inIdx] = new TTLConditionalTriggerEditorInputRow(newParent, inIdx);
        inputRows[inIdx]->setBounds(TTLCONDTRIG_XGAP, (TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP) * (inIdx+1) + TTLCONDTRIG_YGAP, TTLCONDTRIG_INROW_XSIZE, TTLCONDTRIG_YSIZE);
        addAndMakeVisible(inputRows[inIdx]);
    }

    bannerLabel = new Label("Panel Title", "undefined");
    bannerLabel->setBounds(TTLCONDTRIG_XGAP, TTLCONDTRIG_YGAP, TTLCONDTRIG_INPANEL_XSIZE - (TTLCONDTRIG_XGAP * 2), TTLCONDTRIG_YSIZE);
    addAndMakeVisible(bannerLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    bannerLabel->setEnabled(false);

    textWantLabel = new Label("AnyAll Banner", "Want");
    textWantLabel->setBounds(TTLCONDTRIG_INROW_XSIZE + TTLCONDTRIG_XGAP*2, TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP*2, TTLCONDTRIG_BOOLBUTTON_XSIZE, TTLCONDTRIG_YSIZE);
    textWantLabel->setJustificationType(Justification::centred);
    addAndMakeVisible(textWantLabel);

    textAnyAllLabel = new Label("AnyAll State", "---");
    textAnyAllLabel->setBounds(TTLCONDTRIG_INROW_XSIZE + TTLCONDTRIG_XGAP*2, TTLCONDTRIG_YSIZE*2 + TTLCONDTRIG_YGAP*3, TTLCONDTRIG_BOOLBUTTON_XSIZE, TTLCONDTRIG_YSIZE);
    textAnyAllLabel->setJustificationType(Justification::centred);
    addAndMakeVisible(textAnyAllLabel);

    wantAnyAllButton = new UtilityButton( "", Font("Small Text", 13, Font::plain) );
    wantAnyAllButton->setBounds(TTLCONDTRIG_INROW_XSIZE + TTLCONDTRIG_XGAP*2, TTLCONDTRIG_YSIZE*3 + TTLCONDTRIG_YGAP*4, TTLCONDTRIG_BOOLBUTTON_XSIZE, TTLCONDTRIG_BOOLBUTTON_YSIZE);
    wantAnyAllButton->addListener(this);
    addAndMakeVisible(wantAnyAllButton);

    setOutputLabel("undefined");
    setAnyAllState(true);
}


// Callback for button presses.
// This is the "want any" button or the "want all" button.
void TTLConditionalTriggerEditorInputPanel::buttonClicked(Button *theButton)
{
    parent->clickedAnyAll();
}


// Redraw hook.
void TTLConditionalTriggerEditorInputPanel::paint(Graphics& g)
{
    // Flat background underneath child components.
    g.fillAll(backgroundColour);
}


// Accessors.

void TTLConditionalTriggerEditorInputPanel::setOutputLabel(std::string newLabel)
{
    std::string scratchstring;

    scratchstring = "Inputs for ";
    scratchstring += newLabel;
    bannerLabel->setText(scratchstring, dontSendNotification);
}


void TTLConditionalTriggerEditorInputPanel::setInputLabel(int inIdx, std::string newLabel)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setInputLabel(newLabel);
}


void TTLConditionalTriggerEditorInputPanel::setRawLampState(int inIdx, bool wantLit)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setRawLampState(wantLit);
}


void TTLConditionalTriggerEditorInputPanel::setCookedLampState(int inIdx, bool wantLit)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setCookedLampState(wantLit);
}


void TTLConditionalTriggerEditorInputPanel::setAnyAllState(bool wantAll)
{
    textAnyAllLabel->setText( (wantAll ? "All" : "Any"), dontSendNotification );
}


void TTLConditionalTriggerEditorInputPanel::setFillColour(Colour newColour)
{
    backgroundColour = newColour;
}


void TTLConditionalTriggerEditorInputPanel::setRunningState(bool isRunning)
{
    for (int inIdx = 0; inIdx < TTLCONDTRIG_INPUTS; inIdx++)
        inputRows[inIdx]->setRunningState(isRunning);


    textWantLabel->setEnabled(!isRunning);
    textAnyAllLabel->setEnabled(!isRunning);
    wantAnyAllButton->setEnabled(!isRunning);
}


void TTLConditionalTriggerEditorInputPanel::setInputEnabled(int inIdx, bool isEnabled)
{
    if ((inIdx >= 0) && (inIdx < TTLCONDTRIG_INPUTS))
        inputRows[inIdx]->setInputEnabled(isEnabled);
}


//
// This is the end of the file.
