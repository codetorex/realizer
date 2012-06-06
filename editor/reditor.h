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

class GSchemedSkin;
class GImageList;
class VTexture;

class REditorResources
{
public:
	VTexture* RealizerLogo;
	VTexture* StartPageTexture;

};

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

	GImageList* ProjectViewImages;
	GToolStrip ProjectToolbar;
	GTreeView ProjectView;

	RProject* Project;

	void NewProject();

	/// Monospaced font that is good for console and debug stuff
	GFont* DebugFont;

	GConsole* DebugConsole;

	void LoadResources();

	void InitializeMainGui();

	void Render();

	void ActivateConsole(bool value);

	void NewItem_Click();
};

extern REditor Editor;

#endif