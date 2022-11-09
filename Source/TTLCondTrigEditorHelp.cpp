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
    doneButton->setBounds(0, TTLCONDTRIG_CONFIGPANEL_YSIZE - TTLCONDTRIG_YSIZE, TTLCONDTRIG_HELPDONE_XSIZE, TTLCONDTRIG_YSIZE);
    doneButton->addListener(this);
    addAndMakeVisible(doneButton);
}


// Redraw hook.
void TTLConditionalTriggerEditorConfigHelp::paint(Graphics& g)
{
    g.fillAll(juce::Colours::white);

    // Cursors behind waveforms.
    g.setColour(juce::Colours::red);
    drawGridHVLine(g, 6, 16, 6, 21, 1);
    drawGridHVLine(g, 14, 16, 14, 21, 1);

    // Waveforms.

    g.setColour(juce::Colours::black);

    drawGridHVLine(g, 2, 10, 3, 10, 2);
    drawGridHVLine(g, 3, 10, 3, 6, 2);
    drawGridHVLine(g, 3, 6, 4, 6, 2);
    drawGridHVLine(g, 4, 6, 4, 10, 2);
    drawGridHVLine(g, 4, 10, 6, 10, 2);
    drawGridHVLine(g, 6, 10, 6, 6, 2);
    drawGridHVLine(g, 6, 6, 9, 6, 2);
    drawGridHVLine(g, 9, 6, 9, 10, 2);
    drawGridHVLine(g, 9, 10, 11, 10, 2);
    drawGridHVLine(g, 11, 10, 11, 6, 2);
    drawGridHVLine(g, 11, 6, 13, 6, 2);
    drawGridHVLine(g, 13, 6, 13, 10, 2);
    drawGridHVLine(g, 13, 10, 14, 10, 2);
    drawGridHVLine(g, 14, 10, 14, 6, 2);
    drawGridHVLine(g, 14, 6, 17, 6, 2);
    drawGridHVLine(g, 17, 6, 17, 10, 2);
    drawGridHVLine(g, 17, 10, 18, 10, 2);

    drawGridHVLine(g, 2, 20, 8, 20, 2);
    drawGridHVLine(g, 8, 20, 8, 16, 2);
    drawGridHVLine(g, 8, 16, 10, 16, 2);
    drawGridHVLine(g, 10, 16, 10, 20, 2);
    drawGridHVLine(g, 10, 20, 16, 20, 2);
    drawGridHVLine(g, 16, 20, 16, 16, 2);
    drawGridHVLine(g, 16, 16, 18, 16, 2);

    // Cursor arrows.
    g.setColour(juce::Colours::blue);
    drawGridVArrow(g, 6, 11, 15);
    drawGridVArrow(g, 14, 11, 15);

    // Dimension bars.
    g.setColour(juce::Colours::blue);
    drawGridHDim(g, 3, 5, 4);
    drawGridHDim(g, 6, 14, 4);
    drawGridHDim(g, 6, 8, 23);
    drawGridHDim(g, 8, 10, 23);
    drawGridHDim(g, 14, 16, 23);

    // Labels.

    g.setFont(Font("Small Text", 13, Font::plain));

    g.setColour(juce::Colours::black);
    drawGridText(g, 0, 6, 2, 10, "Input");
    drawGridText(g, 0, 16, 2, 20, "Output");

    g.setColour(juce::Colours::darkcyan);
    drawGridText(g, 0, 1, 3, 5, "t_stable");
    drawGridText(g, 3, 21, 6, 25, "t_delay");
    drawGridText(g, 10, 21, 13, 25, "t_sustain");
    drawGridText(g, 14, 1, 17, 5, "t_dead");
}


// Button click handler.
void TTLConditionalTriggerEditorConfigHelp::buttonClicked(Button* theButton)
{
    // This was the "close" button.
    parent->clickedHelpScreenDone();
}


// Utility function for forcing ascending order in integer pairs.
void TTLConditionalTriggerEditorConfigHelp::forceAscending(int &num1, int &num2)
{
    int scratch;

    if (num1 > num2)
    {
        scratch = num1;
        num1 = num2;
        num2 = scratch;
    }
}


