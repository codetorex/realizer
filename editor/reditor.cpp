#include "stdafx.h"
#include "reditor.h"
#include "cengine.h"
#include "gwireframeskin.h"
#include "gfont.h"
#include "gschemedskin.h"
#include "gconsole.h"
#include "gcomponents.h"

#include "rtextview.h"
#include "rtextdocument.h"

REditor Editor;

REditorResources Resources;


void REditor::LoadResources()
{
	try
	{
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write );
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		DebugFont = Engine.GUI.Fonts.GetFont("Bitstream Vera Sans Mono",14);

		EditorSkin = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");
		Engine.GUI.EnableGUI(EditorSkin);


		Engine.Renderer.EnableBlending();
		Engine.Renderer.EnableTextureAlphaVertexColorAlpha(0);

		/*TestDesktop = Engine.GUI.CreateDesktop(true);
		TestDesktop->Font = DebugFont;*/

		DebugConsole = new GConsole();
		Engine.GUI.Desktop->OwnObject(DebugConsole);
		Engine.GUI.Desktop->OnTopObjects.Add(DebugConsole); 
		DebugConsole->Font = DebugFont;

		//DebugConsole->Font = DebugFont;
		DebugConsole->InitializeConsole(80,24,96);
		DebugConsole->X = 0;
		DebugConsole->Y = Engine.GUI.Desktop->Height - DebugConsole->Height;

		Engine.Command.Start(&DebugConsole->Buffer);
		Engine.Command.ConnectAsLogOutput(); // TODO: make this somewhere internal?

		Resources.RealizerLogo = Engine.Textures.LoadTexture("realizer.png");
		Resources.StartPageTexture = Engine.Textures.LoadTexture("editor/start-page.png");

		InitializeMainGui();
	}
	catch( Exception& e )
	{
		TStringBuilder sb;
		sb.AppendLine("Error when loading most basic stuff:");
		sb.Append(e.Message);

		TWinTools::ShowMessage(sb.ToString());
	}
}

void REditor::Render()
{
	Engine.Renderer.Enter2D();

	Engine.GUI.Render();

	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}

void REditor::ActivateConsole( bool value )
{
	if ( DebugInputEnabled && value )
		return;

	if ( !DebugInputEnabled && value)
		return;

	if (!DebugInputEnabled)
	{
		OldFocused = Engine.GUI.Focused;
		DebugConsole->SetFocus();
		DebugInputEnabled = true;
	}
	else
	{
		OldFocused->SetFocus();
		DebugInputEnabled = false;
	}
}

void REditor::InitializeMainGui()
{
	GMenuStrip* mainMenu = new GMenuStrip();
	mainMenu->SetRectangle(0,0,100,20);
	mainMenu->Dock = DCK_TOP;
	Engine.GUI.Desktop->AddChild(mainMenu);

	mainMenu->AddItem("File");
	mainMenu->AddItem("Edit");
	mainMenu->AddItem("View");
	mainMenu->AddItem("Project");
	mainMenu->AddItem("Debug");
	mainMenu->AddItem("Tools");
	mainMenu->AddItem("Help");

	EditorImages = new GImageList(EditorSkin->SkinTexture, EditorSkin->Pack);



	MainPages.SetRectangle(0,0,100,100);
	MainPages.Dock = DCK_FILL;
	Engine.GUI.Desktop->AddChild(&MainPages);

	MainPages.AddPage(&StartPage,0,true);

	RTextView* tv = new RTextView();
	RTextDocument* doc = new RTextDocument();



	MainPages.AddPage(tv,doc,false);
	
	Engine.GUI.Desktop->Layout();
	Engine.GUI.Desktop->Layout();

	// Engine.GUI.Desktop->DebugMode = GDB_MOUSEON;
}