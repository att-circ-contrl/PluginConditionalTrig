#include <EditorHeaders.h>
#include "TTLCondTrigIcons.h"

using namespace TTLConditionTrig;


//
// Settings button image.

// Private macros and constants for the settings button image.

#define WRENCH_FOREGROUND juce::Colours::black
#define WRENCH_BACKGROUND juce::Colours::white

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
// This is the end of the file.
