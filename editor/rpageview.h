#ifndef RPAGEVIEW_H
#define RPAGEVIEW_H

#include "gobject.h"
#include "gbuttonbase.h"
#include "gproxy.h"
#include "gsystembutton.h"

class RPage;
class RPageView;
class RDocumentView;
class RDocument;

class GPageCloseButton: public GSystemButton
{
public:
	RPage* Page;

	GPageCloseButton();

	void Clicked(int x, int y, int button);
};

/**
 * GTabButton like control for easily switching views and documents.
 */
class RPageButton: public GButtonBase
{
public:
	GPageCloseButton Closer;

	/**
	 * Page pointer attached to this button.
	 */
	RPage* Page;

	void Clicked(int x, int y, int button);

	RPageView* GetPageView();
	bool IsSelected();

	void Render();
	void Layout();

	void OnMouseExit();
};

/**
 * Opened tab in PageView.
 */
class RPage
{
public:
	RPage();

	RPageView*		PageViewer; // TODO: change this when tab is moved to another PageView!
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
	Array< RPage* >	Pages;
	IRectangle			ViewRectangle;
	RPage*				ActivePage;

	/**
	 * Add page to list.
	 * TODO: make these pointers as smart pointers. Since we can open same document in multiple views.
	 * Closing a page maybe problematic.
	 */
	RPage* AddPage( RDocumentView* view, RDocument* doc, bool activate = false );

	void ClosePage( RPage* page );

	void MovePage(RPage* page, RPageView* newView);

	void ActivatePage( RPage* page );

	void Render();
	void Update();
	void Layout();
	GObject* FindObject();
};


#endif