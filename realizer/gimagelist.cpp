#include "stdafx.h"
#include "gimagelist.h"
#include "vgui.h"
#include "vtexture.h"
#include "tgraphics.h"
#include "tpackedrectangle.h"

GImageList::GImageList( VTexture* _baseTexture, TPackedRectangle* _pack )
{
	if (_baseTexture->bitmap == NULL)
	{
		throw Exception("Invalid argument");
	}

	BaseTexture = _baseTexture;
	Pack = _pack;
	TextureSize.SetSize(*BaseTexture);
}

void GImageList::Initialize( int _width, int _height )
{
	TextureSize.SetSize(_width,_height);
}

GImage* GImageList::AddImage( TBitmap* bmp )
{
	if (BaseTexture == 0)
	{
		throw Exception("Object not initialized properly");
	}

	TRectangleNode* place = Pack->Insert(bmp);

	TBitmapGraphics g(BaseTexture->bitmap);
	g.DrawImage2(*bmp,*place);
	BaseTexture->UpdateTexture();

	GImage* r = new GImage();
	r->SetRectangle(0,0,bmp->Width,bmp->Height);
	r->Texture = BaseTexture;
	r->TexturePart.Initialize(*BaseTexture, *place);
	Images.Add(r);

	return r;
}

GImage* GImageList::AddImage( const TString& path )
{
	TBitmap* bmp = Engine.Textures.LoadToBitmap(path);
	GImage* result = AddImage(bmp);
	delete bmp;
	return result;
}

void GImageList::Draw( TGraphics* g, int x, int y, int index )
{
	GImage& img = GetImage(index);
	g->DrawImage(*BaseTexture->bitmap,x,y,img.TexturePart.X,img.TexturePart.Y,img.TexturePart.Width,img.TexturePart.Height);
}