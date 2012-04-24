// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>


#include "tlog.h"
#include "tlogstream.h"

#include "tapplication.h"
#include "realizerversion.h"
#include "raiseversion.h"

#include "tevent.h"

#include "tbitmap.h"

#include "rtestsuite.h"

#include "test_texture.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	Application.Begin("Realizer Test Suite", TVersion(REALIZER_BUILD,REALIZER_PHASE), RAISE_DEFAULT_COMPANYNAME );
	USESRAISE;
	USESREALIZER;

	Engine.Renderer.InitializeRenderer(1280,720,"Realizer Test",false,24);

	TestSuite.Keyboard = Engine.Inputs.CreateMappedKeyboard();
	TTriggerAction* exitAction = Engine.Inputs.CreateAction("ExitEngine",&Engine.Running);
	Engine.Inputs.BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	TLogStream textLog(Engine.FileSystem.Open("test-log.txt", fm_Write));
	Log.RegisterOutput(&textLog);

	TEventAction* nextTest = Engine.Inputs.CreateAction("NextTest",GetHandler(&TestSuite,&RTestSuite::ActivateNextTest));
	Engine.Inputs.BindKey(Keys::PageDown,nextTest);

	TEventAction* prevTest = Engine.Inputs.CreateAction("PrevTest",GetHandler(&TestSuite,&RTestSuite::ActivatePrevTest));
	Engine.Inputs.BindKey(Keys::PageUp,prevTest);

	TEventAction* consoleAction = Engine.Inputs.CreateAction("ActivateConsole",GetHandler(&TestSuite,&RTestSuite::ActivateConsole));
	Engine.Inputs.BindKey(Keys::Tilde, consoleAction);

	TestSuite.LoadResources();
	
	TestSuite.AddTest(&TestTexture);

	// Lets setup basic rendering pipeline/stack.
	Engine.Scenes.SetupBasicStack();

	Engine.Scenes.LoadScaneBeforeEnd(&TestSuite);
	Engine.Scenes.ActivateScene(&TestSuite);


	TString BGRComposition = TextureFormats->fBGR->ToString();
	Engine.Command.Output->Write(BGRComposition);

	/*TEventAction* consoleAction = Engine.Inputs.CreateAction("ActivateConsole",GetHandler(&dbgScene,&DebugScene::ActivateConsole));
	Engine.Inputs.BindKey(Keys::Tilde, consoleAction);*/

	// run over this stack
	Engine.Run();
	return 0;
}