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