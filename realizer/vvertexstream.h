#ifndef VVERTEXSTREAM_H
#define VVERTEXSTREAM_H

#include <mvector3.h>
#include "vvertexbuffer.h"
#include "cresource.h"

class VVertexStream: public VVertexBuffer
{
public:
	vec3 PreTranslation;


	VVertexStream()
	{

	}

	VVertexStream(VVertexBufferFormat* _format, int _capacity,int _meshType, bool makeitReady = false): VVertexBuffer(_format,_capacity,_meshType,makeitReady)
	{

	}

	void PreTranslate(float x,float y,float z)
	{
		PreTranslation = vec3(x,y,z);
	}

	void AddTranslatedVector3(float x,float y,float z)
	{
		float* fp = (float*)Indicator;
		fp[0] = x + PreTranslation.x;
		fp[1] = y + PreTranslation.y;
		fp[2] = z + PreTranslation.z;
		Indicator += 3 * 4;
	}


	void AddVector3(float x,float y,float z)
	{
		AddFloat(x);
		AddFloat(y);
		AddFloat(z);
	}

	void AddVector2(float x,float y)
	{
		AddFloat(x);
		AddFloat(y);
	}

	void Add2DVertex1Tex(float x,float y,float u,float v)
	{
		float* fp = (float*)Indicator;
		fp[0] = x+PreTranslation.x;
		fp[1] = y+PreTranslation.y;
		fp[2] = 0.0f;
		fp[3] = u;
		fp[4] = v;
		Indicator += 5 * sizeof(float);
		Used++;
	}

	void Add2DVertexColor1Tex(float x,float y,float u,float v, dword color)
	{
		float* fp = (float*)Indicator;
		fp[0] = x+PreTranslation.x;
		fp[1] = y+PreTranslation.y;
		fp[2] = 0.0f;
		*(dword*)(&fp[3]) = color;
		fp[4] = u;
		fp[5] = v;
		Indicator += 6 * sizeof(float);
		Used++;
	}


	void Add2DQuad1Tex(float x0,float y0,float x1,float y1, float tu0,float tv0,float tu1,float tv1)
	{
		Add2DVertex1Tex(x0,y1,tu0,tv1);
		Add2DVertex1Tex(x0,y0,tu0,tv0);
		Add2DVertex1Tex(x1,y1,tu1,tv1);

		Add2DVertex1Tex(x0,y0,tu0,tv0);
		Add2DVertex1Tex(x1,y0,tu1,tv0);	
		Add2DVertex1Tex(x1,y1,tu1,tv1);
	}

	void Add2DQuadColor1Tex(float x0,float y0,float x1,float y1, float tu0,float tv0,float tu1,float tv1,dword color)
	{
		Add2DVertexColor1Tex(x0,y1,tu0,tv1,color);
		Add2DVertexColor1Tex(x0,y0,tu0,tv0,color);
		Add2DVertexColor1Tex(x1,y1,tu1,tv1,color);

		Add2DVertexColor1Tex(x0,y0,tu0,tv0,color);
		Add2DVertexColor1Tex(x1,y0,tu1,tv0,color);	
		Add2DVertexColor1Tex(x1,y1,tu1,tv1,color);
	}
};

#endif