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
#include "test_enginecolor.h"
#include "test_gui.h"
#include "test_modelobj.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	Application.Begin("Realizer Test Suite", TVersion(REALIZER_BUILD,REALIZER_PHASE), RAISE_DEFAULT_COMPANYNAME );
	USESRAISE;
	USESREALIZER;

	Engine.Renderer.InitializeRenderer(1280,720,"Realizer Test",false,24);

	CInputMode* baseMode = Engine.Inputs.CreateBaseInputMode();
	TTriggerAction* exitAction = baseMode->CreateBoolAction("ExitEngine",&Engine.Running);
	Engine.Inputs.BaseMode->BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	TLogStream textLog(Engine.FileSystem.Open("test-log.txt", fm_Write));
	Log.RegisterOutput(&textLog);

	TTriggerAction* nextTest = baseMode->CreateEventAction("NextTest",GetHandler(&TestSuite,&RTestSuite::ActivateNextTest));
	baseMode->BindKey(Keys::PageDown,nextTest);

	TTriggerAction* prevTest = baseMode->CreateEventAction("PrevTest",GetHandler(&TestSuite,&RTestSuite::ActivatePrevTest));
	baseMode->BindKey(Keys::PageUp,prevTest);

	TTriggerAction* consoleAction = baseMode->CreateEventAction("ActivateConsole",GetHandler(&TestSuite,&RTestSuite::ActivateConsole));
	baseMode->BindKey(Keys::Tilde, consoleAction);

	TestSuite.LoadResources();
	
	TestSuite.AddTest(&TestTexture);
	TestSuite.AddTest(&TestColor);
	TestSuite.AddTest(&TestGUI);
	TestSuite.AddTest(&TestModelObj);

	

	// Lets setup basic rendering pipeline/stack.
	Engine.Scenes.SetupBasicStack();

	Engine.Scenes.LoadScaneBeforeEnd(&TestSuite);
	Engine.Scenes.ActivateScene(&TestSuite);


	TString BGRComposition = TextureFormats->fBGR->ToString();
	Engine.Command.Output->Write(BGRComposition);

	TestSuite.ActivateLastTest();

	/*TEventAction* consoleAction = Engine.Inputs.CreateAction("ActivateConsole",GetHandler(&dbgScene,&DebugScene::ActivateConsole));
	Engine.Inputs.BindKey(Keys::Tilde, consoleAction);*/

	// run over this stack
	Engine.Run();
	return 0;
}