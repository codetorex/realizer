#ifndef GQUAD_H
#define GQUAD_H

#include "vtexturepart.h"

/**
* Unoptimized scalable quad that formed by parts (524 bytes)
*/
class GScalableQuadParted
{
public:
	// Static Corners
	VTexturePart TopLeft;
	VTexturePart TopRight;
	VTexturePart BottomLeft;
	VTexturePart BottomRight;

	// Sizing Parts
	VTexturePartDefined Left;
	VTexturePartDefined Top;
	VTexturePartDefined Right;
	VTexturePartDefined Bottom;
	VTexturePartDefined Center;

	void Render(GObject* obj);
};

/**
* Optimized scalable quad (48 bytes)
* It created by equal border size definitions.
*/
class GScalableQuad
{
public:

	float TextureCoords[8];

	int LeftMargin;
	int RightMargin;
	int TopMargin;
	int BottomMargin;

	GScalableQuad()
	{

	}

	/**
	* Calculates texture coordinates
	* BorderSizes should be set before calling this.
	* @param tx texture pointer to be referenced
	* @param rect texture rect that this quad going to be use
	*/
	void Initialize(VTexture* tx, const TRegion& rect);

	/**
	* Rotates texture coordinates, so resulting image will be look like rotated.
	*/
	void Rotate(int way);

	void Render(GObject* obj);
};

/**
* Horizontal optimized scalable quad.
* Center part scales while left and right part static.
*/
class GHorizontalQuad
{
public:
	float TextureCoords[6];
	int LeftMargin;
	int RightMargin;

	void Initialize(VTexture* tx, const TRegion& rect);

	void Render(GObject* obj);
};

/**
* Vertical optimized scalable quad.
* Center part scales while top and bottom part static.
*/
class GVerticalQuad
{
public:
	float TextureCoords[6];
	int TopMargin;
	int BottomMargin;

	void Initialize(VTexture* tx, const TRegion& rect);

	void Render(GObject* obj);
};



#endif