#include "stdafx.h"
#include "gskinmanager.h"
#include "cengine.h"
#include "gschemedskinbuilder.h"


GSkin* GSkinManager::LoadSkin( const String& path,int textureSize)
{
	TStream* fs = Engine.FileSystem.Open(path,fm_Read);
	if (fs == NULL)
	{
		throw Exception("File not found");
	}


	GSkin* result;

	if (path.EndsWith("uis"))
	{
		GSchemedSkinBuilder gsb;
		gsb.Begin(textureSize,textureSize); // maybe needed to change?
		gsb.LoadFromScheme(fs);
		result = gsb.Finish();
	}
	else if (path.EndsWith("rsk")) // realizer skin
	{

	}
	else
	{
		throw Exception("Skin loading plugin system is not coded yet");
	}

	Add(result);
	return result;
}