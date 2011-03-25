#ifndef VDRAW_H
#define VDRAW_H

#include "vvertexstream.h"
#include <tcolor.h>

class VDraw: public VVertexStream
{
public:
	rtex CurrentTexture;

	TColor32 DefaultDiffuse;

	/**
	* Initializes and creates the VBO. Should be called after engine initialization.
	*/
	void Initialize()
	{
		InitializeBuffer(VVertexBufferFormats::ColoredTextured1, 3 * 1024,RL_TRIANGLELIST);
		CreateVertexBuffer(Capacity);
		LockBuffer();
		DefaultDiffuse = TColor32(255,255,255,255); // default to white color
	}

	void SetTexture(rtex NewTexture);

	inline void NoTexture()
	{
		SetTexture( 0 );
	}

	void Flush();

	void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,DefaultDiffuse);
	}

	void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1, dword color)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,color);
	}
};


#endif