#ifndef RPAGEVIEW_H
#define RPAGEVIEW_H

#include "gobject.h"
#include "gbuttonbase.h"
#include "gproxy.h"

class RPage;
class RPageView;
class RDocumentView;
class RDocument;

/**
 * GTabButton like control for easily switching views and documents.
 */
class RPageButton: public GButtonBase
{
public:
	RPageButton();

	/**
	 * Page pointer attached to this button.
	 */
	RPage* Page;

	void Clicked(int x, int y, int button);

	RPageView* GetPageView();
	bool IsSelected();

	void Render();

	void MouseExit();
};

/**
 * Opened tab in PageView.
 */
class RPage
{
public:
	RPage();

	RPageButton		Button;
	RDocumentView*	View;
	RDocument*		Document;
};


/**
 * TabControl like control for organizing opened document tabs.
 */
class RPageView: public GObject
{
public:
	RPageView();

	GObject				PageButtons;
	TArray< RPage* >	Pages;
	IRectangle			ViewRectangle;
	RPage*				ActivePage;

	/**
	 * Add page to list.
	 * TODO: make these pointers as smart pointers. Since we can open same document in multiple views.
	 * Closing a page maybe problematic.
	 */
	RPage* AddPage( RDocumentView* view, RDocument* doc, bool activate = false );

	void ActivatePage( RPage* page );

	void Render();
	void Update();
	void Layout();
	GObject* FindObject();
};


#endif