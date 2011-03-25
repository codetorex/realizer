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
		Loader->loadbmp(fs,false,true);
	}
	else if (path.EndsWith("tga"))
	{
		Loader->loadtga(fs,false,true);
	}
	else
	{
		// TODO: code texture loading plugin system... and implement it here.
		throw Exception("Texture loading plugin system is not coded yet");
	}

	// delete fs; Remains of old system.
	return Loader;
}


VTexture* VTextureManager::LoadTexture(const str8& path, bool keepBitmap )
{
	TBitmap* Loader = LoadToBitmap(path);

	VTextureFormat* BitmapFormat = (VTextureFormat*)Loader->BufferFormat;

	if (!BitmapFormat->IsSupported)
	{
		if (BitmapFormat->FallbackFormat == NULL)
		{
			throw Exception("Bitmap is in not supported format");
		}

		Loader->Convert( BitmapFormat->FallbackFormat );
	}

	VTexture* LoadedTexture = CreateTexture(Loader);
	LoadedTexture->path = path;
	LoadedTexture->pathHash = str8::GetHash(path);

	if (!keepBitmap)
	{
		delete Loader;
		LoadedTexture->bitmap = 0;
	}

	// Texture was added to list in CreateTexture function call.
	return LoadedTexture;
}

void VTextureManager::ReleaseTexture( VTexture* texture )
{
	Remove(texture);
	delete texture;
}

VTexture* VTextureManager::CreateTexture( TBitmap* source )
{
	VTexture* createdTexture = new VTexture(source);
	createdTexture->CreateTexture();
	Add(createdTexture);
	return createdTexture;
}