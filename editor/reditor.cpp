#include "stdafx.h"
#include "reditor.h"
#include "cengine.h"
#include "gwireframeskin.h"
#include "gfont.h"
#include "gschemedskin.h"
#include "gconsole.h"
#include "gcomponents.h"

#include "rguiview.h"
#include "rtextview.h"
#include "rtextdocument.h"
#include "vtexture.h"
#include "tbitmapcodecs.h"


REditor Editor;
REditorMenu EditorMenu;
REditorEvents EditorEvents;
REditorCommands EditorCommands;
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

void REditorResources::Initialize()
{
	EditorImages = new GImageList(Editor.Skin->SkinTexture, Editor.Skin->Pack);


	RealizerLogo = Engine.Textures.LoadTexture("realizer.png");
	StartPageTexture = Engine.Textures.LoadTexture("editor/start-page.png");

	/// TODO: load these paths from somewhere else
	NewProjectLargeIcon  = EditorImages->AddImage("editor/newproject.png");
	ProjectLargeIcon     = EditorImages->AddImage("editor/project.png");
	OpenProjectLargeIcon = Fugue("32/folder-horizontal.png");
	NewItemIcon          = Fugue("blue-document--plus.png" );
	ExistingItemIcon     = Fugue("blue-document--arrow.png");
	NewFolderIcon        = Fugue("folder--plus.png");
	RenameIcon           = Fugue("pencil-field.png");
	ImportFolderIcon	 = Fugue("folder-import.png");

	NewProjectIcon       = Fugue("block--plus.png");
	OpenProjectIcon      = Fugue("folder-horizontal-open.png");
	SaveProjectIcon      = Fugue("disk.png");

	PVProject			 = Fugue("block.png");
	PVFolder			 = Fugue("blue-folder.png");
	PVLiveFolder		 = Fugue("folder.png");
	PVDocument			 = Fugue("document.png");
	PVImage				 = Fugue("image.png");
}

GImage* REditorResources::Fugue( const TString& fugueName )
{
	return EditorImages->AddImage("icons/fugue/" + fugueName);
}

void REditorMenu::Initialize()
{
	MainMenu = new GMenuStrip();
	MainMenu->SetRectangle(0,0,100,20);
	MainMenu->Dock = DCK_TOP;
	Engine.GUI.Desktop->AddChild(MainMenu);

	File    = MainMenu->AddItem("File");
	Edit    = MainMenu->AddItem("Edit");
	View    = MainMenu->AddItem("View");
	Project = MainMenu->AddItem("Project");
	Debug   = MainMenu->AddItem("Debug");
	Tools   = MainMenu->AddItem("Tools");
	Help    = MainMenu->AddItem("Help");

	File->AddSubMenu("New Project",*Resources.NewProjectIcon, EditorEvents.NewProject);
	File->AddSubMenu("Open Project",*Resources.OpenProjectIcon);
	File->AddSubMenu("Save Project",*Resources.SaveProjectIcon, EditorEvents.SaveProject);
	File->Layout();

	Project->AddSubMenu("Add New Item",*Resources.NewItemIcon, EditorEvents.AddNewItem);
	Project->AddSubMenu("Add Existing Item",*Resources.ExistingItemIcon, EditorEvents.AddExistingItem);
	Project->AddSubMenu("Add Folder",*Resources.NewFolderIcon, EditorEvents.AddNewFolder);
	Project->AddSubMenu("Rename", *Resources.RenameIcon,EditorEvents.Rename);
	Project->AddSubMenu("Import Live Folder", *Resources.ImportFolderIcon, EditorEvents.ImportFolder);
	Project->Layout();
}

void REditor::InitializeMainGui()
{
	Resources.Initialize();
	EditorMenu.Initialize();

	RProjectSourceTypeManager::Instance.InitializeDefaultTypes();

	StartPage.NewProjectButton.Image.SetImage(*Resources.NewProjectLargeIcon);
	StartPage.NewProjectButton.Margin.SetPadding(4,2);
	StartPage.OpenProjectButton.Image.SetImage(*Resources.OpenProjectLargeIcon);
	StartPage.OpenProjectButton.Margin.SetPadding(4,2);

	ProjectEditorSplit.SetRectangle(0,0,100,100);
	ProjectEditorSplit.Dock = DCK_FILL;
	Engine.GUI.Desktop->AddChild(&ProjectEditorSplit);

	MainPages.SetRectangle(0,0,100,100);
	MainPages.Dock = DCK_FILL;
	//Engine.GUI.Desktop->AddChild(&MainPages);
	ProjectEditorSplit.Panel1.AddChild(&MainPages);

	MainPages.AddPage(&StartPage,0,true);

	RTextViewStyle* defStyle = new RTextViewStyle();
	TStream* styleStream = Engine.FileSystem.Open("editor/styles/vs2010/soft-metro.htm", fm_Read);
	RTextViewStyleVS2010Loader::Instance.LoadStyle(defStyle,styleStream);
	defStyle->Font = Engine.GUI.Fonts.GetFont("Dina",12);
	RTextView::DefaultStyle = defStyle;
	



	ProjectToolbar.SetRectangle(0,0,100,100);
	ProjectToolbar.Dock = DCK_TOP;
	ProjectEditorSplit.Panel2.AddChild(&ProjectToolbar);

	ProjectToolbar.AddButton("Add New Item", *Resources.NewItemIcon, EditorEvents.AddNewItem);
	ProjectToolbar.AddButton("Add Existing Item", *Resources.ExistingItemIcon, EditorEvents.AddExistingItem);
	ProjectToolbar.AddButton("Add New Folder", *Resources.NewFolderIcon, EditorEvents.AddNewFolder);
	ProjectToolbar.AddButton("Rename",*Resources.RenameIcon, EditorEvents.Rename);
	ProjectToolbar.AddButton("Import Folder",*Resources.ImportFolderIcon, EditorEvents.ImportFolder);

	ProjectView.SetRectangle(0,0,100,100);
	ProjectView.Dock = DCK_FILL;
	ProjectView.ShowRoot = true;
	ProjectView.ShowLines = true;
	ProjectView.ShowPlusMinus = true;
	ProjectEditorSplit.SplitterDistance = (int)((float)Engine.Renderer.vWidth * 0.85f);
	ProjectEditorSplit.Panel2.AddChild(&ProjectView);

	RTextView* tv = new RTextView();
	RTextDocument* doc = new RTextDocument();
	doc->FilePath = "C:/Library/OldProjects/oldrealizercode/vbitmap.cpp";
	doc->LoadDocument();

	MainPages.AddPage(tv,doc,false);

	RGUIView* gv = new RGUIView();
	MainPages.AddPage(gv,0,false);
	
	Engine.GUI.Desktop->Layout();
	Engine.GUI.Desktop->Layout();

	// Engine.GUI.Desktop->DebugMode = GDB_MOUSEON;

#ifdef _DEBUG
	// DEBUG PURPOSES ONLY
	TStream* fs = Engine.FileSystem.Open("outputEDITOR.png",fm_Write);
	Skin->SkinTexture->bitmap->Save(fs,TBitmapCodecs::Png);
	fs->Close();
#endif // _DEBUG
}


