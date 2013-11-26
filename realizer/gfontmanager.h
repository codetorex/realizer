#ifndef GFONTMANAGER_H
#define GFONTMANAGER_H

#include "gfontcache.h"

class GFontManager: public Array< GFont* >
{
public:
	GFontCache Cache;

	/**
	 * Gets font from cache if available, if not then it will load the font. If font not found will return 0.
	 * Use this whenever you need a font!
	 */
	GFont* GetFont(const String& fontName, int fontSize = 12, FontWeight fontWeight = RW_NORMAL , int outlineWidth = 0, bool italic = false);

	void ReleaseFont(GFont* font);


	///////////////////// THESE FUNCTIONS FOR INTERNAL USE ONLY, WELL YOU MAY STILL USE THEM BUT USE GETFONT

	/**
	 * Creates a font with given properties out of vectorial fonts like TTF. Its simple thing, but more complex things can be built via special plugins.
	 * Engine supports its own format and AngelCode's FNT. 
	 * Note: The formate used Angelcode bitmap font generator is renamed to BMF to not collide with regulat FNT.
	 * But other formats can supported with extensions.
	 */
	GFont* LoadFont(const String& path, int size, FontWeight weight, FontQuality quality,int outlineWidth, bool italic);

	/**
	 * Static fonts have their own properties baked with them. So you cant change their size weight or quality. They will just get loaded.
	 */
	GFont* LoadStaticFont(const String& path);
};


#endif