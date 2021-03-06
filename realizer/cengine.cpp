#include "stdafx.h"
#include "cengine.h"
#include "realizerversion.h"

REngine Engine;

void REngine::Initialize( int width,int height, const String& title, bool fullscreen /*= false*/,int bits /*= 24*/ )
{
	Renderer.InitializeRenderer(width,height,title,fullscreen,bits);
	Running = false;
}

void REngine::Run()
{
	Running = true;

	Time.Begining();
	Draw.Initialize();
	while( Running )
	{
		Renderer.DoEvents();
		Scenes.Run();
		Time.TimePassed();
	}
}
