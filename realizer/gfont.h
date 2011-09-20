#ifndef GFONT_H
#define GFONT_H

#include "tstring.h"
#include "genums.h"
#include "vtexturepart.h"

class VTexture;

// http://en.wikipedia.org/wiki/Kerning
class GKerning
{
public:

};

class GCharacter: public VTexturePart // 32 bytes on 32 bit space
{
public:
	int XOffset;
	int YOffset;
	int XAdvance;

	GCharacter(TRange* rng,int x,int y,int w,int h, int xoff, int yoff, int xadv): VTexturePart(rng,x,y,w,h)
	{
		XOffset = xoff;
		YOffset = yoff;
		XAdvance = xadv;
	}

	GCharacter()
	{
		X = -1;
		Kerning = 0;
	}

	inline void SetCharacter(int xoff, int yoff, int xadv)
	{
		XOffset = xoff;
		YOffset = yoff;
		XAdvance = xadv;
	}

	inline void DrawCharacter(float x,float y,ui32 color)
	{
		x += XOffset;
		y += YOffset;
		Draw(x,y,color);
	}

	GKerning* Kerning; // Its kerning class, it can support kerning pairs too
};

/*
In a second thought, I realized there is no other or better way to render text than using textures.
Hmm maybe different render function for monospaced fonts?
class GFont
{
public:
	TString Name;
	
	virtual void Render(const TString& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual void Render(const str16& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual int GetStringWidth(const TString& text) = 0;
	virtual int GetStringWidth(const str16& text) = 0;
};*/

class GFont
{
public:
	TString Name;
	TString DevName; // engine name? or dev name

	VTexture* FontTexture;
	GCharacter* Characters[256]; // TODO: Implement TIndex and use it here for supporting astral planes
	GCharacter DefaultCharacter;


	int BaseLine; // Y Offset for defining baseline of font.
	
	union
	{
		int Height; // font size
		int Size;
	};

	int OutlineWidth;
	int SpaceWidth; // Pixel width of whitespace character.

	bool Italic;

	FontWeight Weight;
	FontQuality Quality;

	GFont()
	{
		MemoryDriver::Set(Characters,0,256 * sizeof(GCharacter*));
		Height = 0;
		OutlineWidth = 0;
		SpaceWidth = 0;
		BaseLine = 0;
		FontTexture = 0;
		Quality = FQ_DEFAULT;
		Weight = RW_NORMAL;
		Italic = false;
	}

	GCharacter* GetCharacter(ch32 charID)
	{
		int plane = charID >> 8;
		int planeChar = charID & 0xFF;

		GCharacter* charPlane = Characters[plane];
		GCharacter* charData;
		if (charPlane == 0)
		{
			charData = &DefaultCharacter;
		}
		else
		{
			charData = &charPlane[planeChar];
		}
		return charData;
	}

	inline void BeginCustomRendering()
	{
		Engine.Draw.SetTexture(FontTexture);
	}

	int GetStringIndexAt(const TString& value, int x)
	{
		throw 0;
	}

	int RenderText(const TString& text, float x,float y, ui32 color)
	{
		Engine.Draw.SetTexture(FontTexture);
		int totalWidth = (int)x;
		
		TCharacterEnumerator schars(text);
		while(schars.MoveNext())
		{	
			GCharacter* charData = GetCharacter(schars.Current);
			charData->DrawCharacter(x,y,color);
			x += charData->XAdvance;
		}
		return ((int)x - totalWidth);
	}

	void Render(const TString& text, const TRegion& screenRegion, ContentAlignment align, const TColor32& color, int xOffset = 0, int yOffset = 0, int stringPixelWidth = -1)
	{
		if (stringPixelWidth == -1)
		{
			stringPixelWidth = GetStringWidth(text);
		}

		int x,y;
		switch(align)
		{
		case CA_TopLeft:
			x = screenRegion.X;
			y = screenRegion.Y;
			break;

		case CA_TopCenter:
			x = screenRegion.X + ((screenRegion.Width - stringPixelWidth) / 2);
			y = screenRegion.Y;
			break;

		case CA_TopRight:
			x = (screenRegion.Right) - stringPixelWidth; // TODO: check stringPixelWidth is larger than space
			y = screenRegion.Y;
			break;

		case CA_MiddleLeft:
			x = screenRegion.X;
			y = screenRegion.Y + (( screenRegion.Height - Size ) / 2);
			break;

		case CA_MiddleCenter:
			x = screenRegion.X + ((screenRegion.Width - stringPixelWidth) / 2);
			y = screenRegion.Y + (( screenRegion.Height - Size ) / 2);
			break;

		case CA_MiddleRight:
			x = (screenRegion.Right) - stringPixelWidth; // TODO: check stringPixelWidth is larger than space
			y = screenRegion.Y + (( screenRegion.Height - Size ) / 2);
			break;

		case CA_BottomLeft:
			x = screenRegion.X;
			y = ( screenRegion.Bottom ) - Size;
			break;

		case CA_BottomCenter:
			x = screenRegion.X + ((screenRegion.Width - stringPixelWidth) / 2);
			y = ( screenRegion.Bottom ) - Size;
			break;

		case CA_BottomRight:
			x = (screenRegion.Right) - stringPixelWidth; // TODO: check stringPixelWidth is larger than spacelarger than space
			y = ( screenRegion.Bottom ) - Size;
			break;
		}
		x += xOffset;
		y += yOffset;

		RenderText(text,(float)x,(float)y,color.color);
	}

	inline void CalculatePosition(const TString& text, ContentAlignment align, TPosition& pos, const TRange& objRange)
	{
		int totalWidth = GetStringWidth(text);
		
	}

	inline void Render(const TString& text, float x, float y, const TColor32& color, ContentAlignment align)
	{
		RenderText(text,x,y,color.color);
	}

	/**
	 * Renders a text with default aligning definitions.
	 * Default Alignment is Top Left.
	 */
	inline void Render(const TString& text,float x,float y, const TColor32& color)
	{
		RenderText(text,x,y,color.color);
	}

	int GetStringWidth(const string& text)
	{
		int result = 0;
		TCharacterEnumerator schars(text);

		while(schars.MoveNext())
		{
			GCharacter* plane = Characters[schars.Current >> 8];
			if (plane != 0)
			{
				result += plane[schars.Current & 0xFF].XAdvance;
			}
			else
			{
				result += DefaultCharacter.XAdvance;
			}
		}
		return result;
	}

	void LoadBMF(TStream* bmfstream, const TString& texturePath, bool closestream = true);
	void LoadRMF(TStream* rmfstream, bool closestream = true);
};

#endif