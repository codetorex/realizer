#include "stdafx.h"
#include "gtextbox.h"
#include "vgui.h"
#include "tinputenum.h"


GTextBox::GTextBox()
{
	ClassID = GTEXTBOX_CLASSID;
}

void GTextBox::Render()
{
	Skin->RenderTextBox(this);
	this->GTextBoxBase::Render();
}

/*void GTextBox::KeyPress( dword keyID )
{
	if (keyID == Keys::BackSpace)
	{
		return;
	}
	Text.AppendUnicode(keyID);
}

void GTextBox::LostFocus()
{

}

void GTextBox::GotFocus()
{

}

void GTextBox::KeyDown( dword keyID )
{
	if (keyID == Keys::BackSpace)
	{
		Text.Truncate(Text.Length-1);
	}
}*/