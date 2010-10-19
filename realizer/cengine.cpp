#include "stdafx.h"
#include "cengine.h"

void REngine::Initialize( int width,int height, string title, bool fullscreen /*= false*/,int bits /*= 24*/ )
{
	Renderer.InitializeRenderer(width,height,title.Chars,fullscreen,bits);
	running = false;
}

void REngine::Run()
{
	running = true;

	while( running )
	{
		Renderer.DoEvents();

	}
}
