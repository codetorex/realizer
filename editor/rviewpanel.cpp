#include "stdafx.h"
#include "rviewpanel.h"
#include "rdocument.h"
#include "rdocumentview.h"
#include "cengine.h"

#include "gfont.h"
#include "gschemedskin.h"


RViewPanel::RViewPanel()
{
	ClassID = GOBJECT_CLASSID; // TODO: make an extensible class definition stuff
	ActiveView = 0;
	Pages.Layouter = &GLayoutHorizontalOverflow::Instance;
	Pages.Dock = DCK_TOP;
}

GObject* RViewPanel::FindObject()
{
	GObject* fd = GObject::FindObject();

	if (fd)
	{
		return fd;
	}


	if (ActiveView)
	{
		return ActiveView->FindObject();
	}

	return this;
}

void RViewPanel::Render()
{
	Engine.Draw.NoTexture();
	GSchemedSkin* ss = (GSchemedSkin*)Skin;
	Engine.Draw.FillRectangle(DrawRegion, ss->Colors.ButtonFace);

	GObject::Render(); // render childs

	if (ActiveView)
	{
		ActiveView->Render();
	}
}

void RViewPanel::Update()
{
	GObject::Update();
	if (ActiveView)
	{
		ActiveView->Update();
	}
}

void RViewPanel::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&Pages);
	}

	OwnObject(&Pages);

	TLinkedListEnumerator< GObject* > tc(&Pages);
	while(tc.MoveNext())
	{
		GObject* c = tc.Current;
		Pages.OwnObject(c);
	}

	UpdateContent();
	ViewRectangle = Layouter->Layout(this,false);


	if (ActiveView)
	{
		ActiveView->SetRectangle(ViewRectangle);
		ActiveView->Layout();
	}
}

void RViewPanel::ActivateView( RDocumentView* view, RDocument* doc )
{
	ActiveView = view;
	ActiveView->Document = doc;
	if (view->Width != Width || view->Height != Height)
	{
		ActiveView->SetRectangle(ViewRectangle);
		ActiveView->Layout();

		OwnObject(ActiveView);
	}
}

RViewButton* RViewPanel::AddPage( RDocumentView* view, RDocument* doc , bool activate )
{
	RViewButton* btn = new RViewButton();
	btn->Document = doc;
	if (doc == 0)
	{
		btn->Text = view->Name;
	}
	else
	{
		btn->Text = doc->Name;
	}
	TCharacterEnumerator sb(btn->Text);
	int nw = Font->GetStringWidth(sb)+ 10;
	int nh = Font->Height + 8;

	btn->SetRectangle(0,0,nw,nh);
	btn->View = view;

	Pages.Add(btn);
	Layout();

	ActivateView(view,doc);
	return btn;
}

void RViewPanel::ActivatePage( RViewButton* button )
{

}

RViewPanel* RViewButton::GetPanel()
{
	GObject* p = (GObject*)Parent;
	RViewPanel* rp = (RViewPanel*)p->Parent;
	return rp;
}

void RViewButton::Render()
{
	Skin->RenderTabButton(this,IsSelected());
	GObject::Render();
}

void RViewButton::Clicked( int x, int y, int button )
{
	RViewPanel* pnl = GetPanel();
	pnl->ActivateView(View,Document);
}

void RViewButton::MouseExit()
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

bool RViewButton::IsSelected()
{
	RViewPanel* panel = GetPanel();
	if (panel->ActiveView == View)
	{
		if (View->Document == Document)
		{
			return true;
		}
	}
	return false;
}