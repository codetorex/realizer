#include "stdafx.h"
#include "gmenuitem.h"
#include "vgui.h"
#include "glayout.h"
#include "gfont.h"

void GMenuItem::Render()
{
	if (IsParentDropDown())
	{
		Skin->RenderMenuItem(this);	

		if (!Seperator)
		{
			Font->Render(Text,TextX + ScreenRegion.X,TextY + ScreenRegion.Y, ForeColor);
		}

		if (Image.Visible)
		{
			Image.Render();
		}
	}
	else // we are on menu strip
	{
		Skin->RenderMenuStripItem(this);

		Font->Render(Text,TextX + ScreenRegion.X,TextY + ScreenRegion.Y, ForeColor);
	}
}

GMenuItem::GMenuItem()
{
	ClassID = GMENUITEM_CLASSID;
	GraphicState = 0;
	Seperator = false;
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

	// bir timer calistir 300 yada 400 ms lik
	// timer dolunca dropdown u varsa showlasin amk
}

void GMenuItem::MouseExit()
{
	if (!Seperator)
	{
		GraphicState = 0;
	}
}

void GMenuItem::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		Click.call();
	}
}

void GMenuItem::Update()
{
	this->GObject::Update();
	Image.Update();
}

void GMenuItem::Layout()
{
	TCharacterEnumerator c(Text);
	int strWidth = Font->GetStringWidth(c); 

	if (IsParentDropDown())
	{
		TextX = 16 + 8; // 4px margin 16 px for icon
		TextY = 3;
		SetWidth( strWidth + 16 + TextX );
		SetHeight( Font->Height + 10);
		Image.SetLeftTop(2,(Height - Image.Height)/2);
	}
	else
	{
		GObject* p = (GObject*)Parent;
		SetHeight(Font->Height + 8);
		SetWidth(strWidth+12);
		// text is on middle
		TextX = (Width - strWidth) / 2;
		TextY = (Height - Font->Height) / 2;
		Skin->LayoutMenuStripItem(this);
	}

	ObjectRegion.SetRectangle(0,0,Width,Height);
	Image.SetParent(this);
}

GMenuItem* GMenuItem::AddSeperator()
{
	GMenuItem* r = new GMenuItem();
	r->SetSize(0,0,20,10);
	r->set_Seperator(true);
	AddChild(r);
	return r;
}