#ifndef CRESOURCE_H
#define CRESOURCE_H

#include "cengine.h"

class Resource
{
public:
	enum ResourceTypes
	{
		TEXTURE,
		SHADER,
		MATERIAL,
		MODEL,
		FONT,
		MESH,
	};

	REngine*		engine;
	ResourceTypes	resourceType;
};

#endif