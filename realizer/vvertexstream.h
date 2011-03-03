#ifndef VVERTEXSTREAM_H
#define VVERTEXSTREAM_H

#include <mvector.h>
#include "vvertexbuffer.h"
#include "cresource.h"

class VVertexStream: public VVertexBuffer
{
public:
	vec3 PreTranslation;


	VVertexStream(VVertexBufferFormat* _format, int _capacity,int _meshType): VVertexBuffer(_format,_capacity,_meshType)
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
		fp[0] = x;
		fp[1] = y;
		fp[2] = 0.0f;
		fp[3] = u;
		fp[4] = v;
		Indicator += 5 * sizeof(float);
		Used++;
	}

	void Add2DVertex1TexTranslated(float x,float y,float u,float v)
	{
		float* fp = (float*)Indicator;
		fp[0] = x + PreTranslation.x;
		fp[1] = y + PreTranslation.y;
		fp[2] = 0.0f;
		fp[3] = u;
		fp[4] = v;
		Indicator += 5 * sizeof(float);
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

	void Add2DQuad1TexTranslated(float x0,float y0,float x1,float y1, float tu0,float tv0,float tu1,float tv1)
	{
		Add2DVertex1TexTranslated(x0,y1,tu0,tv1);
		Add2DVertex1TexTranslated(x0,y0,tu0,tv0);
		Add2DVertex1TexTranslated(x1,y1,tu1,tv1);

		Add2DVertex1TexTranslated(x0,y0,tu0,tv0);
		Add2DVertex1TexTranslated(x1,y0,tu1,tv0);	
		Add2DVertex1TexTranslated(x1,y1,tu1,tv1);
	}
};

#endif