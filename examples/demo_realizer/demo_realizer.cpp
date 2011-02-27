// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>

class IntroScene: public VScene
{
public:
	VTexture* TestTexture;

	void Render()
	{
		Engine.Renderer.Clear(RL_COLOR_BUFFER | RL_ZBUFFER);
		Engine.Renderer.BeginScene();



		Engine.Renderer.EndScene();
		Engine.Renderer.SwapBackBuffer();
	}

	void Update()
	{
		// Update scene for animations here
	}

	void Initialize()
	{
		SceneName = L"Intro Scene";
		// Load resources here
		TestTexture = Engine.Textures.LoadTexture("test.bmp");
	}

	void Finalize()
	{
		// Release resources here
	}
};


int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	Engine.Renderer.InitializeRenderer(1280,720,L"Realizer3D",false,24);
	
	CTriggerAction* exitAction = (CTriggerAction*)Engine.Inputs.CreateAction("ExitEngine",&Engine.running);
	Engine.Inputs.BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	IntroScene iScene;
	Engine.Scenes.ActivateScene(&iScene);

	Engine.Run();
	return 0;
}