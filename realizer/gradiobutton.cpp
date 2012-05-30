#include "stdafx.h"
#include "gradiobutton.h"
#include "vgui.h"

GRadioButton::GRadioButton()
{
	Checked = false;
	CheckAlign = CA_MiddleLeft;
	TextAlign = CA_TopLeft;
	ClassID = GRADIOBUTTON_CLASSID;
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

		TLinkedListEnumerator<GObject*> pobj(Parent);
		while(pobj.MoveNext())
		{
			if (pobj.Current->ClassID == GRADIOBUTTON_CLASSID)
			{
				GRadioButton* curRadio = (GRadioButton*)pobj.Current;
				if (curRadio->Checked)
				{
					curRadio->Checked = false;
				}
			}
		}
	}
	Checked = value;
}

void GRadioButton::Clicked( int x, int y, int button )
{
	set_Checked(true);
}