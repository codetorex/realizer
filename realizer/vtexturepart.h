#ifndef VTEXTUREPART_H
#define VTEXTUREPART_H

#include "mregion2.h"
#include "mvector2.h"
#include "cengine.h"

class VTexture;

class VTexturePart: public IRectangle
{
public:
	VTexturePart( const ISize& range, int x,int y, int _width,int _height)
	{
		Initialize(range,x,y,_width,_height);
	}

	VTexturePart( const ISize& range, const IRectangle& reg)
	{
		Initialize(range,reg.X,reg.Y,reg.Width,reg.Height);
	}

	VTexturePart()
	{

	}

	Vector2 TopLeftCoord;
	Vector2 BottomRightCoord;

	void Initialize(const ISize& size, int x,int y, int _width,int _height)
	{
		SetRectangle(x,y,_width,_height);
		TopLeftCoord = size.Ratio(x,y);
		BottomRightCoord = size.Ratio(x+_width,y+_height);
	}

	inline void Initialize(const ISize& range, const IRectangle& rect)
	{
		Initialize(range,rect.X,rect.Y,rect.Width,rect.Height);
	}

	inline void InitializeRelative(const ISize& range,const IPosition& relation, int x,int y,int _width, int _height)
	{
		Initialize(range,relation.X + x,relation.Y +y,_width,_height);
	}

	inline void InitializeOffset(const ISize& range, const IRectangle& source, int xOffset, int yOffset )
	{
		Initialize(range,source.X + xOffset,source.Y + yOffset,source.Width,source.Height);
	}

	inline void Initialize(const VTexturePart& othr)
	{
		TopLeftCoord = othr.TopLeftCoord;
		BottomRightCoord = othr.BottomRightCoord;
		SetRectangle(othr);
	}

	inline void Draw(float x,float y, const TColor32& color)
	{
		Engine.Draw.DrawQuad(x,y,x+Width,y+Height,TopLeftCoord.X,TopLeftCoord.Y,BottomRightCoord.X,BottomRightCoord.Y,color);
	}

	inline void Draw90DegRotated(float x,float y, const TColor32& color)
	{
		Engine.Draw.DrawQuad(x,y,x+Height,y+Width,BottomRightCoord.X,TopLeftCoord.Y,TopLeftCoord.X,BottomRightCoord.Y,color);
	}
	
	inline void Draw(float x,float y)
	{
		Draw(x,y,TColors::White);
	}

	inline void DrawScaled(float x,float y, float w, float h,  const TColor32& color)
	{
		Engine.Draw.DrawQuad(x,y,x+w,y+h,TopLeftCoord.X,TopLeftCoord.Y,BottomRightCoord.X,BottomRightCoord.Y,color);
	}

	inline void DrawScaled(float x,float y, float w, float h)
	{
		DrawScaled(x,y,w,h,TColors::White);
	}


	void DrawCropped(float x,float y, float w,float h,const TColor32& color)
	{
		throw NotImplementedException();
	}

	inline void DrawCropped(float x,float y, float w,float h)
	{
		DrawCropped(x,y,w,h,TColors::White);
	}


	void DrawTiled(float x,float y, float w,float h,const TColor32& color)
	{
		throw NotImplementedException();
	}

	inline void DrawTiled(float x,float y, float w,float h)
	{
		DrawTiled(x,y,w,h,TColors::White);
	}
};

/**
 * TODO: reimplement this with STATE pattern.
 */
class VTexturePartDefined: public VTexturePart
{
public:
	enum PartTypes
	{
		Scaled,
		Cropped,
		Tiled,
	};

	VTexturePartDefined()
	{
		SetPartType(Scaled);
	}


	VTexturePartDefined(const ISize& range, int x,int y, int w,int h): VTexturePart(range,x,y,w,h)
	{
		DrawFunction = 0;
	}

	VTexturePartDefined(const ISize& range, const IRectangle& reg): VTexturePart(range,reg)
	{
		DrawFunction = 0;
	}

	typedef void (VTexturePartDefined::*PartDrawFncPtr) (float x,float y,float w,float h, const TColor32& color);
	PartDrawFncPtr DrawFunction;

	void SetPartType(PartTypes partType)
	{
		switch(partType)
		{
		case Scaled:
			DrawFunction = &VTexturePartDefined::DrawScaled;
			break;

		case Cropped:
			DrawFunction = &VTexturePartDefined::DrawCropped;
			break;

		case Tiled:
			DrawFunction = &VTexturePartDefined::DrawTiled;
			break;
		}
	}

	inline void DrawPart(float x,float y,float w,float h,ui32 color)
	{
		(this->*DrawFunction)(x,y,w,h,color);
	}

	inline void DrawPart(float x,float y,float w,float h)
	{
		DrawPart(x,y,w,h,TColors::White);
	}
};

#endif