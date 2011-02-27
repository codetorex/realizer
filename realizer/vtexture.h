
#ifndef VTEXTURE_H
#define VTEXTURE_H

#include "realizertypes.h"
#include "cresource.h"
#include "cengine.h"
#include "vtextureformats.h"


class RDLL VTexture: public Resource
{
public:
	dword		pathHash;
	str8		path;

	TBitmap*	bitmap;
	rtex		texID;

	int			width;
	int			height;

	TBufferFormat* format;

	VTexture()
	{
		pathHash = 0;
		bitmap = 0;
		texID = 0;
		width = 0;
		height = 0;

		resourceType = Resource::TEXTURE;
	}

	inline void CreateTexture()
	{
		if (texID != 0)
			throw Exception("Already created");

		if (bitmap == 0)
			throw Exception("No bitmap present");

		texID = Engine.Renderer.LoadTextureFromBitmap(this->bitmap);
	}

	inline void DeleteTexture()
	{
		if (texID == 0)
			throw Exception("Texture not created");

		Engine.Renderer.DeleteTexture(texID);
	}

	inline void UpdateTexture()
	{
		if (texID == 0)
			throw Exception("Texture not created");

		if (bitmap == 0)
			throw Exception("No bitmap present");

		Engine.Renderer.UpdateTextureFromBitmap(this->texID,this->bitmap);
	}
};



#endif