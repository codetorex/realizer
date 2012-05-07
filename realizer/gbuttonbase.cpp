#include "stdafx.h"
#include "gbuttonbase.h"
#include "vgui.h"

GButtonBase::GButtonBase()
{
	SetGraphic(GBG_NORMAL);
	ClassID = GBUTTONBASE_CLASSID;
}

void GButtonBase::MouseMove( int x,int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			SetGraphic(GBG_DOWN);
		}
		else
		{
			SetGraphic(GBG_OVER);
		}
	}
	else
	{
		SetGraphic(GBG_OVER);
	}
}

void GButtonBase::MouseExit()
{
	SetGraphic(GBG_NORMAL);
}

void GButtonBase::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		Clicked(x,y,button);
	}
}

