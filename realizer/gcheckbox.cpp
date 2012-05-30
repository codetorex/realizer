#include "stdafx.h"
#include "gcheckbox.h"
#include "vgui.h"

GCheckBox::GCheckBox()
{
	ThreeState = false;
	CheckState = CS_Unchecked;
	Checked = false;
	CheckAlign = CA_MiddleLeft;
	TextAlign = CA_TopLeft;
	ClassID = GCHECKBOX_CLASSID;
}

void GCheckBox::Render()
{
	Skin->RenderCheckBox(this);
}

void GCheckBox::Clicked( int x,int y, int button )
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
	CheckedChanged.call();
}

void GCheckBox::setChecked( bool checked )
{
	if (checked == this->Checked)
		return;

	if(checked)
	{
		this->Checked = true;
		CheckState = CS_Checked;
	}
	else
	{
		this->Checked = false;
		CheckState = CS_Unchecked;
	}

	CheckedChanged.call();
}