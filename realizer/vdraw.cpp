#include "stdafx.h"
#include "vdraw.h"
#include "vtexture.h"



void VDraw::Flush()
{
	if (Builder.GetWrittenVertexCount() > 0)
	{
		Builder.EndBuild();
		Mesh.UpdatePrimitiveCount();
		//Engine.Renderer.RenderVertexBuffer(&VertexBuffer);
		Engine.Renderer.RenderMesh(&Mesh);
		Mesh.SetMeshType(RL_TRIANGLELIST);
	}

	if (!VertexBuffer.Locked)
	{
		Builder.BeginBuild(&VertexBuffer);
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
	DrawQuad(x,y,x+(float)image->Width,y+(float)image->Height,0.0f,0.0f,1.0f,1.0f,TColors::White);
}

void VDraw::DrawImage( VTexture* image, float x, float y, float scale )
{
	SetTexture(image);
	int newW = (float)image->Width * scale;
	int newH = (float)image->Height * scale;
	DrawQuad(x,y,x+newW,y+newH,0.0f,0.0f,1.0f,1.0f,TColors::White);
}

void VDraw::SetClip( const IRectangle& rect )
{
	Engine.Renderer.EnableScissor(rect);
}

void VDraw::ResetClip()
{
	Engine.Renderer.DisableScissor();
}