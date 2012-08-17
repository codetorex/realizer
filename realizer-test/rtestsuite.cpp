#include "stdafx.h"
#include "rtestsuite.h"
#include "cengine.h"
#include "gwireframeskin.h"
#include "gfont.h"

RTestSuite TestSuite;

void RTestSuite::LoadResources()
{
	try
	{
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write );
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		DebugFont = Engine.GUI.Fonts.GetFont("Bitstream Vera Sans Mono",14);

		DebugSkin = new GWireFrameSkin();
		DebugSkin->SystemFont = DebugFont;
		DebugSkin->DefaultFontColor = TColors::White;

		CurrentTest = 0;
		CurrentTestIndex = -1;

		Engine.Renderer.EnableBlending();
		Engine.Renderer.EnableTextureAlphaVertexColorAlpha(0);

		TestDesktop = Engine.GUI.CreateDesktop(true);
		TestDesktop->Font = DebugFont;

		TestConsole = new GConsole();
		TestDesktop->AddChild(TestConsole); // auto sets font

		TestConsole->InitializeConsole(80,24,96);
		TestConsole->X = 0;
		TestConsole->Y = TestDesktop->Height - TestConsole->Height;

		TestDesktop->Layout();

		Engine.Command.Start(&TestConsole->Buffer);
		Engine.Command.ConnectAsLogOutput(); // TODO: make this somewhere internal?
	}
	catch( Exception& e )
	{
		TStringBuilderStack<1024> sb;
		sb.AppendLine("Error when loading most basic stuff:");
		sb.Append(e.Message);

		TWinTools::ShowMessage(sb.ToString());
	}
}

void RTestSuite::ActivateNextTest(bool value)
{
	if (!value) return;

	if (CurrentTestIndex == Tests.Count-1)
	{
		return;
	}

	ActivateTest(Tests[CurrentTestIndex + 1]);
}

void RTestSuite::ActivatePrevTest(bool value)
{
	if (!value) return;

	if( CurrentTestIndex <= 0)
	{
		CurrentTestIndex = -1;
		CurrentTest = 0;
		return;
	}

	ActivateTest(Tests[CurrentTestIndex - 1]);
}

void RTestSuite::ActivateTest( RTestScene* Test )
{
	if (CurrentTest == Test)
	{
		return;
	}

	int i = Tests.IndexOf(Test);
	if (i == -1)
	{
		throw Exception("Test is not registered");
	}

	if (CurrentTest)
	{
		CurrentTest->OnDeactivated();
	}

	if (!Test->Initialized)
	{
		Test->Initialize();
		Test->Initialized = true;
	}

	CurrentTest = Test;
	CurrentTestIndex = i;

	Test->OnActivated();
}

void RTestSuite::WriteTextRight( TCharacterEnumerator schars )
{
	WriteText(schars,RightRegion, CA_TopRight);
}

void RTestSuite::WriteTextLeft( TCharacterEnumerator schars )
{
	WriteText(schars,LeftRegion);
}

void RTestSuite::Render()
{
	if (CurrentTest)
	{
		TestSuite.CurrentTest->Render();
	}

	Engine.Renderer.Enter2D();

	Engine.GUI.RenderDesktop(TestDesktop);

	LeftRegion.SetRegion(10,10,Engine.Renderer.vWidth,10);
	RightRegion.SetRegion(10,10,Engine.Renderer.vWidth-20,10);

	WriteText(Application.IdentifyText,LeftRegion);
	WriteText(Application.Modules.Item[0]->IdentifyText,RightRegion,CA_TopRight);

	sb.Clear();
	sb.Append("Current test: ");
	sb.AppendPadded(CurrentTestIndex+1,2,' ');
	sb.AppendChar('\\');
	sb.Append(Tests.Count);
	sb.Append("  [ ");
	
	if (TestSuite.CurrentTest)
	{
		sb.Append(TestSuite.CurrentTest->SceneName);
	}
	else
	{
		sb.Append("NONE");
	}

	sb.Append(" ]");
	
	WriteText(sb,LeftRegion);

	// TODO: make these functions to a class named FPSCounter.Update
	ui32 frameMS = Engine.Time.RealTimeMS - LastFrame;
	LastFrame = Engine.Time.RealTimeMS;

	if(Engine.Time.RealTimeMS - LastFPS > 1000)
	{
		LastFPS = Engine.Time.RealTimeMS;
		FPS = FrameCount;
		FrameCount = 0;
	}
	FrameCount++;

	sb.Clear();
	sb.Append("FPS: ");
	sb.Append(sfu(FPS,-8));
	WriteText(sb,RightRegion, CA_TopRight);
	

	TStringBuilderStack<64> ss;
	ss.Append(sfu(1000/FPS));
	ss.Append(" ms");

	sb.Clear();
	sb.Append("Average Frame Time: ");
	sb.AppendPadded(ss,-8,' ');

	WriteText(sb,RightRegion, CA_TopRight);

	ss.Clear();
	ss.Append(sfu(frameMS));
	ss.Append(" ms");

	sb.Clear();
	sb.Append("Frame Time: ");
	sb.AppendPadded(ss,-8,' ');
	WriteText(sb,RightRegion, CA_TopRight);

	if (CurrentTest)
	{
		CurrentTest->DrawData();
	}

	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}

void RTestSuite::AddTest( RTestScene* test )
{
	Tests.Add(test);
}

void RTestSuite::Update()
{
	if (CurrentTest)
	{
		CurrentTest->Update();
	}
}

void RTestSuite::WriteText( TCharacterEnumerator schars , IRegion& reg, Alignment alg)
{
	TestSuite.DebugFont->Render(schars, reg, alg, TColors::White);
	reg.MoveYDiff( DebugFont->Height );
}

void RTestSuite::ActivateConsole(bool value)
{
	if ( DebugInputEnabled && value )
		return;

	if ( !DebugInputEnabled && value)
		return;

	if (!DebugInputEnabled)
	{
		OldEnabled = Engine.GUI.Enabled;
		OldDesktop = Engine.GUI.Desktop;
		OldFocused = Engine.GUI.Focused;

		if (!OldEnabled)
		{
			Engine.GUI.EnableGUI(0,TestDesktop);
		}

		TestConsole->SetFocus();
		DebugInputEnabled = true;
	}
	else
	{
		if (!OldEnabled)
		{
			Engine.GUI.DisableGUI();
		}
		Engine.GUI.Desktop = OldDesktop;
		OldFocused->SetFocus();

		DebugInputEnabled = false;
	}
}

void RTestSuite::ActivateLastTest()
{
	ActivateTest(Tests.GetLast());
}