#include "stdafx.h"
#include "vtexturemanager.h"
#include "vtexture.h"

TBitmap* VTextureManager::LoadToBitmap(const str8& path)
{
	TStream* fs = Engine.FileSystem.Open(path,fm_Read);
	if (fs == NULL)
	{
		throw Exception("File not found");
	}

	TBitmap* Loader = new TBitmap();

	if (path.EndsWith("bmp"))
	{
		Loader->loadbmp(fs,true,true);
	}

	delete fs;
	return Loader;
}


VTexture* VTextureManager::LoadTexture(const str8& path, bool keepBitmap )
{
	TBitmap* Loader = LoadToBitmap(path);

	VTextureFormat* BitmapFormat = (VTextureFormat*)Loader->format;

	if (!BitmapFormat->IsSupported)
	{
		if (BitmapFormat->FallbackFormat == NULL)
		{
			throw Exception("Bitmap is in not supported format");
		}

		Loader->convert( BitmapFormat->FallbackFormat );
	}

	VTexture* LoadedTexture = new VTexture();
	LoadedTexture->width = Loader->width;
	LoadedTexture->height = Loader->height;
	LoadedTexture->path = path;
	LoadedTexture->pathHash = str8::GetHash(path);
	LoadedTexture->format = Loader->format;
	LoadedTexture->bitmap = Loader;

	LoadedTexture->CreateTexture();

	if (!keepBitmap)
	{
		delete Loader;
		LoadedTexture->bitmap = 0;
	}

	return LoadedTexture;
}