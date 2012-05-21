#ifndef REDITOR_H
#define REDITOR_H

#include "vscene.h"
#include "genums.h"
#include "gconsole.h"
#include "rviewpanel.h"
#include "rstartpage.h"

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
	}


	GSchemedSkin* EditorSkin;

	GImageList* EditorImages;

	RViewPanel MainPanel;

	RStartPage StartPage;

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