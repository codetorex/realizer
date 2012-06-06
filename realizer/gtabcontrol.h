#ifndef GTABCONTROL_H
#define GTABCONTROL_H

#include "gobject.h"
#include "gbuttonbase.h"
#include "glayout.h"

class GTabPage;
class GTabControl;



class GTabButton: public GButtonBase
{
public:
	/// Associated page with this button
	GTabPage* Page;

	GTabControl* GetTabControl();

	bool IsSelectedPage();

	void OnMouseExit();
	void Clicked(int x, int y, int button);

	void Render();
	
	bool Active;
};

class GTabPage: public GObject
{
public:
	//GLayout* Layouter;

	GTabPage();

	GTabButton TabButton;
};

enum GTabAlignment
{
	GTB_TOP,
	GTB_LEFT,
	GTB_RIGHT,
	GTB_BOTTOM
};

class GTabControl: public GObject
{
public:
	GObject TabPageButtons;
	IRectangle PageArea;

	GTabControl();

	GTabAlignment Alignment;

	TArray< GTabPage* > TabPages;

	GTabPage* CurrentPage;

	GTabPage* AddPage(const TString& pageName);
	void RemovePage(GTabPage* page);

	void SelectPage(int index);
	void SelectPage(GTabPage* page);


	void Layout();
	void Render();
	void Update();

	GObject* FindObject();
};

#endif