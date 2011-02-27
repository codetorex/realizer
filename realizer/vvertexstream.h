#ifndef VVERTEXSTREAM_H
#define VVERTEXSTREAM_H

#include <mvector.h>
#include "vvertexbuffer.h"
#include "cresource.h"

class VVertexStream: public VVertexBuffer
{
public:
	vec3 PreTranslation;


	VVertexStream(VVertexBufferFormat* _format, int _capacity): VVertexBuffer(_format,_capacity)
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
};

#endif