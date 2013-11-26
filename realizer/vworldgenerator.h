#ifndef VWORLDGENERATOR_H
#define VWORLDGENERATOR_H

// can generate worlds

#include "realizertypes.h"

#include "mcircle.h"
#include "tarray.h"
#include "tbitmap.h"
#include "mrandom.h"
#include "mline.h"



class VLand
{
public:
	/// Defines center of mass and radius
	Circle2 Mass;

	Array < Vector2 > OuterPoints;

	Array < Line > OuterLines;

	/// Pangaea all continents are single continent

	/// Supercontinents are like multiple continents
	/// Continents are very large land masses
	/// Islands are any piece that is smaller than continents

	/// Craton is subdivider?

	ui32 RandomSeed;
};

class VMapParameter
{
public:

};

class VMap;

class VLandGenerator
{
public:
	VMap* Map;

	virtual VLand* GenerateLand() = 0;
};

/**
 * Randomly creates a land.
 */
class VLandGeneratorRandom: public VLandGenerator
{
public:
	float Size;

	VLandGeneratorRandom(VMap* _map)
	{
		Map = _map;
	}

	void GenerateLands();
};

/**
 * Creates a Pangaea then splits it to different continents and drifts those continents.
 */
class VLandPangaeaBased: public VLandGenerator
{
public:

};


class VMapDefinition
{
public:
	bool Lakes;
	bool Rivers;

	float LandWaterRatio; // set this to 0 for creating places looks like part of a world


};



/**
 * Map is a structure defined between 0,0 and 1,1 (like a texture)
 */
class VMap
{
public:
	Array< VLand* > Lands;
};

/**
 * Captured image of a map.
 */
class VMapView: public ISize
{
public:
	VMap* View;

	Vector2 TopLeftCoord;
	Vector2 BottomRightCoord;

	void Initialize(int _width, int _height, int x, int y, int subwidth, int subheight)
	{
		Width = _width;
		Height = _height;

		TopLeftCoord = Ratio(x,y);
		BottomRightCoord = Ratio(x + subwidth , y + subheight);
	}

	VMapView( VMap* map , ui32 pWidth, ui32 pHeight)
	{
		View = map;
		Initialize(pWidth,pHeight, 0,0,pWidth,pHeight);
	}

	//TBitmap Visualization;

	void Visualize(TBitmap* bmp);
};

class VMapGenerator
{
public:
	VMap Generation;

	MRandom* RandomGenerator;

	void GenerateLands();
};

/**
 * Will generate maps with lower detail then gradually improves it.
 * Should able to simulate "Continental drift"
 * and Erosion
 */
class VWorldGenerator
{
public:



};

#endif