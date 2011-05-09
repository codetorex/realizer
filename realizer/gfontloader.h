#ifndef GFONTLOADER_H
#define GFONTLOADER_H

#include "gfont.h"

/**
* Defines basic tools for loading a font.
*/
class GFontLoader
{
public:
	GFont* font;

	GFontLoader(GFont* target)
	{
		font = target;
	}

	void SetChar(GCharacter* gchr,int x, int y, int w,int h,int xoffset, int yoffset, int xadvance);
	GCharacter* CreateCharacter(int id);
	void AddKerningPair(int first, int second, int amount);
	void SetUndefinedCharacters();
	void SetSpaceWidth();
};

#endif