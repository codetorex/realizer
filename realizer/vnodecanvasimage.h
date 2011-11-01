#ifndef VNODECANVASIMAGE_H
#define VNODECANVASIMAGE_H

#include "vnodecanvas.h"
#include "vtexture.h"
#include "vtexturepart.h"


class VNodeCanvasImage: public VNodeCanvasItem
{
public:

	// TODO: implement reference point instead align?

	ContentAlignment Align;
	VTexture* Texture;
	VTexturePart Part;

	VNodeCanvasImage()
	{
		Texture = 0;
		Align = CA_TopLeft;
	}
	
	void Update()
	{

	}

	void Render();
};

#endif