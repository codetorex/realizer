#ifndef REDITOR_H
#define REDITOR_H

#include "vscene.h"
#include "genums.h"
#include "gconsole.h"

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


	GSkin* EditorSkin;

	/// Monospaced font that is good for console and debug stuff
	GFont* DebugFont;

	GConsole* DebugConsole;

	void LoadResources();

	void Render();

	void ActivateConsole(bool value);
};

extern REditor Editor;

#endif