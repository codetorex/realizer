#ifndef VVERTEXBUILDER_H
#define VVERTEXBUILDER_H

#include <mvector3.h>
#include <tcolor.h>
#include "vvertexbuffer.h"
#include "cresource.h"
#include "tmemorywriter.h"

/**
 * Builds vertexes on a buffer
 */
class VVertexBuilder: public TMemoryWriter
{
public:
	VVertexBuffer* Buffer;

	Vector3 PreTranslation;
	TColor32 DefaultDiffuse;

	inline VVertexBuilder()
	{
		DefaultDiffuse = TColors::White; // default to white color
		Buffer = 0;
	}

	inline VVertexBuilder(VVertexBuffer* buf)
	{
		BeginBuild(buf);
	}


	/**
	 * Setups memoryWriter, and if vertexBuffer is not locked it will be locked
	 */
	inline void BeginBuild( VVertexBuffer* buf )
	{
		DefaultDiffuse = TColors::White; // default to white color
		Buffer = buf;
		if (!buf->Locked)
		{
			buf->LockBuffer();
		}
		InitializeMemoryWriter((byte*)buf->Data);
	}

	inline ui32 GetWrittenVertexCount()
	{
		ui32 bytesWritten = GetWrittenByteCount();
		return (bytesWritten / Buffer->ItemLength);
	}

	inline void EndBuild()
	{
		Buffer->Length = GetWrittenByteCount();
		Buffer->UnlockBuffer();
	}

	inline void PreTranslate(float x,float y,float z)
	{
		PreTranslation.set(x,y,z);
	}

	inline void ResetTranslation()
	{
		PreTranslation.set(0.0f,0.0f,0.0f);
	}

	/**
	 * Writes a vector with pre translation.
	 */
	inline void WriteTranslatedVector3(float x,float y,float z)
	{
		WriteFloat(x + PreTranslation.x);
		WriteFloat(y + PreTranslation.y);
		WriteFloat(z + PreTranslation.z);
	}

	inline void WriteVector3(const Vector3& vec)
	{
		WriteFloat(vec.x);
		WriteFloat(vec.y);
		WriteFloat(vec.z);
	}

	/**
	 * Writes a vector without pre translation.
	 */
	inline void WriteVector3(float x,float y,float z)
	{
		WriteFloat(x);
		WriteFloat(y);
		WriteFloat(z);
	}

	inline void WriteVector2(float x,float y)
	{
		WriteFloat(x);
		WriteFloat(y);
	}

	/**
	 * Writes x and y component of a Vector3
	 */
	inline void WriteVector2(const Vector3& vec)
	{
		WriteFloat(vec.x);
		WriteFloat(vec.y);
	}

	inline void WriteColor(const TColor32& clr)
	{
		ui32* p = (ui32*)Data;
		*p = TransformColorToEngineColor(clr);
		Data += sizeof(ui32);
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

	//// THESE FUNCTIONS GOING TO BE OBSOLOTE!

	void Add2DVertex1Tex(float x,float y,float u,float v)
	{
		float* fp = (float*)Data;
		fp[0] = x+PreTranslation.x;
		fp[1] = y+PreTranslation.y;
		fp[2] = 0.0f;
		fp[3] = u;
		fp[4] = v;
		Data += 5 * sizeof(float);
	}

	void Add2DVertexColor1Tex(float x,float y,float u,float v, const TColor32& color)
	{
		WriteTranslatedVector3(x,y,0.0f);
		WriteColor(color);
		WriteVector2(u,v);
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

	template <class T>
	inline void AddVertex(T& pVertex)
	{
		MemoryDriver::ShortCopy(Data,&pVertex,sizeof(T));
		Data += sizeof(T);
	}
};

#endif