#include "stdafx.h"
#include "gsystembutton.h"

GSystemButton::GSystemButton()
{
	ClassID = GSYSTEMBUTTON_CLASSID;
}

void GSystemButton::Render()
{
	Skin->RenderSystemButton(this);
}

void GSystemButton::Layout()
{
	Skin->LayoutSystemButton(this);
	GObject::Layout();
}