#ifndef CRESOURCE_H
#define CRESOURCE_H

class REngine;

#include "tarray.h"

class Resource
{
public:
	enum ResourceTypes
	{
		SCENE,
		TEXTURE,
		FONT,
		SHADER,
		MATERIAL,
		MODEL,
		VERTEXBUFFER,
	};

	Resource()
	{
		ReferenceCount = 0;
	}

	ui32			ReferenceCount;
	ResourceTypes	ResourceType;

	/// Sub resources related/required with this resource. Like you can load a model but its texture associate with it will be its child resource.
	TArray<Resource*>	Nodes;

	/**
	 * Frees resource and all resources related with it, if they not reference by other resources.
	 */
	virtual void	Free() = 0;
};

#endif