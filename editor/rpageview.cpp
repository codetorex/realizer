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
	Button.Closer.Page = this;
	Button.TextAlign = CA_MiddleLeft;
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
	if (fd != this)
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
	GSchemedSkin* ss = (GSchemedSkin*)Skin;

	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(DrawRegion, ss->Colors.ButtonFace);

	Engine.Draw.SetTexture(ss->SkinTexture);
	
	IRegion borderRegion;
	borderRegion.SetRegion(DrawRegion.X()+ViewRectangle.X,DrawRegion.Y() + ViewRectangle.Y - 1,ViewRectangle.Width,ViewRectangle.Height);
	ss->TabBorder.Render(borderRegion);

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
	page->View->SetDocument( page->Document );
	ActivePage->Button.SetGraphic(GBG_DOWN);
}


RPage* RPageView::AddPage( RDocumentView* view, RDocument* doc , bool activate )
{
	RPage* pg = new RPage();
	pg->View = view;
	pg->Document = doc;
	pg->PageViewer = this;
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
	pg->Button.Padding.SetPadding(5,3,4,4);
	pg->Button.SetRectangle(0,0,10,10);
	PageButtons.AddChild(&pg->Button);
	pg->Button.Layout();
	Layout();

	Pages.Add(pg);

	if (activate)
	{
		ActivatePage(pg);
	}

	return pg;
}

void RPageView::MovePage( RPage* page, RPageView* newView )
{
	throw NotImplementedException();
}

void RPageView::ClosePage( RPage* page )
{
	PageButtons.RemoveChild(&page->Button);
	Pages.Remove(page);

	if (ActivePage == page)
	{
		if (Pages.Count > 0)
		{
			ActivatePage(Pages.Item[0]);
		}
		else
		{
			ActivePage = 0;
		}
	}

	/*
	 * TODO:
	 *
	if (page->Document->Views.Count < 0)
	{
		delete it
	}*/


	Layout();
}

RPageView* RPageButton::GetPageView()
{
	GObject* p = (GObject*)Parent;
	RPageView* rp = (RPageView*)p->Parent;
	return rp;
}

void RPageButton::Render()
{
	if (Page->Document)
	{
		if (Page->Document->Changed)
		{
			ForeColor = TColors::Red;
		}
		else
		{
			ForeColor = TColors::Black;
		}
	}
	Skin->RenderTabButton(this,IsSelected());
	GObject::Render();
}

void RPageButton::Clicked( int x, int y, int button )
{
	RPageView* pnl = GetPageView();
	pnl->ActivatePage(Page);
}

void RPageButton::OnMouseExit()
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

void RPageButton::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&Closer);
	}
	this->GObject::Layout();

	OwnObject(&Closer);
	Closer.Layout();

	TCharacterEnumerator ce(Text); // TODO: fix this
	int stringWidth = Font->GetStringWidth(ce);
	SetContentWidth(stringWidth + 4 + Closer.Width);
	SetContentHeight( MathDriver::Max(Closer.Height,Font->Height) );

	UpdateContent();

	Closer.SetVector(Content.Width-Closer.Width, (Content.Height - Closer.Height) / 2);
}

GPageCloseButton::GPageCloseButton()
{
	ButtonType = GSystemButton::BT_TOOLWINDOW_CLOSE;
}

void GPageCloseButton::Clicked( int x, int y, int button )
{
	Page->PageViewer->ClosePage(Page);
}