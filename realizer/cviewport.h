
#ifndef CVIEWPORT_H
#define CVIEWPORT_H

#include "realizertypes.h"

class RDLL Viewport
{
public:
	int vWidth;
	int vHeight;
	int vColorDepth;
	int vDepth;
	int vStencil;
	int vAntialiasing;

	bool vActive;

	void SetViewportSize(int width,int height)
	{
		vWidth = width;
		vHeight = height;
	};

	// TODO: put camera , projection and view things here. view matrix, camera matrix etc
};


#endif