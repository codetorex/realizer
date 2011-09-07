#include "stdafx.h"
#include "gfontmanager.h"
#include "cengine.h"
#include "gfont.h"

GFont* GFontManager::LoadFont( const TString& path, int size, FontWeight weight, FontQuality quality,int outlineWidth, bool italic )
{
	return LoadStaticFont(path); // ONLY THIS FOR NOW
}

GFont* GFontManager::LoadStaticFont( const TString& path )
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
		TString mainfolder = TPath::StripFilename(path);
		result = new GFont();
		result->LoadBMF(fs,mainfolder);
	}
	else
	{
		throw Exception("Font loading plugin system is not coded yet");
	}

	if (result) Add(result);
	return result;
}

GFont* GFontManager::GetFont( const TString& fontName, int fontSize /*= 12*/, FontWeight fontWeight /*= RW_NORMAL */, int outlineWidth /*= 0*/, bool italic /*= false*/ )
{
	GFontEntry* fentry = Cache.FindFontEntry(fontName); // TODO: this function maybe slow to find but if its in cache it will find it no matter what?

	for (dword i=0;i<Count;i++)
	{
		GFont* curFont = Item[i];
		if (curFont->Size == fontSize && curFont->Weight == fontWeight && curFont->OutlineWidth == outlineWidth && curFont->Italic == italic)
		{
			return curFont; // PERFECT MATCH!
		}
	}

	// none of fonts are match, lets check not loaded yet files
	for (dword i=0;i<fentry->Files.Count;i++)
	{
		GFontFile* curFile = fentry->Files.Item[i];
		if (curFile->Match(fontSize,fontWeight,italic,outlineWidth))
		{
			GFont* createdfont = LoadFont(curFile->FileName,fontSize,fontWeight,FQ_ANTIALIASED,outlineWidth,italic);
			if (createdfont)
			{
				return createdfont;
			}
			break;
		}
	}

	return 0;
}
