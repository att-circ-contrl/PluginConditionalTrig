#include "TTLCondTrig.h"
#include "TTLCondTrigEditor.h"
#include "TTLCondTrigConstants.h"

using namespace TTLConditionTrig;


//
//
// Help screen for event processing timing.


// Constructor.
TTLConditionalTriggerEditorConfigHelp::TTLConditionalTriggerEditorConfigHelp(TTLConditionalTriggerEditorConfigPanel *newParent)
{
    parent = newParent;

    doneButton = new UtilityButton("Close", Font("Small Text", 13, Font::plain));
    doneButton->setBounds(0, 0, 40, TTLCONDTRIG_YSIZE);
    doneButton->addListener(this);
    addAndMakeVisible(doneButton);
}


// Redraw hook.
void TTLConditionalTriggerEditorConfigHelp::paint(Graphics& g)
{
    g.fillAll(juce::Colours::white);
}


// Button click handler.
void TTLConditionalTriggerEditorConfigHelp::buttonClicked(Button* theButton)
{
    // This was the "close" button.
    parent->clickedHelpScreenDone();
}


//
// This is the end of the file.
