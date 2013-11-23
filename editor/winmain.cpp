#include "stdafx.h"
#include <realizer.h>


#include "tlog.h"
#include "tlogstream.h"

#include "tapplication.h"
#include "realizerversion.h"
#include "raiseversion.h"

#include "tevent.h"

#include "tbitmap.h"

#include "reditor.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	Application.Begin("Realizer Editor", TVersion(REALIZER_BUILD,REALIZER_PHASE), RAISE_DEFAULT_COMPANYNAME );
	USESRAISE;
	USESREALIZER;

	Engine.Renderer.InitializeRenderer(1776,1000,"Realizer Editor",false,24);

	CInputMode* baseMode = Engine.Inputs.CreateBaseInputMode();
	TTriggerAction* exitAction = baseMode->CreateBoolAction("ExitEngine",&Engine.Running);
	Engine.Inputs.BaseMode->BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	TLogStream textLog(Engine.FileSystem.Open("editor-log.txt", fm_Write));
	Log.RegisterOutput(&textLog);

	Editor.LoadResources();

	// Lets setup basic rendering pipeline/stack.
	Engine.Scenes.SetupBasicStack();

	Engine.Scenes.LoadScaneBeforeEnd(&Editor);

	TTriggerAction* consoleAction = baseMode->CreateEventAction("ActivateConsole",GetHandler(&Editor,&REditor::ActivateConsole));
	baseMode->BindKey(Keys::Tilde, consoleAction);

	// run over this stack
	Engine.Run();
	return 0;
}