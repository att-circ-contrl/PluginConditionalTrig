#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigIcons.h"
#include "TTLCondTrigConstants.h"

#define TRIGDEBUGPREFIX "[CondTrigOutput]  "
#include "TTLCondTrigDebug.h"

using namespace TTLConditionTrig;


//
//
// GUI row for status and config buttons for one output.


// Constructor.
TTLConditionalTriggerEditorOutputRow::TTLConditionalTriggerEditorOutputRow(TTLConditionalTriggerEditor* newParent, int newOutIdx)
{
    parent = newParent;
    outIdx = newOutIdx;

    // Enable button.
    connectOnImage = new Connected16Image(CONN_BACKGROUND, CONN_FOREGROUND);
    connectOffImage = new Disconnected16Image(DISCONN_BACKGROUND, DISCONN_FOREGROUND);
    enableButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    enableButton->setImages(true, true, true, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOffImage, 1.0, COLOUR_TRANSPARENT, *connectOnImage, 0.5, COLOUR_TRANSPARENT);
    enableButton->setBounds(TTLCONDTRIG_XGAP, 0, TTLCONDTRIG_CONN_SIZE, TTLCONDTRIG_CONN_SIZE);
    enableButton->setClickingTogglesState(true);
    enableButton->addListener(this);
    addAndMakeVisible(enableButton);

    // Output name label.
    outputNameLabel = new Label("Output Name", "undefined");
    outputNameLabel->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_XGAP*2, 0, TTLCONDTRIG_LABEL_XSIZE, TTLCONDTRIG_YSIZE);
    addAndMakeVisible(outputNameLabel);
    // Disabling this greys it out. Instead let it get clicks and ignore them.
//    outputNameLabel->setEnabled(false);

    // Settings button.
    settingsImage = new Wrench16Image(WRENCH_BACKGROUND, WRENCH_FOREGROUND);
    settingsButton = new ImageButton;
    // Images are normal, hover-over, and pressed.
    // Tuples are image, image opacity, and overlay colour.
    settingsButton->setImages(true, true, true, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 1.0, COLOUR_TRANSPARENT, *settingsImage, 0.5, COLOUR_TRANSPARENT);
    settingsButton->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_XGAP*3, 0, TTLCONDTRIG_WRENCH_SIZE, TTLCONDTRIG_WRENCH_SIZE);
    settingsButton->addListener(this);
    addAndMakeVisible(settingsButton);

    // Indicator lamp icon.
    // It's less expensive to have two images and make only one visible than it is to change the image on one component.

    lampOnImage = new IndicatorLamp16Image(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_GREEN_FILL, LAMP_GREEN_HIGHLIGHT);
    lampOnComponent = new ImageComponent();
    lampOnComponent->setImage(*lampOnImage);

    lampOnComponent->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*4, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOnComponent);
    lampOnComponent->setEnabled(false);
    lampOnComponent->setVisible(false);

    lampOffImage = new IndicatorLamp16Image(LAMP_BACKGROUND, LAMP_OUTLINE, LAMP_OFF_FILL, LAMP_OFF_HIGHLIGHT);
    lampOffComponent = new ImageComponent();
    lampOffComponent->setImage(*lampOffImage);

    lampOffComponent->setBounds(TTLCONDTRIG_CONN_SIZE + TTLCONDTRIG_LABEL_XSIZE + TTLCONDTRIG_WRENCH_SIZE + TTLCONDTRIG_XGAP*4, 0, TTLCONDTRIG_LAMP_SIZE, TTLCONDTRIG_LAMP_SIZE);
    addAndMakeVisible(lampOffComponent);
    lampOffComponent->setEnabled(false);

    // Tab button.
    tabButton = new ColorButton("", Font("Small Text", 13, Font::plain));
    tabButton->setColors(juce::Colours::black, COLOUR_BOGUS);
    tabButton->setBounds(TTLCONDTRIG_OUTROW_XSIZE - TTLCONDTRIG_TAB_XSIZE, 0, TTLCONDTRIG_TAB_XSIZE, TTLCONDTRIG_YSIZE);
    tabButton->addListener(this);
    addAndMakeVisible(tabButton);

    // Force sane state.
    setTabColour(COLOUR_BOGUS);
    setOutputEnabled(false);
    setRunningState(false);
}


// Callback for button presses.
// This is the settings button, the enable button, or the output selection tab.
void TTLConditionalTriggerEditorOutputRow::buttonClicked(Button *theButton)
{
    if (theButton == tabButton)
        parent->clickedOutputTab(outIdx);
    else if (theButton == settingsButton)
        parent->clickedOutputSettings(outIdx);
    else
        parent->clickedOutputEnableToggle(outIdx);
}


// Redraw hook.
void TTLConditionalTriggerEditorOutputRow::paint(Graphics& g)
{
    // Flat background underneath child components.
    g.fillAll(currentBackgroundColour);
}


// Accessors.

void TTLConditionalTriggerEditorOutputRow::setOutputLabel(std::string newLabel)
{
    outputNameLabel->setText(newLabel, dontSendNotification);
}


