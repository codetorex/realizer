#include "stdafx.h"
#include "cengine.h"

REngine Engine;

void REngine::Initialize( int width,int height, const TString& title, bool fullscreen /*= false*/,int bits /*= 24*/ )
{
	Renderer.InitializeRenderer(width,height,title,fullscreen,bits);
	running = false;
}

void REngine::Run()
{
	running = true;

	Time.Begining();
	Draw.Initialize();
	while( running )
	{
		Renderer.DoEvents();
		Scenes.Run();
		Time.TimePassed();
	}
}
