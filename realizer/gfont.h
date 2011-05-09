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

	inline void DrawCharacter(float x,float y,dword color)
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
	str8 Name;
	
	virtual void Render(const str8& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual void Render(const str16& text, float x, float y, const TColor32& color, TextHorizontalAlign xalign, TextVerticalAlign yalign) = 0;
	virtual int GetStringWidth(const str8& text) = 0;
	virtual int GetStringWidth(const str16& text) = 0;
};*/

class GFont
{
public:
	str8 DevName; // engine name? or dev name
	str8 Name;
	

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
	}

	template <class T>
	int RenderText(T* text, int textLength, float x,float y, dword color)
	{
		Engine.Draw.SetTexture(FontTexture);


		int totalWidth = (int)x;

		while(textLength--)
		{
			ch16 curChar = (ch16)*text;
			int plane = curChar >> 8;
			int planeChar = curChar & 0xFF;

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

			charData->DrawCharacter(x,y,color);

			x += charData->XAdvance;
			text++;
		}
		return ((int)x - totalWidth);
	}

	inline void CalculatePosition(float& x, float& y, TextHorizontalAlign xalign, TextVerticalAlign yalign)
	{
		
	}

	inline void Render(const str8& text, float x, float y, const TColor32ARGB& color, TextHorizontalAlign xalign, TextVerticalAlign yalign)
	{
		RenderText(text.Chars,text.Length,x,y,color.color);
	}

	inline void Render(const str16& text, float x, float y, const TColor32ARGB& color, TextHorizontalAlign xalign, TextVerticalAlign yalign)
	{
		RenderText(text.Chars,text.Length,x,y,color);
		//Engine.Draw.DrawLine(x,y + (Height / 2),1024,y+ (Height / 2),color);
		//Engine.Draw.Flush();
	}

	/**
	* Renders a text with default aligning definitions.
	* Default Alignment is Top Left.
	*/
	inline void Render(const str16& text,float x,float y, const TColor32ARGB& color)
	{
		RenderText(text.Chars,text.Length,x,y,color);
	}

	inline void Render(const str8& text,float x,float y, const TColor32ARGB& color)
	{
		RenderText(text.Chars,text.Length,x,y,color);
	}

	int GetStringWidth(const str8& text)
	{
		int length = text.Length;
		GCharacter* plane0 = Characters[0];
		int result = 0;
		while(length--)
		{
			result += plane0[text.Chars[length]].XAdvance;
		}
		return result;
	}

	int GetStringWidth(const str16& text)
	{
		int length = text.Length;
		
		int result = 0;
		ch16* ptext = text.Chars;
		while(length--)
		{
			ch16 curChar = *ptext;
			GCharacter* plane = Characters[curChar >> 8];
			if (plane != 0)
			{
				result += plane[curChar & 0xFF].XAdvance;
			}
			else
			{
				result += DefaultCharacter.XAdvance;
			}
			ptext++;
		}
		return result;
	}

	void LoadBMF(TStream* bmfstream, bool closestream = true);
	void LoadRMF(TStream* rmfstream, bool closestream = true);
};

#endif