#include "stdafx.h"
#include "gcheckbox.h"
#include "vgui.h"

inline void GCheckBox::SetGraphicState( GenericGraphicState id )
{
	GraphicState = (CheckState * 4) + id;
}

GCheckBox::GCheckBox()
{
	GraphicState = 0;
	ThreeState = false;
	CheckState = CS_Unchecked;
	Checked = false;
	SetGraphicState(GGS_NORMAL);
	CheckAlign = CA_MiddleLeft;
	ClassID = GCHECKBOX_CLASSID;
}

void GCheckBox::MouseMove( int x, int y )
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

void GCheckBox::MouseExit()
{
	SetGraphicState(GGS_NORMAL);
}

void GCheckBox::MouseUp( int x, int y, int button )
{
	if (MouseInside)
	{
		if (ThreeState)
		{
			switch(CheckState)
			{
			case CS_Unchecked:
				CheckState = CS_Checked;
				Checked = true;
				break;

			case CS_Checked:
				CheckState = CS_Indeterminate;
				Checked = false;
				break;

			case CS_Indeterminate:
				CheckState = CS_Unchecked;
				Checked = false;
				break;
			}
		}
		else
		{
			Checked = !Checked;
			if (Checked)
			{
				CheckState = CS_Checked;
			}
			else
			{
				CheckState = CS_Unchecked;
			}
		}
	}
}

void GCheckBox::Render()
{
	Skin->RenderCheckBox(this);
}
