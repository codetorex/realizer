#include "stdafx.h"
#include "gfontloader.h"
#include "vtexture.h"

void GFontLoader::SetChar( GCharacter* gchr,int x, int y, int w,int h,int xoffset, int yoffset, int xadvance )
{
	gchr->Initialize(font->FontTexture,x,y,w,h);
	gchr->SetCharacter(xoffset,yoffset,xadvance);
}

GCharacter* GFontLoader::CreateCharacter( int id )
{
	ui32 plane = (id & 0xFF00) >> 8;
	ui32 chr = (id & 0xFF);

	if (font->Characters[plane] == 0)
	{
		font->Characters[plane] = new GCharacter [256];
	}

	GCharacter* gchr = &font->Characters[plane][chr];
	return gchr;
}

void GFontLoader::SetUndefinedCharacters()
{
	for (int i=0;i<256;i++)
	{
		GCharacter* curPlane = font->Characters[i];
		if (curPlane != 0)
		{
			for (int k=0;k<256;k++)
			{
				if (curPlane[k].X == -1) // X = -1 means it wasn't loaded.
				{
					curPlane[k] = font->DefaultCharacter;
				}
			}
		}
	}
}

void GFontLoader::AddKerningPair( int first, int second, int amount )
{
	// NOT IMPLEMENTED YET
}

void GFontLoader::SetSpaceWidth()
{
	font->SpaceWidth = font->Characters[0][' '].XAdvance; // space char is on plane 0
}