#include "stdafx.h"
#include "vdraw.h"
#include "vtexture.h"



void VVertexStream::Flush()
{
	if (Used > 0)
	{
		UnlockBuffer();
		Engine.Renderer.RenderVertexBuffer(this);
		MeshType = RL_TRIANGLELIST;
	}

	if (!Locked)
	{
		LockBuffer();
	}
}

void VDraw::SetTexture( VTexture* NewTexture )
{
	if (CurrentTexture == NewTexture)
	{
		return;
	}

	Flush();
	if (NewTexture != 0)
	{
		Engine.Renderer.SetTexture(0,NewTexture->texID);
	}
	else
	{
		Engine.Renderer.SetTexture(0,0);
	}
	
	CurrentTexture = NewTexture;
}

void VDraw::DrawImage( VTexture* image, float x, float y )
{
	SetTexture(image);
	DrawQuad(x,y,x+(float)image->Width,y+(float)image->Height,0.0f,0.0f,1.0f,1.0f);
}

void VDraw::DrawImage( VTexture* image, float x, float y, float scale )
{
	SetTexture(image);
	int newW = (float)image->Width * scale;
	int newH = (float)image->Height * scale;
	DrawQuad(x,y,x+newW,y+newH,0.0f,0.0f,1.0f,1.0f);
}