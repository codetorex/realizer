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
 * TODO: split this to two classes, ONE is only defines image, other is a GObject that can redner it
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
		Width = other.Width;
		Height = other.Height;
	}

	inline void Render()
	{
		const IRectangle& dRect = DrawRegion;
		if (Stretch)
		{
			Render(dRect.X,dRect.Y,dRect.Width,dRect.Height);
		}
		else
		{
			Render(dRect.X,dRect.Y);
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