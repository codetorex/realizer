#include "stdafx.h"
#include "rpageview.h"
#include "rdocument.h"
#include "rdocumentview.h"
#include "cengine.h"

#include "gfont.h"
#include "gschemedskin.h"


RPage::RPage()
{
	Button.Page = this;
}

RPageView::RPageView()
{
	ClassID = GOBJECT_CLASSID; // TODO: make an extensible class definition stuff
	ActivePage = 0;
	PageButtons.Layouter = &GLayoutHorizontalOverflow::Instance;
	PageButtons.Dock = DCK_TOP;
}

GObject* RPageView::FindObject()
{
	GObject* fd = GObject::FindObject();

	if (fd)
	{
		return fd;
	}


	if (ActivePage)
	{
		return ActivePage->View->FindObject();
	}

	return this;
}

void RPageView::Render()
{
	Engine.Draw.NoTexture();
	GSchemedSkin* ss = (GSchemedSkin*)Skin;
	Engine.Draw.FillRectangle(DrawRegion, ss->Colors.ButtonFace);

	GObject::Render(); // render childs

	if (ActivePage)
	{
		ActivePage->View->Render();
	}
}

void RPageView::Update()
{
	GObject::Update();
	if (ActivePage)
	{
		ActivePage->View->Update();
	}
}

void RPageView::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&PageButtons);
	}

	OwnObject(&PageButtons);

	TLinkedListEnumerator< GObject* > tc(&PageButtons);
	while(tc.MoveNext())
	{
		GObject* c = tc.Current;
		PageButtons.OwnObject(c);
	}

	UpdateContent();
	ViewRectangle = Layouter->Layout(this,false);


	if (ActivePage)
	{
		ActivePage->View->SetRectangle(ViewRectangle);
		ActivePage->View->Layout();
	}
}

void RPageView::ActivatePage( RPage* page )
{
	if (ActivePage)
	{
		ActivePage->Button.SetGraphic(GBG_NORMAL);
	}

	ActivePage = page;
	if (page->View->Width != ViewRectangle.Width || page->View->Height != ViewRectangle.Height)
	{
		OwnObject(page->View);
		page->View->SetRectangle(ViewRectangle);
		page->View->Layout();
	}
	ActivePage->Button.SetGraphic(GBG_DOWN);
}


RPage* RPageView::AddPage( RDocumentView* view, RDocument* doc , bool activate )
{
	RPage* pg = new RPage();
	pg->View = view;
	pg->Document = doc;
	if (doc == 0)
	{
		pg->Button.Text = view->Name;
	}
	else
	{
		// TODO: count doc refs, if there is more than one user append editor name to text
		pg->Button.Text = doc->Name;
	}

	TCharacterEnumerator sb(pg->Button.Text);
	int nw = Font->GetStringWidth(sb)+ 10;
	int nh = Font->Height + 8;
	pg->Button.SetRectangle(0,0,nw,nh);

	PageButtons.AddChild(&pg->Button);
	Layout();

	if (activate)
	{
		ActivatePage(pg);
	}

	return pg;
}


RPageButton::RPageButton()
{
	// TODO: add ToolWindow close button here
}

RPageView* RPageButton::GetPageView()
{
	GObject* p = (GObject*)Parent;
	RPageView* rp = (RPageView*)p->Parent;
	return rp;
}

void RPageButton::Render()
{
	Skin->RenderTabButton(this,IsSelected());
	GObject::Render();
}

void RPageButton::Clicked( int x, int y, int button )
{
	RPageView* pnl = GetPageView();
	pnl->ActivatePage(Page);
}

void RPageButton::MouseExit()
{
	if (IsSelected())
	{
		SetGraphic(GBG_DOWN);
	}
	else
	{
		SetGraphic(GBG_NORMAL);
	}
}

bool RPageButton::IsSelected()
{
	RPageView* panel = GetPageView();
	if (panel->ActivePage == Page)
	{
		return true;
	}
	return false;
}

