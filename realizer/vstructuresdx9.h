#ifndef VSTRUCTURESDX9_H
#define VSTRUCTURESDX9_H

#include "vvertexstream.h"

class VTextured2DVertexStream: public VVertexStream
{
public:
	VTextured2DVertexStream(int _cap): VVertexStream(VVertexBufferFormats::Textured2DFormat,_cap)
	{

	}

	/**
	* Adds single vertex to buffer. x , y is coordinate of vertex and u v is texture coordinate.
	*/
	void AddVertex(float x,float y,float u, float v)
	{
		//assert((Used + 20) <= Capacity);
		float* fp = (float*)Indicator;
		fp[0] = x;
		fp[1] = y;
		fp[2] = 0.0f;
		fp[3] = 1.0f;
		fp[4] = u;
		fp[5] = v;
		Indicator += 6 * 4;
		Used++;
	}

	void AddVertexTranslated(float x,float y,float u,float v)
	{
		float* fp = (float*)Indicator;
		fp[0] = x + PreTranslation.x;
		fp[1] = y + PreTranslation.y;
		fp[2] = 0.0f;
		fp[3] = 1.0f;
		fp[4] = u;
		fp[5] = v;
		Indicator += 6 * 4;
		Used++;
	}
};


#endif