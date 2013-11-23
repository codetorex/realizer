#ifndef VDRAW_H
#define VDRAW_H

#include "vvertexbuffer.h"
#include "vvertexstream.h"
#include "mregion2.h"
#include "vmesh.h"
#include "vvertexbufferformat.h"

class VTexture;


/**
* Uses preallocated space for easy rendering.
* Should allow rendering on 3d space via world matrix modifications.
*/
class VDraw
{
protected:
	void ChangeMeshType(int newMeshType)
	{
		if (Mesh.MeshType != newMeshType)
		{
			Flush();
		}

		Mesh.SetMeshType(newMeshType);
	}

public:
	VVertexBufferDefined<VDrawVertex> VertexBuffer;
	VMesh Mesh;
	VVertexBuilder Builder;

	VTexture* CurrentTexture;

	void Flush();

	inline void Translate(int x, int y)
	{
		Builder.PreTranslate((float)x,(float)y,0.0f);
	}

	inline void Translate(float x, float y, float z)
	{
		Builder.PreTranslate(x,y,z);
	}

	void ResetTransform()
	{
		Builder.ResetTranslation();
	}

	/**
	* Initializes and creates the VBO. Should be called after engine initialization.
	*/
	inline void Initialize()
	{
		VertexBuffer.CreateDefinedBuffer(4096,VertexBufferFormats->ColoredTextured1);
		VertexBuffer.ItemLength = 24;
		Mesh.Vertices = &VertexBuffer;
		Mesh.SetMeshType(RL_TRIANGLELIST);
		Builder.BeginBuild(&VertexBuffer);
	}

	void SetTexture(VTexture* NewTexture);

	inline void NoTexture()
	{
		SetTexture( 0 );
	}

	inline void DrawQuad(float x0,float y0,float x1,float y1, float tu0,float tv0, float tu1,float tv1, const TColor32& color)
	{
		Builder.Add2DQuadColor1Tex(x0,y0,x1,y1,tu0,tv0,tu1,tv1,color);

		/*Builder.WriteTranslatedVector3(x0,y1,0.0f);
		Builder.WriteColor(color);
		Builder.WriteVector2(tu0,tv0);



		VDrawVertex* vd = (VDrawVertex*)Builder.Data;
		vd++->Set(x0,y1,0.0f,color,tu0,tv1);
		vd++->Set(x0,y0,0.0f,color,tu0,tv0);
		vd++->Set(x1,y1,0.0f,color,tu1,tv1);
		vd++->Set(x0,y0,0.0f,color,tu0,tv0);
		vd++->Set(x1,y0,0.0f,color,tu1,tv0);
		vd++->Set(x1,y1,0.0f,color,tu1,tv1);
		Builder.Data = (byte*)vd;*/
	}

	inline void DrawQuad(const IRegion& reg,const TColor32& color )
	{
		DrawQuad((float)reg.X(),(float)reg.Y(),(float)reg.Right(),(float)reg.Bottom(),0,0,1,1,color);
	}

	void SetClip(const IRectangle& rect);

	void TranslateClip(int x, int y)
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void ResetClip();


	void DrawImage(VTexture* image, float x, float y);

	void DrawImage(VTexture* image, float x, float y, float scale);

	// TODO: FORMAT THESE LINE DRAWING STUFF TO BETTER SPACE?

	inline void DrawLine(float x0,float y0, float x1,float y1,const TColor32& color)
	{
		ChangeMeshType(RL_LINELIST);

		SetTexture(0);


		// TODO: replace these
		Builder.Add2DVertexColor1Tex(x0,y0,0.0f,0.0f,color);
		Builder.Add2DVertexColor1Tex(x1,y1,0.0f,0.0f,color);
	}

	void DrawRectangle(float x,float y,float width,float height,const TColor32& color)
	{
		int mType = Mesh.MeshType;

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