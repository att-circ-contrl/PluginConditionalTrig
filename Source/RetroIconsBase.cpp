#include <EditorHeaders.h>
#include "RetroIcons.h"

using namespace RetroIcons;


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
// This is the end of the file.
