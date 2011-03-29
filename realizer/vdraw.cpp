#include "stdafx.h"
#include "vdraw.h"
#include "vtexture.h"

void VDraw::Flush()
{
	if (Used > 0)
	{
		UnlockBuffer();
		Engine.Renderer.RenderVertexBuffer(this);
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
	Engine.Renderer.SetTexture(0,NewTexture->texID);
	CurrentTexture = NewTexture;
}