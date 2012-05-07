#include "stdafx.h"
#include "vtexturemanager.h"
#include "vtexture.h"
#include "thashcodeprovider.h"
#include "tlog.h"
#include "tbitmapreader.h"
#include "mmathcolor.h"

TBitmap* VTextureManager::LoadToBitmap(const TString& path)
{
	TStream* fs = Engine.FileSystem.Open(path,fm_Read);
	if (fs == NULL)
	{
		throw Exception("File not found");
	}

	ui32 bitmapExtension = TPath::GetExtensionAsDword(path);

	TBitmap* Loader = new TBitmap();
	TBitmapReader* myReader = TBitmapReader::GetReader(bitmapExtension);

	if (myReader != 0)
	{
		myReader->ReadBitmap(Loader,fs);
		fs->Close();
	}
	else
	{
		throw Exception("This texture format is not supported");
	}

	// delete fs; Remains of old system.
	return Loader;
}


VTexture* VTextureManager::LoadTexture(const TString& path, bool keepBitmap )
{
	TBitmap* Loader = LoadToBitmap(path);


	Log.Output(LG_INF,"Loading texture %", sfs(path));

	VTexture* LoadedTexture = CreateTexture(Loader);
	LoadedTexture->path = path;
	LoadedTexture->pathHash = TBasicHashCodeProvider::Instance.GetHashCode(path);

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

	Log.Output(LG_INF,"Created texture out of bitmap with size of %x% Format:%", sfu(source->Width),sfu(source->Height),sfs(source->BufferFormat->ShortName));

	return createdTexture;
}