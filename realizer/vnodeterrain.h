#ifndef VNODETERRAIN_H
#define VNODETERRAIN_H

#include "vnode.h"

class VTerrainCell
{
public:
	ui16 Elevation; // its actually between -100 and 200 since 300 meters is high enough steps with 4.5 mm ers
	byte Texture;
	byte StaticModel; // like a lamp post, or a wall like thing?

	inline float GetAbsoluteElevation()
	{
		return (((float)Elevation * 300.0f) / 65536.0f) - 100.0f;
	}
};


class VNodeTerrain: public VNode
{
public:




	virtual void GenerateVertexBuffer() = 0;
};


#endif