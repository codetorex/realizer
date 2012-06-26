#ifndef VNODETERRAIN_H
#define VNODETERRAIN_H

#include "vnode.h"

class TStream;
class VShaderPipeline;

/**
 * TODO: extend this and move to Raise?
 * Could be even used in TBitmap
 */
template <class T>
class TArray2D: public ISize
{
public:
	T* Cells;

	TArray2D()
	{
		Width = 0;
		Height = 0;
		Cells = 0;
	}

	TArray2D(int nWidth, int nHeight)
	{
		Cells = 0;
		Allocate(nWidth,nHeight);
	}

	void Allocate(int newWidth, int newHeight)
	{
		if (Cells)
		{
			delete [] Cells;
		}

		int cellsNeeded = newWidth * newHeight;
		Cells = new T [cellsNeeded];
		Width = newWidth;
		Height = newHeight;
	}

	inline T& GetCell(int x,int y)
	{
		return Cells[(y * Width) + x];
	}
};

class VTerrainCellInterface
{
public:
	virtual float GetAbsoluteElevation() = 0;
	virtual void Write(TStream* stream) = 0;
	virtual void Read(TStream* stream) = 0;
};

class VTerrainDataInterface
{
public:
	 virtual void GenerateVertexBuffer() = 0;
	 virtual VShaderPipeline* GetShaderPipeline();
};

/**
 * Generic cell data
 */
class VTerrainCell
{
public:
	ui16 Elevation; // its actually between -100 and 200 since 300 meters is high enough steps with 4.5 mm ers
	byte Texture;
	byte StaticModel; // like a lamp post, or a wall like thing?
	ui32 Color;

	inline float GetAbsoluteElevation()
	{
		return (((float)Elevation * 300.0f) / 65536.0f) - 100.0f;
	}
};

class VTerrainData: public TArray2D< VTerrainCell >
{
public:
	
	

};

class VNodeTerrain: public VNode
{
public:


	
};


#endif