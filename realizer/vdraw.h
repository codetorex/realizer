#ifndef VDRAW_H
#define VDRAW_H

#include "vvertexbuffer.h"
#include "vvertexstream.h"
#include "mregion2.h"

class VTexture;


/**
* Uses preallocated space for easy rendering.
* Should allow rendering on 3d space via world matrix modifications.
*/
class VDraw
{
public:
	VVertexBufferDefined<VDrawVertex> VertexBuffer;
	VTexture* CurrentTexture;

	/**
	* Initializes and creates the VBO. Should be called after engine initialization.
	*/
	inline void Initialize()
	{
		VertexBuffer.CreateDefinedBuffer(4096);

		/*CreateBuffer(VertexBufferFormats->ColoredTextured1, 3 * 1024,RL_TRIANGLELIST);
		CreateVertexBuffer(CapacityItem);
		LockBuffer();*/
	}

	void SetTexture(VTexture* NewTexture);

	inline void NoTexture()
	{
		SetTexture( 0 );
	}

	inline void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,DefaultDiffuse);
	}

	inline void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1, const TColor32& color)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,color);
	}

	inline void DrawQuad(const IRegion& reg,const TColor32& color )
	{
		DrawQuad((float)reg.X(),(float)reg.Y(),(float)reg.Right(),(float)reg.Bottom(),0,0,1,1,color);
	}

	void SetClip(const IRectangle& rect);

	void TranslateClip(int x, int y)
	{
		throw NotImplementedException();
	}

	void ResetClip();


	void DrawImage(VTexture* image, float x, float y);

	void DrawImage(VTexture* image, float x, float y, float scale);

	// TODO: FORMAT THESE LINE DRAWING STUFF TO BETTER SPACE?

	inline void DrawLine(float x0,float y0, float x1,float y1,const TColor32& color)
	{
		ChangeMeshType(RL_LINELIST);

		SetTexture(0);
		Add2DVertexColor1Tex(x0,y0,0.0f,0.0f,color);
		Add2DVertexColor1Tex(x1,y1,0.0f,0.0f,color);
	}

	void DrawRectangle(float x,float y,float width,float height,const TColor32& color)
	{
		int mType = MeshType;

		float r = x + width;
		float b = y + height;
		DrawLine(x,y,r,y,color);
		DrawLine(r,y,r,b,color);
		DrawLine(r,b,x,b,color);
		DrawLine(x,b,x,y,color);

		ChangeMeshType(mType);
	}

	inline void DrawRectangle(const IRectangle& reg, const TColor32& color)
	{
		DrawRectangle((float)reg.X,(float)reg.Y,(float)reg.Width,(float)reg.Height,color);
	}

	inline void FillRectangle(float x,float y, float w,float h, const TColor32& color)
	{
		DrawQuad(x,y,x+w,y+h,0.0f,0.0f,1.0f,1.0f,color);
	}

	inline void FillRectangle( const IRectangle& rect, const TColor32& color)
	{
		DrawQuad((float)rect.X, (float)rect.Y,(float)(rect.Right()),(float)(rect.Bottom()),0.0f,0.0f,1.0f,1.0f,color);
	}

	/**
	* Enables alpha blending for texture and vertex color channel
	*/
	void SetDrawingState()
	{

	}
};


#endif