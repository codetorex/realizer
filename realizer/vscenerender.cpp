#include "stdafx.h"
#include "vscenerender.h"
#include "cengine.h"

void VSceneRenderBegin::Render()
{
	Engine.Renderer.Clear(RL_COLOR_BUFFER | RL_ZBUFFER, ClearColor.color);
	Engine.Renderer.BeginScene();

	// DO FPS CALCULATION HERE
}


void VSceneRenderEnd::Render()
{
	Engine.Renderer.EndScene();
	Engine.Renderer.SwapBackBuffer();
}