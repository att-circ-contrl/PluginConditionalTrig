#include <EditorHeaders.h>
#include "RetroIcons.h"

using namespace RetroIcons;


//
// Wrench image definition.

#define WRENCH16_WIDTH 16
#define WRENCH16_HEIGHT 16

const char wrench16picture[WRENCH16_WIDTH*WRENCH16_HEIGHT] =
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

#define CONN16_WIDTH 16
#define CONN16_HEIGHT 16

const char connected16picture[CONN16_WIDTH*CONN16_HEIGHT] =
{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ',' ','x','x','x','x',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ','x',' ','x','x',' ',' ',' ',' ',' ',' ',
  'x','x','x','x','x','x','x',' ','x','x','x','x','x','x','x','x',

  'x','x','x','x','x','x','x',' ','x','x','x','x','x','x','x',' ',
  ' ',' ',' ',' ',' ',' ','x',' ','x','x',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ','x','x','x','x',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


//
// "Disconnected" image definition.

#define DISCONN16_WIDTH 16
#define DISCONN16_HEIGHT 16

const char disconnected16picture[DISCONN16_WIDTH*DISCONN16_HEIGHT] =
{ ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ','x','x','x','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ','x','x',' ',' ',' ',
  'x','x','x','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x','x',

  'x','x','x','x',' ',' ',' ',' ',' ',' ',' ','x','x','x','x','x',
  ' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ','x','x',' ',' ',' ',
  ' ',' ',' ','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ','x','x','x','x',' ',' ',' ',' ',' ',' ',' ',' ',' ',

  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
  ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


//
// Indicator lamp image definition.

#define LAMP16_WIDTH 16
#define LAMP16_HEIGHT 16

const char lamp16picture[LAMP16_WIDTH*LAMP16_HEIGHT] =
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
// Constructors for specific monochrome images.

// NOTE - If image dimensions don't match image pixel buffer size, bad things will happen. Use caution.

Wrench16Image::Wrench16Image(Colour bgcolour, Colour fgcolour) : IconMonochrome(WRENCH16_WIDTH, WRENCH16_HEIGHT, wrench16picture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


Connected16Image::Connected16Image(Colour bgcolour, Colour fgcolour) : IconMonochrome(CONN16_WIDTH, CONN16_HEIGHT, connected16picture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


Disconnected16Image::Disconnected16Image(Colour bgcolour, Colour fgcolour) : IconMonochrome(DISCONN16_WIDTH, DISCONN16_HEIGHT, disconnected16picture, bgcolour, fgcolour)
{
    // Nothing else to do.
}


//
// Constructors for specific palette-based images.

// NOTE - If image dimensions don't match image pixel buffer size, bad things will happen. Use caution.

// NOTE - We're building maps on the fly using C++11 initializers. This is compact but might not be supported by all compilers.

IndicatorLamp16Image::IndicatorLamp16Image(Colour bgcolour, Colour outlinecolour, Colour lampcolour, Colour highlightcolour)
    : IconPalette( LAMP16_WIDTH, LAMP16_HEIGHT, lamp16picture, bgcolour,
        {{' ',outlinecolour},{'l',lampcolour},{'#',highlightcolour}} )
{
    // Nothing else to do.
}


//
// This is the end of the file.
