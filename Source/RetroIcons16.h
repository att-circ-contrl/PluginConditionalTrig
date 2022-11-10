#ifndef RETRO_ICONS_16_H_DEFINED
#define RETRO_ICONS_16_H_DEFINED

namespace RetroIcons
{
	// Monochrome icons.

	// Settings button image (wrench).
	class Wrench16Image : public IconMonochrome
	{
	public:
		Wrench16Image(Colour bgcolour, Colour fgcolour);
	};

	// Connected image (cable).
	class Connected16Image : public IconMonochrome
	{
	public:
		Connected16Image(Colour bgcolour, Colour fgcolour);
	};

	// Disconnected image (cable).
	class Disconnected16Image : public IconMonochrome
	{
	public:
		Disconnected16Image(Colour bgcolour, Colour fgcolour);
	};


	// Palette-based icons.

	// Indicator lamp image.
	class IndicatorLamp16Image : public IconPalette
	{
	public:
		IndicatorLamp16Image(Colour bgcolour, Colour outlinecolour, Colour lampcolour, Colour highlightcolour);
	};
}

#endif

// This is the end of the file.
