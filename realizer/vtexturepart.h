#ifndef VTEXTUREPART_H
#define VTEXTUREPART_H

#include "tregion.h"
#include "mvector2.h"
#include "cengine.h"

class VTexture;

class VTexturePart: public TRectangle
{
public:
	VTexturePart( TRange* range, int x,int y, int _width,int _height)
	{
		Initialize(range,x,y,_width,_height);
	}

	VTexturePart( TRange* range, TRectangle* reg)
	{
		Initialize(range,reg->X,reg->Y,reg->Width,reg->Height);
	}

	VTexturePart()
	{

	}

	vec2 TopLeftCoord;
	vec2 BottomRightCoord;

	void Initialize(TRange* range, int x,int y, int _width,int _height)
	{
		SetRectangle(x,y,_width,_height);
		TopLeftCoord = range->GetRatio((float)x,(float)y);
		BottomRightCoord = range->GetRatio((float)x+_width,(float)y+_height);
	}

	inline void InitializeRelative(TRange* range, TPosition* relation, int x,int y,int _width, int _height)
	{
		Initialize(range,relation->X + x,relation->Y +y,_width,_height);
	}

	inline void InitializeOffset(TRange* range, TRectangle* source, int xOffset, int yOffset )
	{
		Initialize(range,source->X + xOffset,source->Y + yOffset,source->Width,source->Height);
	}

	inline void Initialize(VTexturePart* othr)
	{
		TopLeftCoord = othr->TopLeftCoord;
		BottomRightCoord = othr->BottomRightCoord;
		SetRectangleFrom(othr);
	}

	inline void Draw(float x,float y,dword color)
	{
		Engine.Draw.Add2DQuadColor1Tex(x,y,x+Width,y+Height,TopLeftCoord.x,TopLeftCoord.y,BottomRightCoord.x,BottomRightCoord.y,color);
	}
	
	inline void Draw(float x,float y)
	{
		Draw(x,y,Engine.Draw.DefaultDiffuse);
	}

	inline void DrawScaled(float x,float y, float w, float h, dword color)
	{
		Engine.Draw.Add2DQuadColor1Tex(x,y,x+w,y+h,TopLeftCoord.x,TopLeftCoord.y,BottomRightCoord.x,BottomRightCoord.y,color);
	}

	inline void DrawScaled(float x,float y, float w, float h)
	{
		DrawScaled(x,y,w,h,Engine.Draw.DefaultDiffuse);
	}


	void DrawCropped(float x,float y, float w,float h,dword color)
	{

	}

	inline void DrawCropped(float x,float y, float w,float h)
	{
		DrawCropped(x,y,w,h,Engine.Draw.DefaultDiffuse);
	}


	void DrawTiled(float x,float y, float w,float h,dword color)
	{

	}

	inline void DrawTiled(float x,float y, float w,float h)
	{
		DrawTiled(x,y,w,h,Engine.Draw.DefaultDiffuse);
	}
};

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


	VTexturePartDefined(TRange* range, int x,int y, int w,int h): VTexturePart(range,x,y,w,h)
	{
		DrawFunction = 0;
	}

	VTexturePartDefined(TRange* range, TRegion* reg): VTexturePart(range,reg)
	{
		DrawFunction = 0;
	}

	typedef void (VTexturePartDefined::*PartDrawFncPtr) (float x,float y,float w,float h,dword color);
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

	inline void DrawPart(float x,float y,float w,float h,dword color)
	{
		(this->*DrawFunction)(x,y,w,h,color);
	}

	inline void DrawPart(float x,float y,float w,float h)
	{
		DrawPart(x,y,w,h,Engine.Draw.DefaultDiffuse);
	}
};

#endif