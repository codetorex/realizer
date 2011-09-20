#ifndef VDRAW_H
#define VDRAW_H

#include "vvertexstream.h"

class VTexture;


/**
* Uses preallocated space for easy rendering.
* Should allow rendering on 3d space via world matrix modifications.
*/
class VDraw: public VVertexStream
{
public:
	VTexture* CurrentTexture;

	/**
	* Initializes and creates the VBO. Should be called after engine initialization.
	*/
	void Initialize()
	{
		InitializeBuffer(VVertexBufferFormats::ColoredTextured1, 3 * 1024,RL_TRIANGLELIST);
		CreateVertexBuffer(CapacityItem);
		LockBuffer();
	}

	void SetTexture(VTexture* NewTexture);

	inline void NoTexture()
	{
		SetTexture( 0 );
	}

	void Flush();

	void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,DefaultDiffuse);
	}

	void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1, ui32 color)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,color);
	}


	// TODO: FORMAT THESE LINE DRAWING STUFF TO BETTER SPACE?

	inline void DrawLine(float x0,float y0, float x1,float y1,ui32 color)
	{
		if (MeshType != RL_LINELIST)
		{
			Flush();
			MeshType = RL_LINELIST; 
		}

		SetTexture(0);
		Add2DVertexColor1Tex(x0,y0,0.0f,0.0f,color);
		Add2DVertexColor1Tex(x1,y1,0.0f,0.0f,color);
	}

	void DrawRectangle(float x,float y,float width,float height,ui32 color)
	{
		float r = x + width;
		float b = y + height;
		DrawLine(x,y,r,y,color);
		DrawLine(r,y,r,b,color);
		DrawLine(r,b,x,b,color);
		DrawLine(x,b,x,y,color);
	}

	/**
	* Enables alpha blending for texture and vertex color channel
	*/
	void SetDrawingState()
	{

	}
};


#endif