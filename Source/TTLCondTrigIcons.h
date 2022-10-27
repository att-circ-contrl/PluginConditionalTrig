#ifndef TTLCONDTRIG_ICONS_H_DEFINED
#define TTLCONDTRIG_ICONS_H_DEFINED

namespace TTLConditionTrig
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


	// Monochrome icons.

	// Settings button image (wrench).
	class WrenchImage : public IconMonochrome
	{
	public:
		WrenchImage(Colour bgcolour, Colour fgcolour);
	};

	// Connected image (cable).
	class ConnectedImage : public IconMonochrome
	{
	public:
		ConnectedImage(Colour bgcolour, Colour fgcolour);
	};

	// Disconnected image (cable).
	class DisconnectedImage : public IconMonochrome
	{
	public:
		DisconnectedImage(Colour bgcolour, Colour fgcolour);
	};


	// Palette-based icons.

	// Indicator lamp image.
	class IndicatorLampImage : public IconPalette
	{
	public:
		IndicatorLampImage(Colour bgcolour, Colour outlinecolour, Colour lampcolour, Colour highlightcolour);
	};
}

#endif

// This is the end of the file.
