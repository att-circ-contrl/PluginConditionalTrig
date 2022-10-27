#include <EditorHeaders.h>
#include "TTLCondTrigIcons.h"

using namespace TTLConditionTrig;


//
// Wrench image definition.

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


//
// "Connected" image definition.

#define CONN_WIDTH 16
#define CONN_HEIGHT 16

const char connectedpicture[CONN_WIDTH*CONN_HEIGHT] =
{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ','x','x','x','x','x','x',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ','x',' ','x','x','x','x',' ',' ',' ',' ',' ',
  'x','x','x','x','x','x',' ','x','x','x','x','x','x','x','x','x',

  'x','x','x','x','x','x',' ','x','x','x','x','x','x','x','x','x',
  ' ',' ',' ',' ',' ','x',' ','x','x','x','x',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ','x','x','x','x','x','x',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


//
// "Disconnected" image definition.

#define DISCONN_WIDTH 16
#define DISCONN_HEIGHT 16

const char disconnectedpicture[DISCONN_WIDTH*DISCONN_HEIGHT] =
{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ','x','x','x','x','x','x',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x',' ',' ',
  'x','x','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x','x','x',

  'x','x','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x','x','x',
  ' ',' ','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x',' ',' ',
  ' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ','x','x','x','x','x','x',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


//
// Indicator lamp image definition.

#define LAMP_WIDTH 16
#define LAMP_HEIGHT 16

const char lamppicture[LAMP_WIDTH*LAMP_HEIGHT] =
{ 'b','b','b','b','b','b',' ',' ',' ',' ','b','b','b','b','b','b',
  'b','b','b','b',' ',' ',' ',' ',' ',' ',' ',' ','b','b','b','b',
  'b','b',' ',' ',' ',' ','l','l','l','l',' ',' ',' ',' ','b','b',
  'b','b',' ',' ','l','l','l','l','l','l','l','l',' ',' ','b','b',

  'b',' ',' ','l','l','l','l','l','#','#','l','l','l',' ',' ','b',
  'b',' ',' ','l','l','l','l','l','l','#','#','l','l',' ',' ','b',
  ' ',' ','l','l','l','l','l','l','l','l','#','#','l','l',' ',' ',
  ' ',' ','l','l','l','l','l','l','l','l','l','#','l','l',' ',' ',

  ' ',' ','l','l','#','l','l','l','l','l','l','l','l','l',' ',' ',
  ' ',' ','l','l','#','#','l','l','l','l','l','l','l','l',' ',' ',
  'b',' ',' ','l','l','#','#','l','l','l','l','l','l',' ',' ','b',
  'b',' ',' ','l','l','l','#','#','l','l','l','l','l',' ',' ','b',

  'b','b',' ',' ','l','l','l','l','l','l','l','l',' ',' ','b','b',
  'b','b',' ',' ',' ',' ','l','l','l','l',' ',' ',' ',' ','b','b',
  'b','b','b','b',' ',' ',' ',' ',' ',' ',' ',' ','b','b','b','b',
  'b','b','b','b','b','b',' ',' ',' ',' ','b','b','b','b','b','b' };


//
// Constructors for base classes.

// Generic two-colour icon.
// Pixels with value ' ' are background; anything else is foreground.
// NOTE - This has no bounds checking! Use with caution.

IconMonochrome::IconMonochrome(int width, int height, const char *pixels, Colour bgcolour, Colour fgcolour) : Image(Image::PixelFormat::ARGB, width, height, true)
{
    char thischar;

    for (int hidx = 0; hidx < width; hidx++)
        for (int vidx = 0; vidx < height; vidx++)
        {
            thischar = pixels[vidx * width + hidx];
            setPixelAt( hidx, vidx, (thischar == ' ' ? bgcolour : fgcolour) );
        }
}


// Icon built with a colour lookup table.
// Pixels with values found in the lookup table use LUT values. Anything else uses the default.
// NOTE - This has no bounds checking! Use with caution.
// NOTE - Passing the map by value lets us build it on the fly in-line using C++11 initialization syntax, if desired.

IconPalette::IconPalette(int width, int height, const char *pixels, Colour defaultcolour, std::map<char,Colour> palette) : Image(Image::PixelFormat::ARGB, width, height, true)
{
    char thischar;

    for (int hidx = 0; hidx < width; hidx++)
        for (int vidx = 0; vidx < height; vidx++)
        {
            thischar = pixels[vidx * width + hidx];
            if ( palette.count(thischar) > 0 )
                setPixelAt( hidx, vidx, palette[thischar] );
            else
                setPixelAt( hidx, vidx, defaultcolour );
        }
}


//
// Constructors for specific monochrome images.

// NOTE - If image dimensions don't match image pixel buffer size, bad things will happen. Use caution.

WrenchImage::WrenchImage(Colour bgcolour, Colour fgcolour) : IconMonochrome(WRENCH_WIDTH, WRENCH_HEIGHT, wrenchpicture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


ConnectedImage::ConnectedImage(Colour bgcolour, Colour fgcolour) : IconMonochrome(CONN_WIDTH, CONN_HEIGHT, connectedpicture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


DisconnectedImage::DisconnectedImage(Colour bgcolour, Colour fgcolour) : IconMonochrome(DISCONN_WIDTH, DISCONN_HEIGHT, disconnectedpicture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


//
// Constructors for specific palette-based images.

// NOTE - If image dimensions don't match image pixel buffer size, bad things will happen. Use caution.

// NOTE - We're building maps on the fly using C++11 initializers. This is compact but might not be supported by all compilers.

IndicatorLampImage::IndicatorLampImage(Colour bgcolour, Colour outlinecolour, Colour lampcolour, Colour highlightcolour)
    : IconPalette( LAMP_WIDTH, LAMP_HEIGHT, lamppicture, bgcolour,
        {{' ',outlinecolour},{'l',lampcolour},{'#',highlightcolour}} )
{
    // Nothing else to do.
}


//
// This is the end of the file.
