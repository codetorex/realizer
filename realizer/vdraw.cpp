#include "stdafx.h"
#include "cengine.h"
#include "vdraw.h"

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

void VDraw::SetTexture( rtex NewTexture )
{
	if (CurrentTexture == NewTexture)
	{
		return;
	}

	Flush();
	Engine.Renderer.SetTexture(0,NewTexture);
	CurrentTexture = NewTexture;
}
