#ifndef GIMAGE_H
#define GIMAGE_H

#include "gobject.h"
#include "vtexturepart.h"

class TPackedRectangle;
class VTexture;
class TBitmap;

/**
 * Image objects which is defaults to invisible. ( Visible = false )
 * Becomes visible when set to image.
 */
class GImage: public GObject
{
public:
	GImage();

	VTexturePart TexturePart;
	VTexture* Texture;

	bool Stretch;

	/**
	 * Sets image and visibility.
	 */
	inline void SetImage(const GImage& other)
	{
		TexturePart = other.TexturePart;
		Texture = other.Texture;
		Visible = true;
		SetWidth(other.Width);
		SetHeight(other.Height);
	}

	inline void Render()
	{
		if (Stretch)
		{
			Render(ScreenRegion.X,ScreenRegion.Y,ScreenRegion.Width,ScreenRegion.Height);
		}
		else
		{
			Render(ScreenRegion.X,ScreenRegion.Y);
		}
	}

	/**
	 * Draws the image to given position on screen.
	 */
	void Render(int x, int y);

	/**
	 * Draws and stretches the image to given position and size on screen.
	 */
	void Render(int x, int y, int _width, int _height);
};


#endif