// Graphics primitives.

// Horizontal or vertical line.
// This actually draws a rectangle, so diagonal lines will give undesired results.
void TTLConditionalTriggerEditorConfigHelp::drawGridHVLine(Graphics& g, int x1, int y1, int x2, int y2, int thickness)
{
    // Force order and sanity.

    forceAscending(x1, x2);
    forceAscending(y1, y2);

    if (thickness < 1)
        thickness = 1;

    // Convert grid coordinates to pixels and render the rectangle.

    x1 *= TTLCONDTRIG_HELP_XGRID;
    y1 *= TTLCONDTRIG_HELP_YGRID;
    x2 *= TTLCONDTRIG_HELP_XGRID;
    y2 *= TTLCONDTRIG_HELP_YGRID;

    x2 += thickness - 1;
    y2 += thickness - 1;

    g.fillRect(x1, y1, (1 + x2 - x1), (1 + y2 - y1));
}


// Vertical arrow.
void TTLConditionalTriggerEditorConfigHelp::drawGridVArrow(Graphics& g, int x, int y1, int y2)
{
    drawGridHVLine(g, x, y1, x, y2, 1);
    drawGridArrowhead(g, x, y2, false, (y2 < y2));
}


// Horizontal dimension.
void TTLConditionalTriggerEditorConfigHelp::drawGridHDim(Graphics& g, int x1, int x2, int y)
{
    drawGridHVLine(g, x1, y, x2, y, 1);
    drawGridArrowhead(g, x1, y, true, (x1 < x2));
    drawGridArrowhead(g, x2, y, true, (x2 < x1));
    drawGridHVLine(g, x1, y-1, x1, y+1, 1);
    drawGridHVLine(g, x2, y-1, x2, y+1, 1);
}


// Arrowhead with hard-coded geometry.
void TTLConditionalTriggerEditorConfigHelp::drawGridArrowhead(Graphics& g, int x, int y, bool isHorizontal, bool pointsNegative)
{
    x *= TTLCONDTRIG_HELP_XGRID;
    y *= TTLCONDTRIG_HELP_YGRID;
    int delta = TTLCONDTRIG_HELP_YGRID;

    if (isHorizontal)
    {
        // Horizontal arrows are long.
        if (pointsNegative)
        {
            g.drawLine(x, y, x+2*delta, y-delta);
            g.drawLine(x, y, x+2*delta, y+delta);
            g.drawLine(x+2*delta, y-delta, x+2*delta, y+delta);
        }
        else
        {
            g.drawLine(x, y, x-2*delta, y-delta);
            g.drawLine(x, y, x-2*delta, y+delta);
            g.drawLine(x-2*delta, y-delta, x-2*delta, y+delta);
        }
    }
    else
    {
        // Vertical arrows are short.
        if (pointsNegative)
        {
            g.drawLine(x, y, x-delta, y+delta);
            g.drawLine(x, y, x+delta, y+delta);
            g.drawLine(x-delta, y+delta, x+delta, y+delta);
        }
        else
        {
            g.drawLine(x, y, x-delta, y-delta);
            g.drawLine(x, y, x+delta, y-delta);
            g.drawLine(x-delta, y-delta, x+delta, y-delta);
        }
    }
}


// Label that fits in a rectangle.
void TTLConditionalTriggerEditorConfigHelp::drawGridText(Graphics& g, int x1, int y1, int x2, int y2, std::string theText)
{
    // Force order.
    forceAscending(x1, x2);
    forceAscending(y1, y2);

    // Convert to pixel coordinates.
    x1 *= TTLCONDTRIG_HELP_XGRID;
    x2 *= TTLCONDTRIG_HELP_XGRID;
    y1 *= TTLCONDTRIG_HELP_YGRID;
    y2 *= TTLCONDTRIG_HELP_YGRID;

    // Render bounded text.
    g.drawFittedText(theText, x1, y1, (1 + x2 - x1), (1 + y2 - y1), Justification::centredLeft, 1);
}


//
// This is the end of the file.
