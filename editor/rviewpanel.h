#ifndef RVIEWPANEL_H
#define RVIEWPANEL_H

#include "gobject.h"
#include "gbuttonbase.h"

class RViewPanel;
class RDocumentView;
class RDocument;

/**
 * GTabButton like control for easily switching views and documents.
 */
class RViewButton: public GButtonBase
{
public:
	RDocumentView* View;
	RDocument* Document;

	void Clicked(int x, int y, int button);

	RViewPanel* GetPanel();
	bool IsSelected();

	void Render();

	void MouseExit();
};

class RViewTabs: public GObject
{
public:

};

/**
 * Proxy GUI object.
 * That just forwards messages to activeView which is easily changeable view control.
 */
class RViewPanel: public GObject
{
public:
	RViewPanel();

	RDocumentView* ActiveView;

	GObject Pages;

	IRectangle ViewRectangle;

	//TArray< RViewButton* > Pages;

	void ActivatePage( RViewButton* button );

	/**
	 * Add page to list.
	 * TODO: make these pointers as smart pointers. Since we can open same document in multiple views.
	 * Closing a page maybe problematic.
	 */
	RViewButton* AddPage( RDocumentView* view, RDocument* doc, bool activate = false );

	/**
	 * Basically opening a document with a viewer.
	 * You can open a Terrain.RTR file with Terrain editor, or XML viewer YAY!
	 */
	void ActivateView(RDocumentView* view, RDocument* doc);

	void Render();
	void Update();
	void Layout();

	GObject* FindObject();
};


#endif