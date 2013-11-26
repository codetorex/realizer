#include "stdafx.h"
#include "gmenustrip.h"
#include "vgui.h"
#include "glayout.h"
#include "gfont.h"
#include "gmenuitem.h"

GMenuStrip::GMenuStrip()
{
	ClassID = GMENUSTRIP_CLASSID;
	Layouter = &GLayoutHorizontalOverflow::Instance;
}

void GMenuStrip::Render()
{
	Skin->RenderMenuStrip(this);
	this->GObject::Render();
}

void GMenuStrip::Layout()
{
	Content.SetRectangle(5,0,Width,Height-3);
	Layouter->Layout(this,false);
}

GMenuItem* GMenuStrip::AddItem( const String& menuItem )
{
	GMenuItem* r = new GMenuItem();
	r->SetRectangle(0,0,20,10);
	r->Text = menuItem;
	AddChild(r);
	return r;
}

GMenuItem* GMenuStrip::AddItem( const String& menuName, NoArgEvent* event )
{
	GMenuItem* r = AddItem(menuName);
	if (event)
	{
		r->Click += event;
	}
	return r;
}

