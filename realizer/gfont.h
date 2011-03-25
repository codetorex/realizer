#ifndef GFONT_H
#define GFONT_H

#include "tstring.h"
#include "genums.h"
#include "vtexturepart.h"

class VTexture;

class GFont
{
public:
	str8 Name;
	
	virtual void Render(const str8& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual void Render(const str16& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual int GetStringWidth(const str8& text) = 0;
	virtual int GetStringWidth(const str16& text) = 0;
};

class GCharacter: public VTexturePart
{
public:
	int XOffset;
	int YOffset;
	int XAdvance;

	GCharacter(VTexture* texture,int x,int y,int w,int h, int xoff, int yoff, int xadv): VTexturePart(texture,x,y,w,h)
	{
		XOffset = xoff;
		YOffset = yoff;
		XAdvance = xadv;
	}
};

class GTexturedFont: public GFont
{
public:
	VTexture* FontTexture;


	int BaseLine; // Y Offset for defining baseline of font.
	
	union
	{
		int Height; // font size
		int Size;
	};

	bool HasOutline;
	int SpaceWidth; // Pixel width of whitespace character.

	


	void Render(const str8& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign)
	{

	}

	void Render(const str16& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign)
	{

	}

	int GetStringWidth(const str8& text)
	{
		throw Exception("Not Implemented");
		return 0;
	}

	int GetStringWidth(const str16& text)
	{
		throw Exception("Not Implemented");
		return 0;
	}

};

#endif