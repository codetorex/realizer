#ifndef VSCENERENDER_H
#define VSCENERENDER_H

#include "vscene.h"
#include "tcolor.h"

/**
 * Precoded basic scenes that begins the actual rendering and ends it.
 */

class VSceneRenderBegin: public VScene
{
public:
	TColor32 ClearColor;

	VSceneRenderBegin()
	{
		Type = ST_BEGIN;
		Flags = SF_RENDER;
		ClearColor = 0xFF000000;
	}

	void Render();
};

class VSceneRenderEnd: public VScene
{
public:

	VSceneRenderEnd()
	{
		Type = ST_END;
		Flags = SF_RENDER;
	}

	void Render();
};


#endif