#ifndef GQUAD_H
#define GQUAD_H

#include "vtexturepart.h"

/**
* Unoptimized scalable quad that formed by parts (524 bytes) (its now 452 bytes)
*/
class GScalableQuadParted
{
public:
	GScalableQuadParted()
	{
		CenterColor = TColors::White;
	}

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

	ui32 CenterColor;

	void Render(GObject* obj);

	inline int GetHeightSpace(int totalHeight)
	{
		return totalHeight - (Top.Height + Bottom.Height);
	}

	inline int GetWidthSpace(int totalWidth)
	{
		return totalWidth - (Left.Width + Right.Width);
	}

	/**
	 * Adjusts empty space of the object accordingly margins used by skin.
	 */
	void SetObjectContent(GObject* obj);
};

/**
* Optimized scalable quad (48 bytes)
* It created by equal border size definitions.
*/
class GScalableQuad: public IPadding
{
public:
	float TextureCoords[8];

	/**
	* Calculates texture coordinates
	* BorderSizes should be set before calling this.
	* @param tx texture pointer to be referenced
	* @param rect texture rect that this quad going to be use
	*/

	void Initialize(const ISize& textureRange, const IRectangle& rect);

	void Initialize(const GScalableQuad& other);

	void Swap(GScalableQuad& other);

	/**
	* Rotates texture coordinates, so resulting image will be look like rotated.
	*/
	void Rotate(int way);

	void Render(GObject* obj);

	void Render( const IRectangle& rect );

	void RenderLeftOnly( const IRectangle& rect);

	inline int GetHeightSpace(int totalHeight)
	{
		return totalHeight - (Top + Bottom);
	}

	inline int GetWidthSpace(int totalWidth)
	{
		return totalWidth - (Left + Right);
	}

	/**
	 * Adjusts empty space of the object accordingly margins used by skin.
	 */
	void SetObjectRegion(GObject* obj);

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

	void Initialize(VTexture* tx, const IRegion& rect);

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

	void Initialize(VTexture* tx, const IRegion& rect);

	void Render(GObject* obj);
};



#endif