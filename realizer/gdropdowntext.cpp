#include "stdafx.h"
#include "gdropdowntext.h"

GDropDownText::GDropDownText()
{
	EditBox.KeyPress += GetHandler(this, &GDropDownText::EditBox_KeyPress);
}

void GDropDownText::Layout()
{
	if (EditBox.Parent == 0)
	{
		AddChild(&EditBox);
	}
	OwnObject(&EditBox);
	EditBox.ChangeHeight(Font->Size+4);
	EditBox.Layout();
	this->GDropDown::Layout();
}

void GDropDownText::Show( int x, int y, int width )
{
	EditBox.Width = width;
	Layout();
	this->GDropDown::Show(x - Border.Left,y - Border.Top - 1);
	EditBox.SetFocus();
}

void GDropDownText::EditBox_KeyPress( void* sender, KeyEventArgs& e )
{
	if (e.KeyChar.Character == Keys::Esc)
	{
		// cancel
		Hide();
	}

	if (e.KeyChar.Character == Keys::Enter)
	{
		Edited.call();

		if (!Edited.HandlerCount)
		{
			Hide();
		}
	}
}

void GDropDownText::Clear()
{
	EditBox.Clear();
}

void GDropDownText::SetText( const TString& value )
{
	EditBox.set_Text(value);
}

void GDropDownText::SelectAll()
{
	EditBox.SelectAll();
}