#ifndef RETRO_ICONS_H_DEFINED
#define RETRO_ICONS_H_DEFINED

namespace RetroIcons
{
	// Base classes.

	// Generic two-colour icon.
	class IconMonochrome : public Image
	{
	public:
		// Constructor.
		// Pixels with value ' ' are background; anything else is foreground.
		// NOTE - This has no bounds checking! Use with caution.
		IconMonochrome(int width, int height, const char *pixels, Colour bgcolour, Colour fgcolour);
		// Inhereted destructor is fine.
	};

	// Icon built with a colour lookup table.
	class IconPalette : public Image
	{
	public:
		// Constructor.
		// NOTE - This has no bounds checking! Use with caution.
		// Pixels with values found in the lookup table use LUT values. Anything else uses the default.
		IconPalette(int width, int height, const char *pixels, Colour defaultcolour, std::map<char,Colour> palette);
		// Inherited destructor is fine.
	};
}

// Specific icon instances.
#include "RetroIcons16.h"

#endif

// This is the end of the file.
