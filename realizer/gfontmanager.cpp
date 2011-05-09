#include "stdafx.h"
#include "gfontmanager.h"
#include "cengine.h"
#include "gfont.h"


GFont* GFontManager::LoadFont( const str8& path )
{
	TStream* fs = Engine.FileSystem.Open(path,fm_Read);
	if (fs == NULL)
	{
		throw Exception("File not found");
	}


	GFont* result = 0;

	if (path.EndsWith("rfn"))
	{
		// realizer font here, loosely baed on angelcode bitmap font generator
	}
	else if (path.EndsWith("fnt")) // AngelCode bitmap font generator's FNT
	{
		result = new GFont();
		result->LoadBMF(fs);
	}
	else
	{
		throw Exception("Font loading plugin system is not coded yet");
	}

	Add(result);
	return result;
}