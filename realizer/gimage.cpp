#include "stdafx.h"
#include "gimage.h"
#include "vgui.h"
#include "vtexture.h"
#include "tgraphics.h"

void GImage::Render( int x, int y )
{
	Engine.Draw.SetTexture(Texture);
	TexturePart.Draw(x,y);
}

void GImage::Render( int x, int y, int _width, int _height )
{
	Engine.Draw.SetTexture(Texture);
	TexturePart.DrawScaled(x,y,_width,_height);
}

GImage::GImage()
{
	ClassID = GIMAGE_CLASSID;
	Stretch = false;
	Visible = false;
}
