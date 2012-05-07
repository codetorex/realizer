#ifndef GIMAGELIST_H
#define GIMAGELIST_H

#include "gimage.h"

class TGraphics;

class GImageList
{
private:
	VTexture* BaseTexture;
	TPackedRectangle* Pack;
	Size TextureSize;

public:

	TArray< GImage* > Images;

	GImageList()
	{
		BaseTexture = 0;
		Pack = 0;
	}

	GImageList(int _width, int _height)
	{
		Initialize(_width,_height);
	}

	GImageList(VTexture* _baseTexture, TPackedRectangle* _pack);

	void Initialize(int _width, int _height);

	int AddImage(TBitmap* bmp);

	GImage& GetImage(int index)
	{
		return *Images[index];
	}

	void Draw( TGraphics* g, int x, int y, int index);
};



#endif