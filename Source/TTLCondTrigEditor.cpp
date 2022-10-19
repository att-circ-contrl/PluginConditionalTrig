#include "TTLCondTrigEditor.h"
#include "TTLCondTrig.h"

using namespace TTLConditionTrig;


// Private magic constants for GUI geometry.


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
// Settings button image.


// Private macros and constants for the settings button image.

#define WRENCH_WIDTH 16
#define WRENCH_HEIGHT 16

const char wrenchpicture[WRENCH_WIDTH*WRENCH_HEIGHT] =
{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ','#','#','#','#',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ','#','#','#','#',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ','#','#','#','#',' ',' ',' ','#',' ',
  ' ',' ',' ',' ',' ',' ',' ','#','#','#','#','#',' ','#','#',' ',
  ' ',' ',' ',' ',' ',' ','#','#','#','#','#','#','#','#','#',' ',
  ' ',' ',' ',' ',' ','#','#','#','#','#','#','#','#','#','#',' ',
  ' ',' ',' ',' ','#','#','#','#','#','#','#','#','#','#',' ',' ',
  ' ',' ',' ','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',
  ' ',' ','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',
  ' ','#','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ','#','#','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ','#','#','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


// Constructor.

WrenchImage::WrenchImage() : Image(Image::PixelFormat::ARGB, WRENCH_WIDTH, WRENCH_HEIGHT, true)
{
    char thischar;

    for (int hidx = 0; hidx < WRENCH_WIDTH; hidx++)
        for (int vidx = 0; vidx < WRENCH_HEIGHT; vidx++)
        {
            thischar = wrenchpicture[vidx * WRENCH_WIDTH + hidx];
            setPixelAt( hidx, vidx, (thischar == ' ' ? WRENCH_BACKGROUND : WRENCH_FOREGROUND) );
        }
}



//
// GUI tray for conditional trigger display and configuration.

// Constructor.
TTLConditionalTriggerEditor::TTLConditionalTriggerEditor(TTLConditionalTrigger* newParent) : GenericEditor(newParent, true)
{
    parent = newParent;

T_PRINT("Editor constructor called.");

    settingImage = new WrenchImage;

// FIXME - Testing.
dummyButton = new ImageButton("Foo");
dummyButton->setImages(true, true, true, *settingImage, 1.0, WRENCH_TRANSPARENT, *settingImage, 1.0, WRENCH_TRANSPARENT, *settingImage, 0.5, WRENCH_TRANSPARENT);
dummyButton->addListener(this);
addAndMakeVisible(dummyButton);
//dummyButton->setCentreRelative(0.5,0.5);
dummyButton->setBounds(30, 30, 60, 60);

// FIXME - NYI.

    // NOTE - The redraw timer should be running even if we're not acquiring data.
    startTimer(TTLCONDTRIG_DISPLAY_REFRESH_MS);

T_PRINT("Editor constructor finished.");
}


// Destructor.
TTLConditionalTriggerEditor::~TTLConditionalTriggerEditor()
{
    // "OwnedArray" and "ScopedPointer" take care of de-allocation for us.

    // Clean up the timer. Parent constructor probably does this but be safe.
    stopTimer();
}


// Timer callback.
void TTLConditionalTriggerEditor::timerCallback()
{
    // Pull data if not running.
    // If we're running, let process() push it to avoid race conditions.
    // Check the "acquisitionIsActive" state variable from GenericEditor.
    if (!acquisitionIsActive)
        parent->pushStateToDisplay();

    doStateUpdateRedraw();
}


// Accessor to push plugin state to the editor.
// FIXME - Placeholder. This needs to take arguments.
void TTLConditionalTriggerEditor::pushStateToEditor()
{
// FIXME - NYI.
}


// Redraw function. Should be called from the timer, not the plugin.
void TTLConditionalTriggerEditor::doStateUpdateRedraw()
{
// FIXME - NYI.
}


//
// This is the end of the file.
