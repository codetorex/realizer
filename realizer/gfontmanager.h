#ifndef GFONTMANAGER_H
#define GFONTMANAGER_H

#include "gfontcache.h"

class GFontManager: public TArray< GFont* >
{
public:
	GFontCache Cache;

	/**
	* Loads a font.
	* Engine supports its own format and AngelCode's FNT. 
	* Note: The formate used Angelcode bitmap font generator is renamed to BMF to not collide with regulat FNT.
	* But other formats can supported with extensions.
	*/
	GFont* LoadFont(const str8& path);

	/**
	* Gets font from cache if available, if not then it will load the font. If font not found will return 0.
	*/
	GFont* GetFont(const str8& fontName, int fontSize = 12, FontWeight fontWeight = RW_NORMAL , int outlineWidth = 0, bool italic = false);

	GFont* CreateFont(const str8& path,int size, FontWeight weight, FontQuality quality, bool dropshadow,bool outline);

	void ReleaseFont(GFont* font);
};


#endif