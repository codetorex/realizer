#include "stdafx.h"
#include "gmenuitem.h"
#include "vgui.h"
#include "glayout.h"
#include "gfont.h"

void GDropDownTimer::Activate()
{
	if (Parent->SubItems.ItemCount == 0)
	{
		Parent->HideParentMenu();
	}
	else
	{
		if (!Parent->SubItems.Visible)
		{
			Parent->ShowSubMenu();
		}
	}
}

void GMenuItem::Render()
{
	const IRectangle& dRect = DrawRegion;

	if (IsParentDropDown())
	{
		Skin->RenderMenuItem(this);	

		if (!Seperator)
		{
			Font->Render(Text,TextX + dRect.X,TextY + dRect.Y, ForeColor);
		}

		if (Image.Visible)
		{
			Image.Render();
		}
	}
	else // we are on menu strip
	{
		Skin->RenderMenuStripItem(this);

		Font->Render(Text,TextX + dRect.X,TextY + dRect.Y, ForeColor);
	}
}

GMenuItem::GMenuItem()
{
	ClassID = GMENUITEM_CLASSID;
	GraphicState = 0;
	Seperator = false;

	ShowTimer.Initialize(this,400);
}

void GMenuItem::MouseEnter()
{
	if (!IsParentDropDown())
		return;
	
	if (!ShowTimer.Enabled)
	{
		ShowTimer.Start();
	}
}

void GMenuItem::MouseMove( int x,int y )
{
	if (!Seperator)
	{
		GraphicState = 3;
	}

	if (IsParentMenuStrip())
	{
		if (Master->Focused == this)
		{
			if (Master->ButtonState[0] && MouseInside)
			{
				GraphicState = 1;
			}
			else
			{
				GraphicState = 3;
			}
		}
		else
		{
			GraphicState = 3;
		}
	}
}

void GMenuItem::MouseExit()
{
	if (!Seperator)
	{
		GraphicState = 0;
	}

	ShowTimer.Stop();
}

void GMenuItem::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		if (IsParentMenuStrip())
		{
			if (SubItems.ItemCount > 0)
			{
				SubItems.Show(DrawRegion.X(),DrawRegion.Bottom());
			}
		}
		else
		{
			if (SubItems.ItemCount > 0)
			{
				if (!SubItems.Visible)
				{
					ShowTimer.Stop();
					ShowSubMenu();
				}
			}
			else
			{
				HideRelativeMenus();
				Click.call();
			}
		}
	}
}

void GMenuItem::Update()
{
	this->GObject::Update();
	Image.Update();
	ShowTimer.UpdateTimeEffect();
}

void GMenuItem::Layout()
{
	TCharacterEnumerator c(Text);
	int strWidth = Font->GetStringWidth(c); 

	if (IsParentDropDown())
	{
		TextX = 16 + 8; // 4px margin 16 px for icon
		TextY = 3;
		ChangeWidth( strWidth + 16 + TextX );
		ChangeHeight( Font->Height + 10);
		Image.Move(2,(Height - Image.Height)/2);
	}
	else
	{
		GObject* p = (GObject*)Parent;
		ChangeHeight(Font->Height + 8);
		ChangeWidth(strWidth+12);
		// text is on middle
		TextX = (Width - strWidth) / 2;
		TextY = (Height - Font->Height) / 2;
		Skin->LayoutMenuStripItem(this);
	}

	Content.SetRectangle(0,0,Width,Height);
	Image.SetParent(this);

	OwnObject(&SubItems);
	SubItems.Layout();
}

GMenuItem* GMenuItem::AddSeperator()
{
	GMenuItem* r = new GMenuItem();
	r->SetRectangle(0,0,20,10);
	r->set_Seperator(true);
	AddChild(r);
	return r;
}

void GMenuItem::ShowSubMenu()
{
	SubItems.Show(DrawRegion.Right(),DrawRegion.Top());
	GDropDown* p = (GDropDown*)Parent;
	p->SetSubMenu(&SubItems);
}

void GMenuItem::HideRelativeMenus()
{
	if (IsParentDropDown())
	{
		GDropDown* p = (GDropDown*)Parent;
		GDropDown* root = p->GetRootMenu();
		root->Hide();
	}
}

void GMenuItem::HideParentMenu()
{
	if (IsParentDropDown())
	{
		GDropDown* p = (GDropDown*)Parent;
		p->SetSubMenu(&SubItems);
	}
}