void TTLConditionalTriggerEditorOutputRow::setLampState(bool wantLit)
{
    lampOnComponent->setVisible(wantLit);
    lampOffComponent->setVisible(!wantLit);
}


void TTLConditionalTriggerEditorOutputRow::setTabColour(Colour newColour)
{
    // The colour we're passed is the saturated version used for the tab itself.
    // Background and disabled colours are derived from this.

    // Get 50% and 75% versions of the original colour.
    // These are scaled (darkened); to get saturated, add an offset too.

    uint32 scratch = newColour.getARGB();
    uint32 dark50 = (scratch & 0x00fefefe) >> 1;
    uint32 dark75 = (scratch & 0x00ffffff) - ((scratch & 0x00fcfcfc) >> 2);


    // Get various derived colours for readability.
    // Unused ones will be optimized out.
    // There's no "set from uint32" accessor, so we have to create new instances.

    Colour colourSaturated = newColour;

    Colour colour75Light(dark75 + 0xff404040);
    Colour colour75Grey(dark75 + 0xff202020);
    Colour colour75Dark(dark75 + 0xff000000);

    Colour colour50Light(dark50 + 0xff404040);
    Colour colour50Grey(dark50 + 0xff202020);
    Colour colour50Dark(dark50 + 0xff000000);

    // Assign decent-looking colours.
    // NOTE - It turns out that changing colour between enabled/disabled doesn't help.
    tabColourLight = colour75Dark;
    tabColourDark = tabColourLight;
    backColourLight = colourSaturated;
    backColourDark = backColourLight;

    // This gets overwritten when setOutputEnabled() is called.
    currentBackgroundColour = COLOUR_BOGUS;

    // Call setOutputEnabled() to propagate colour values to buttons.
    // The real enable state will get set as soon as the timer callback starts.
    setOutputEnabled(false);
}


Colour TTLConditionalTriggerEditorOutputRow::getBackgroundColour()
{
    // This returns the "active selection" version, irrespective of whether or not we are selected.
    return backColourLight;
}


void TTLConditionalTriggerEditorOutputRow::setRunningState(bool isRunning)
{
    // Lock out the settings button if we're running.
    // We can still switch tabs and enable/disable this output.
    settingsButton->setEnabled(!isRunning);
}


void TTLConditionalTriggerEditorOutputRow::setOutputEnabled(bool isEnabled)
{
    // If we're not using this output, disable the text (to grey it out).
    outputNameLabel->setEnabled(isEnabled);

    // Force the control button to the correct state.
    enableButton->setToggleState(isEnabled, dontSendNotification);

    // Set colours.
    currentBackgroundColour = (isEnabled ? backColourLight : backColourDark);
    tabButton->setColors(juce::Colours::black, (isEnabled ? tabColourLight : tabColourDark));

    // Force a redraw, since otherwise only some of the elements update.
    repaint();
}


//
//
// GUI panel for configuring all outputs.


// Constructor.
TTLConditionalTriggerEditorOutputPanel::TTLConditionalTriggerEditorOutputPanel(TTLConditionalTriggerEditor* newParent)
{
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
    {
        outputRows[outIdx] = new TTLConditionalTriggerEditorOutputRow(newParent, outIdx);
        outputRows[outIdx]->setBounds(0, (TTLCONDTRIG_YSIZE + TTLCONDTRIG_YGAP) * (outIdx+1) + TTLCONDTRIG_YGAP, TTLCONDTRIG_OUTROW_XSIZE, TTLCONDTRIG_YSIZE);
        addAndMakeVisible(outputRows[outIdx]);
    }
}


// Redraw hook.
void TTLConditionalTriggerEditorOutputPanel::paint(Graphics& g)
{
    // Flat background underneath child components.
    g.fillAll(OUTPUT_BACKGROUND);
}


// Accessors.

void TTLConditionalTriggerEditorOutputPanel::setOutputLabel(int outIdx, std::string newLabel)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setOutputLabel(newLabel);
}


void TTLConditionalTriggerEditorOutputPanel::setLampState(int outIdx, bool wantLit)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setLampState(wantLit);
}


void TTLConditionalTriggerEditorOutputPanel::setTabColour(int outIdx, Colour newColour)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setTabColour(newColour);
}


Colour TTLConditionalTriggerEditorOutputPanel::getBackgroundColour(int outIdx)
{
    Colour retVal = COLOUR_BOGUS;

    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        retVal = outputRows[outIdx]->getBackgroundColour();

    return retVal;
}


void TTLConditionalTriggerEditorOutputPanel::setRunningState(bool isRunning)
{
    for (int outIdx = 0; outIdx < TTLCONDTRIG_OUTPUTS; outIdx++)
        outputRows[outIdx]->setRunningState(isRunning);
}


void TTLConditionalTriggerEditorOutputPanel::setOutputEnabled(int outIdx, bool isEnabled)
{
    if ((outIdx >= 0) && (outIdx < TTLCONDTRIG_OUTPUTS))
        outputRows[outIdx]->setOutputEnabled(isEnabled);
}


//
// This is the end of the file.
