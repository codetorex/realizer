#include "stdafx.h"
#include "gdropdown.h"
#include "vgui.h"

GDropDown::GDropDown()
{
	ClassID = GDROPDOWN_CLASSID;
	Visible = false;
}

void GDropDown::Render()
{
	Skin->RenderDropDown(this);
	this->GObject::Render();
}

void GDropDown::Layout()
{
	SetWidth(100);
	SetHeight(100);
	Skin->LayoutDropDown(this);

	int reqWidth = 0;
	int reqHeight = 0;

	TLinkedListEnumerator< GObject* > c(this);
	while(c.MoveNext())
	{
		GObject* cur = c.Current;
		cur->Layout();
		if (reqWidth < cur->Width)
		{
			reqWidth = cur->Width;
		}

		cur->SetTop(reqHeight+1);
		reqHeight += cur->Height+2;
	}

	c.Reset();
	while(c.MoveNext())
	{
		GObject* cur = c.Current;
		cur->SetWidth(reqWidth);
	}

	SetWidth( reqWidth + (Width - ObjectRegion.Width) );
	SetHeight(reqHeight + (Height - ObjectRegion.Height));
	Skin->LayoutDropDown(this);
}


void GDropDown::Show()
{
	Show(Master->X,Master->Y);
}

void GDropDown::Show( int x,int y )
{
	SetLeft(x);
	SetTop(y);

	if (Visible)
	{
		return;
	}

	LastFocus = Master->Focused;
	Master->Desktop->SetTopObject(this);
	Visible = true;
}

void GDropDown::Hide()
{
	Master->Desktop->UnsetTopObject(this);
	Visible = false;
}

void GDropDown::Update()
{
	if (Master->Focused != LastFocus)
	{
		Hide();
	}
	else
	{
		this->GObject::Update();
	}
}