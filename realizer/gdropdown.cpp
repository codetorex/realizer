#include "stdafx.h"
#include "gdropdown.h"
#include "vgui.h"
#include "cengine.h"
#include "gmenuitem.h"

GDropDown::GDropDown()
{
	ClassID = GDROPDOWN_CLASSID;
	Visible = false;
	SubMenu = 0;
	ParentMenu = 0;
}

void GDropDown::Render()
{
	Skin->RenderDropDown(this);
	this->GObject::Render();
}

void GDropDown::Layout()
{
	ChangeWidth(100);
	ChangeHeight(100);
	Skin->LayoutDropDown(this);

	int reqWidth = 0;
	int reqHeight = 0;

	TLinkedListEnumerator< GObject* > c(this);
	while(c.MoveNext())
	{
		GObject* cur = c.Current;
		OwnObject(cur); // update skin font etc
		cur->Layout();
		if (reqWidth < cur->Width)
		{
			reqWidth = cur->Width;
		}

		cur->MoveY(reqHeight+1);
		reqHeight += cur->Height+2;
	}

	c.Reset();
	while(c.MoveNext())
	{
		GObject* cur = c.Current;
		cur->ChangeWidth(reqWidth);
	}

	ChangeWidth( reqWidth + (Width - Content.Width) );
	ChangeHeight(reqHeight + (Height - Content.Height));
	Skin->LayoutDropDown(this);
}


void GDropDown::Show()
{
	Show(Master->X,Master->Y);
}

void GDropDown::Show( int x,int y )
{
	Move(x,y);

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
	if (SubMenu)
	{
		SubMenu->Hide(); // recursively hides all childs
	}
}

void GDropDown::Update()
{
	if (Master->Focused != LastFocus)
	{
		if (!IsRelativeObject(Master->Focused))
		{
			Hide(); // this is dangerous
		}
	}
	else
	{
		this->GObject::Update();
	}
}

bool GDropDown::IsRelativeObject( GObject* obj )
{
	if (obj->ClassID == GDROPDOWN_CLASSID)
	{
		return IsRelative((GDropDown*)obj);
	}

	GObject* p = (GObject*)obj->Parent;
	if (p)
	{
		if (p->ClassID == GDROPDOWN_CLASSID)
		{
			return IsRelative((GDropDown*)p);
		}
	}

	return false;
}

bool GDropDown::IsRelative( GDropDown* menu )
{
	/// Check childs
	GDropDown* c = SubMenu;
	while(c)
	{
		if (c == menu)
		{
			return true;
		}
		c = c->SubMenu;
	}

	// Check Parents
	GDropDown* p = ParentMenu;
	while(p)
	{
		if (p == menu)
		{
			return true;
		}
		p = p->ParentMenu;
	}

	// Check self
	if (menu == this)
	{
		return true;
	}

	// Has no relative, sad
	return false;
}

void GDropDown::SetSubMenu( GDropDown* pSubMenu )
{
	if (SubMenu == pSubMenu)
	{
		return;
	}

	if (SubMenu)
	{
		SubMenu->Hide();
		SubMenu->ParentMenu = 0;
	}
	pSubMenu->ParentMenu = this;
	SubMenu = pSubMenu;
}

GDropDown* GDropDown::GetRootMenu()
{
	GDropDown* p = this;
	while(p)
	{
		if (p->ParentMenu == 0)
		{
			return p;
		}
		p = p->ParentMenu;
	}
}

void GDropDown::HideSubMenu()
{
	if (!SubMenu)
		return;

	SubMenu->Hide();
	SubMenu->ParentMenu = 0;
	SubMenu = 0;
}