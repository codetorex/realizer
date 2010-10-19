
#ifndef VTEXTURE_H
#define VTEXTURE_H

#include "realizertypes.h"
#include "cresource.h"
#include <tbitmap.h>

class RDLL VTexture: public Resource
{
public:
	dword		pathHash;
	str8		path;

	TBitmap*	bitmap;
	rtex		texID;

	int			width;
	int			height;

	VTexture()
	{
		pathHash = 0;
		bitmap = 0;
		texID = 0;
		width = 0;
		height = 0;
	}

	inline void CreateTexture()
	{
		if (texID != 0)
			throw "Already created";

		if (bitmap == 0)
			throw "No bitmap present";

		texID = engine->Renderer.LoadTextureFromBitmap(this);
	}

	inline void DeleteTexture()
	{
		if (texID == 0)
			throw "Texture not created";

		engine->Renderer.DeleteTexture(texID);
	}

	inline void UpdateTexture()
	{
		if (texID == 0)
			throw "Texture not created";
		engine->Renderer.UpdateTextureFromBitmap(this->texID,this);
	}

};



#endif