#include "stdafx.h"
#include "gbutton.h"
#include "vgui.h"

GButton::GButton()
{
	TextAlign = CA_MiddleCenter;
	ClassID = GBUTTON_CLASSID;
}

void GButton::Render()
{
	Skin->RenderButton(this);
}

void GButton::Clicked( int x, int y, int button )
{
	Click.call();
}
// 0 = Normal, 1 = Pressed, 2 = Disabled, 3 = Mouse Over, 4 = Focus & Default

