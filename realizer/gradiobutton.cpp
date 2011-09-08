#include "stdafx.h"
#include "gradiobutton.h"
#include "vgui.h"

inline void GRadioButton::SetGraphicState( GenericGraphicState id )
{
	int i = 0;
	if (Checked) i += 4;
	GraphicState = i + id;
}

GRadioButton::GRadioButton()
{
	GraphicState = 0;
	Checked = false;
	SetGraphicState(GGS_NORMAL);
	CheckAlign = CA_MiddleLeft;
	ClassID = GRADIOBUTTON_CLASSID;
}

void GRadioButton::MouseMove( int x, int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			SetGraphicState(GGS_DOWN);
		}
		else
		{
			SetGraphicState(GGS_OVER);
		}
	}
	else
	{
		SetGraphicState(GGS_OVER);
	}
}

void GRadioButton::MouseExit()
{
	SetGraphicState(GGS_NORMAL);
}

void GRadioButton::MouseUp( int x, int y, int button )
{
	if (MouseInside)
	{
		set_Checked(true);
	}
}

void GRadioButton::Render()
{
	Skin->RenderRadioButton(this);
}

void GRadioButton::set_Checked( bool value )
{
	if (Checked == value) return;
	if (value)
	{
		// TODO: enumerate these with GObjectEnumerator for nicer look
		GObject* curObj = Parent->LastItem;
		while(curObj)
		{
			if (curObj->ClassID == GRADIOBUTTON_CLASSID)
			{
				GRadioButton* curRadio = (GRadioButton*)curObj;
				if (curRadio->Checked)
				{
					curRadio->Checked = false;
					curRadio->GraphicState -= 4;
				}
			}
			curObj = curObj->PrevItem;
		}
	}
	else
	{
		GraphicState -= 4;
	}
	Checked = value;
}