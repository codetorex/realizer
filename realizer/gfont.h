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

	GCharacter(const ISize& rng,int x,int y,int w,int h, int xoff, int yoff, int xadv): VTexturePart(rng,x,y,w,h)
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

	inline void DrawCharacter(float x,float y, const TColor32& color)
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

	int RenderText(TCharacterEnumerator& schars, float x,float y, ui32 color)
	{
		Engine.Draw.SetTexture(FontTexture);
		int totalWidth = (int)x;
		
		while(schars.MoveNext())
		{	
			GCharacter* charData = GetCharacter(schars.Current);
			charData->DrawCharacter(x,y,color);
			x += charData->XAdvance;
		}
		schars.Reset();
		return ((int)x - totalWidth);
	}

	inline int RenderChar(ch32 chr, float x, float y, const TColor32& clr)
	{
		GCharacter* charData = GetCharacter(chr);
		charData->DrawCharacter(x,y,clr);
		return charData->XAdvance;
	}

	void Render(TCharacterEnumerator& text, const IRegion& screenRegion, ContentAlignment align, const TColor32& color, int xOffset = 0, int yOffset = 0, int stringPixelWidth = -1)
	{
		if (stringPixelWidth == -1)
		{
			stringPixelWidth = GetStringWidth(text);
		}

		const IRectangle& dRect = screenRegion;

		int x,y;
		switch(align)
		{
		case CA_TopLeft:
			x = dRect.X;
			y = dRect.Y;
			break;

		case CA_TopCenter:
			x = dRect.X + ((dRect.Width - stringPixelWidth) / 2);
			y = dRect.Y;
			break;

		case CA_TopRight:
			x = (screenRegion.Right()) - stringPixelWidth; // TODO: check stringPixelWidth is larger than space
			y = dRect.Y;
			break;

		case CA_MiddleLeft:
			x = dRect.X;
			y = dRect.Y + (( dRect.Height - Size ) / 2);
			break;

		case CA_MiddleCenter:
			x = dRect.X + ((dRect.Width - stringPixelWidth) / 2);
			y = dRect.Y + (( dRect.Height - Size ) / 2);
			break;

		case CA_MiddleRight:
			x = (screenRegion.Right()) - stringPixelWidth; // TODO: check stringPixelWidth is larger than space
			y = dRect.Y + (( dRect.Height - Size ) / 2);
			break;

		case CA_BottomLeft:
			x = dRect.X;
			y = ( screenRegion.Bottom() ) - Size;
			break;

		case CA_BottomCenter:
			x = dRect.X + ((dRect.Width - stringPixelWidth) / 2);
			y = ( screenRegion.Bottom() ) - Size;
			break;

		case CA_BottomRight:
			x = (screenRegion.Right() ) - stringPixelWidth; // TODO: check stringPixelWidth is larger than spacelarger than space
			y = ( screenRegion.Bottom() ) - Size;
			break;
		}
		x += xOffset;
		y += yOffset;

		RenderText(text,(float)x,(float)y,color.color);
	}

	inline void Render(const TString& text, const IRegion& screenRegion, ContentAlignment align, const TColor32& color, int xOffset = 0, int yOffset = 0, int stringPixelWidth = -1)
	{
		TCharacterEnumerator schars(text);
		Render(schars,screenRegion,align,color,xOffset,yOffset,stringPixelWidth);
	}
	
	inline void Render(const TStringBuilder& sb, const IRegion& screenRegion, ContentAlignment align, const TColor32& color, int xOffset = 0, int yOffset = 0, int stringPixelWidth = -1)
	{
		TCharacterEnumerator schars(sb);
		Render(schars,screenRegion,align,color,xOffset,yOffset,stringPixelWidth);
	}

	inline void CalculatePosition(TCharacterEnumerator& text, ContentAlignment align, IPosition& pos, const ISize& objRange)
	{

		int totalWidth = GetStringWidth(text);
	}

	inline void Render(TCharacterEnumerator& text, float x, float y, const TColor32& color, ContentAlignment align)
	{
		RenderText(text,x,y,color.color);
	}

	/**
	 * Renders a text with default aligning definitions.
	 * Default Alignment is Top Left.
	 */
	inline void Render(TCharacterEnumerator& schars,float x,float y, const TColor32& color)
	{
		RenderText(schars,x,y,color.color);
	}

	inline void Render(const TString& text,float x,float y, const TColor32& color)
	{
		TCharacterEnumerator schars(text);
		RenderText(schars,x,y,color.color);
	}

	inline void Render(const TStringBuilder& sb,float x,float y, const TColor32& color)
	{
		TCharacterEnumerator schars(sb);
		RenderText(schars,x,y,color.color);
	}

	inline int GetCharacterWidth(ch32 character)
	{
		GCharacter* chr = GetCharacter(character);
		return chr->XAdvance;
	}

	int GetStringWidth(TCharacterEnumerator& schars)
	{
		int result = 0;
		while(schars.MoveNext())
		{
			result += GetCharacterWidth(schars.Current);
		}
		schars.Reset();
		return result;
	}

	void LoadBMF(TStream* bmfstream, const TString& texturePath, bool closestream = true);
	void LoadRMF(TStream* rmfstream, bool closestream = true);
};

#endif