#ifndef CRESOURCE_H
#define CRESOURCE_H

class REngine;

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
		VERTEXBUFFER,
	};

	//REngine*		engine;
	ui32			ReferenceCount;
	ResourceTypes	ResourceType;
};

#endif