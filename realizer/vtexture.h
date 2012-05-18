
#ifndef VTEXTURE_H
#define VTEXTURE_H

#include "realizertypes.h"
#include "cresource.h"
#include "cengine.h"
#include "vtextureformats.h"
#include "mvector2.h"
#include "msize2.h"


class RDLL VTexture: public Resource, public ISize
{
public:
	ui32		pathHash;
	TString		path;

	TBitmap*	bitmap;
	rtex		texID;

	TBufferFormat* format;

	VTexture();

	VTexture(TBitmap* source)
	{
		Width = source->Width;
		Height = source->Height;

		format = source->BufferFormat;
		bitmap = source;

		pathHash = 0;
		texID = 0;

		ResourceType = Resource::TEXTURE;
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

	inline void SetTexture(int stage = 0)
	{
		Engine.Renderer.SetTexture(stage,texID);
	}

	inline void Free()
	{
		if (texID != 0)
			DeleteTexture();

		if (bitmap != 0)
			delete bitmap;
	}

	~VTexture()
	{
		Free();
	}
};



#endif