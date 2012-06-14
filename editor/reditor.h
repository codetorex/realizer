#ifndef REDITOR_H
#define REDITOR_H

#include "vscene.h"
#include "genums.h"
#include "gconsole.h"
#include "rpageview.h"
#include "rstartpage.h"
#include "reditorskin.h"
#include "rproject.h"
#include "gtreeview.h"
#include "gsplitcontainer.h"
#include "gtreeview.h"
#include "gimagelist.h"
#include "gtoolstrip.h"

#include "reditorcommands.h"

class GSchemedSkin;
class GImageList;
class VTexture;

class REditorResources
{
public:
	VTexture* RealizerLogo;
	VTexture* StartPageTexture;

	GImageList* EditorImages;

	GImage* NewProjectLargeIcon;
	GImage* ProjectLargeIcon;
	GImage* OpenProjectLargeIcon;

	GImage* NewProjectIcon;
	GImage* OpenProjectIcon;
	GImage* SaveProjectIcon;

	GImage* NewItemIcon;
	GImage* ExistingItemIcon;
	GImage* NewFolderIcon;
	GImage* RenameIcon;
	GImage* ImportFolderIcon;

	GImage* PVProject;
	GImage* PVFolder;
	GImage* PVLiveFolder;
	GImage* PVDocument;
	GImage* PVImage;

	GImage* UICanvas;
	GImage* UIWindow;
	GImage* UIButton;
	GImage* UIUnknown;



	void SetToolboxIcon(GUIClassID clsID, GImage* img);
	GImage* Fugue(const TString& fugueName);

	void Initialize();
};

class REditorMenu
{
public:
	GMenuStrip* MainMenu;

	GMenuItem* File;
	GMenuItem* Edit;
	GMenuItem* View;
	GMenuItem* Project;
	GMenuItem* Debug;
	GMenuItem* Tools;
	GMenuItem* Help;

	void Initialize();
};

extern REditorMenu EditorMenu;

extern REditorResources Resources;

class REditor: public VScene
{
private:
	GObject* OldFocused;
	bool DebugInputEnabled;

public:
	REditor()
	{
		Flags.Set( SF_UPDATE | SF_RENDER | SF_ACTIVE | SF_ALWAYSONTOP );
		Project = 0;
	}

	REditorSkin* Skin;

	GImageList* EditorImages;

	RPageView MainPages; // TODO: make this splittable by attaching split container?

	RStartPage StartPage;

	GSplitContainer ProjectEditorSplit;

	GToolStrip ProjectToolbar;
	GTreeView ProjectView;

	RProject* Project;

	/// Monospaced font that is good for console and debug stuff
	GFont* DebugFont;

	GConsole* DebugConsole;

	void LoadResources();

	void InitializeMainGui();

	void Render();

	void ActivateConsole(bool value);

	
};

extern REditor Editor;

#endif