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
#include "vtexture.h"
#include "tbitmapcodecs.h"


REditor Editor;

REditorResources Resources;


void REditorSkin::LoadEditorSkin()
{
	TStream* fs = Engine.FileSystem.Open("Acrylic 7/Acrylic 7.uis",fm_Read);
	if (fs == NULL)
	{
		throw Exception("File not found");
	}

	int textureSize = 1024;
	Builder.Begin(textureSize,textureSize,true,this); // maybe needed to change?
	Builder.LoadFromScheme(fs);
	
	StartPageButton.SetAll(*Builder.SkinBitmap, TransparentPart, IPadding(2));
	VTexturePart* HoverPart = Builder.LoadInsertImage("editor/startpageover.png"); // TODO: these shits are getting nasty implement CSS like stuff!
	StartPageButton.Over.Initialize(*Builder.SkinBitmap,*HoverPart);

	Builder.Finish();

#ifdef _DEBUG
	// DEBUG PURPOSES ONLY
	fs = Engine.FileSystem.Open("outputEDITOR.png",fm_Write);
	SkinTexture->bitmap->Save(fs,TBitmapCodecs::Png);
	fs->Close();
#endif // _DEBUG
}

void REditor::LoadResources()
{
	//try
	{
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write );
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		DebugFont = Engine.GUI.Fonts.GetFont("Bitstream Vera Sans Mono",14);

		//Skin = (REditorSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");
		Skin = new REditorSkin();
		Skin->LoadEditorSkin();

		Engine.GUI.EnableGUI(Skin);


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
	/*catch( Exception& e )
	{
		TStringBuilderStack<1024> sb;
		sb.AppendLine("Error when loading most basic stuff:");
		sb.Append(e.Message);

		TWinTools::ShowMessage(sb.ToString());
	}*/
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
	EditorImages = new GImageList(Skin->SkinTexture, Skin->Pack);
	EditorImages->AddImage("editor/newproject.png");
	EditorImages->AddImage("editor/project.png");
	EditorImages->AddImage("icons/fugue/32/blue-folder-horizontal.png");

	StartPage.NewProjectButton.Image.SetImage(EditorImages->GetImage(0));
	StartPage.NewProjectButton.Margin.SetPadding(4,2);
	StartPage.OpenProjectButton.Image.SetImage(EditorImages->GetImage(2));
	StartPage.OpenProjectButton.Margin.SetPadding(4,2);

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

	MainPages.SetRectangle(0,0,100,100);
	MainPages.Dock = DCK_FILL;
	Engine.GUI.Desktop->AddChild(&MainPages);

	MainPages.AddPage(&StartPage,0,true);




	RTextViewStyle* defStyle = new RTextViewStyle();
	TStream* styleStream = Engine.FileSystem.Open("editor/styles/vs2010/soft-metro.htm", fm_Read);
	RTextViewStyleVS2010Loader::Instance.LoadStyle(defStyle,styleStream);
	defStyle->Font = Engine.GUI.Fonts.GetFont("Dina",12);
	RTextView::DefaultStyle = defStyle;
	
	RTextView* tv = new RTextView();
	RTextDocument* doc = new RTextDocument();
	doc->FilePath = "C:/Library/OldProjects/oldrealizercode/vbitmap.cpp";
	doc->LoadDocument();

	

	MainPages.AddPage(tv,doc,false);
	
	Engine.GUI.Desktop->Layout();
	Engine.GUI.Desktop->Layout();

	// Engine.GUI.Desktop->DebugMode = GDB_MOUSEON;
}