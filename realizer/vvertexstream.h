#ifndef VVERTEXSTREAM_H
#define VVERTEXSTREAM_H

#include <mvector3.h>
#include <tcolor.h>
#include "vvertexbuffer.h"
#include "cresource.h"

class VVertexStream: public VVertexBuffer
{
public:
	Vector3 PreTranslation;
	TColor32 DefaultDiffuse;

	VVertexStream()
	{
		DefaultDiffuse = TColors::White; // default to white color
	}

	VVertexStream(VVertexBufferFormat* _format, int _capacity,int _meshType, bool makeitReady = false): VVertexBuffer(_format,_capacity,_meshType,makeitReady)
	{
		DefaultDiffuse = TColors::White; // default to white color
	}

	void Flush();

	inline void ChangeMeshType(int mType)
	{
		if (MeshType != mType)
		{
			Flush();
			MeshType = mType;
		}
	}

	void PreTranslate(float x,float y,float z)
	{
		PreTranslation.set(x,y,z);
	}

	void ResetTranslation()
	{
		PreTranslation.set(0.0f,0.0f,0.0f);
	}

	void AddTranslatedVector3(float x,float y,float z)
	{
		float* fp = (float*)(Data + Index);
		fp[0] = x + PreTranslation.x;
		fp[1] = y + PreTranslation.y;
		fp[2] = z + PreTranslation.z;
		Index += 3 * 4;
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

	// TODO: make this function better suited.
	// TODO: consider openGL version of this will use floats?
	inline ui32 TransformColorToEngineColor(const TColor32& color)
	{
		/*ui32 clR = (color.color & 0xFF000000) >> 16;
		ui32 clB = (color.color & 0x0000FF00) << 16;
		return (color.color & 0x00FF00FF) | clR | clB;*/


		TColor32 k(color.b,color.g,color.r,color.a);
		return k.color;
		//return D3DCOLOR_RGBA(color.r,color.g,color.b,color.a);
		/*ui32 col = color.color;
		ui32 a = col & 0xFF;*/
		
	}

	void Add2DVertex1Tex(float x,float y,float u,float v)
	{
		float* fp = (float*)(Data + Index);
		fp[0] = x+PreTranslation.x;
		fp[1] = y+PreTranslation.y;
		fp[2] = 0.0f;
		fp[3] = u;
		fp[4] = v;
		Index += 5 * sizeof(float);
		Used++;
	}

	void Add2DVertexColor1Tex(float x,float y,float u,float v, const TColor32& color)
	{
		float* fp = (float*)(Data + Index);
		fp[0] = x+PreTranslation.x;
		fp[1] = y+PreTranslation.y;
		fp[2] = 0.0f;
		*(ui32*)(&fp[3]) = TransformColorToEngineColor(color);
		fp[4] = u;
		fp[5] = v;
		Index += 6 * sizeof(float);
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

	void Add2DQuadColor1Tex(float x0,float y0,float x1,float y1, float tu0,float tv0,float tu1,float tv1, const TColor32& color)
	{
		Add2DVertexColor1Tex(x0,y1,tu0,tv1,color);
		Add2DVertexColor1Tex(x0,y0,tu0,tv0,color);
		Add2DVertexColor1Tex(x1,y1,tu1,tv1,color);

		Add2DVertexColor1Tex(x0,y0,tu0,tv0,color);
		Add2DVertexColor1Tex(x1,y0,tu1,tv0,color);	
		Add2DVertexColor1Tex(x1,y1,tu1,tv1,color);
	}

	inline void Add2DQuadColor1Tex(float x0,float y0,float x1,float y1, float tu0,float tv0,float tu1,float tv1)
	{
		Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1, DefaultDiffuse );
	}
};

#endif