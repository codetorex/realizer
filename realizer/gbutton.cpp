#include "stdafx.h"
#include "gbutton.h"
#include "vgui.h"

GButton::GButton()
{
	GraphicState = 0;
}

void GButton::Render()
{
	Skin->RenderButton(this);
}

void GButton::MouseMove( int x,int y )
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

void GButton::MouseExit()
{
	GraphicState = 0;
}

void GButton::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		Clicked.call();
	}
}
// 0 = Normal, 1 = Pressed, 2 = Disabled, 3 = Mouse Over, 4 = Focus & Default

