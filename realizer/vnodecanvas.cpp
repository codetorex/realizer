#include "stdafx.h"
#include "vnodecanvas.h"
#include "vnodecanvasimage.h"
#include "cengine.h"


VNodeCanvasImage* VNodeCanvas::DrawImage( VTexture* Texture, int dstX,int dstY, float scale, Alignment align )
{
	VNodeCanvasImage* img = new VNodeCanvasImage();
	img->Scale.x = scale;
	img->Scale.y = scale;
	img->Scale.z = scale;

	img->Texture = Texture;
	img->Part.Initialize(*Texture,0,0,Texture->Width,Texture->Height);

	img->Position.x = (float)dstX;
	img->Position.y = (float)dstY;

	img->Color.color = 0xFFFFFFFF;

	img->Align = align;

	Items.Add(img);
	return img;
}

void VNodeCanvas::Render()
{
	Engine.Renderer.Enter2D();

	int i=Items.Count;
	while(i--)
	{
		Items.Items[i]->Render();
	}

	Engine.Draw.Flush();

	Engine.Renderer.Exit2D();
}

void VNodeCanvasImage::Render()
{
	Engine.Draw.SetTexture(Texture);

	float x = Position.x;
	float y = Position.y;
	float w = Part.Width * Scale.x;
	float h = Part.Height * Scale.y;

	switch(Align)
	{
	case CA_TopLeft:
		Part.DrawScaled(x,y,w,h,Color.color);
		break;

	case CA_MiddleCenter:
		Part.DrawScaled(x-(w/2),y-(h/2),w,h,Color.color);
		break;

	default:
		throw 0;
	}
}