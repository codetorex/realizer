#include "stdafx.h"
#include "gtoolstrip.h"
#include "vgui.h"
#include "glayout.h"
#include "gfont.h"

void GToolStripButton::Render()
{
	Skin->RenderToolButton(this);

	if (DrawStyle == GDS_IMAGE || DrawStyle == GDS_IMAGETEXT)
	{
		Image.Render();
	}

	if (DrawStyle == GDS_TEXT || DrawStyle == GDS_IMAGETEXT)
	{
		IRectangle dRect = DrawRegion;
		dRect += Content;
		Font->Render(Text,dRect.X + TextPosition.X,dRect.Y + TextPosition.Y,ForeColor);
	}

}

GToolStripButton::GToolStripButton()
{
	ClassID = GTOOLBUTTON_CLASSID;
	DrawStyle = GDS_IMAGE;
}

void GToolStripButton::Layout()
{
	int reqWidth,reqHeight,textWidth;

	OwnObject(&Image);

	if (!Image.Visible)
	{
		if (DrawStyle == GDS_IMAGE || DrawStyle == GDS_IMAGETEXT)
		{
			DrawStyle = GDS_TEXT;
			return;
		}
	}

	if (DrawStyle == GDS_TEXT || DrawStyle == GDS_IMAGETEXT)
	{
		TCharacterEnumerator s(Text);
		textWidth = 4 + Font->GetStringWidth(s) + 4;
	}

	switch( DrawStyle )
	{
	case GDS_IMAGE:
		reqWidth = Image.Width;
		reqHeight = Image.Height;
		break;

	case GDS_IMAGETEXT:
		reqWidth = Image.Width + textWidth;
		reqHeight = Math.Max(Image.Height, Font->Size);
		Image.Move(0,(Content.Height - Image.Height) / 2);
		Image.SetParent(this);
		TextPosition.X = Image.Right() + 4;
		break;

	case GDS_TEXT:
		reqWidth = textWidth;
		reqHeight = Font->Size;
		TextPosition.X = 4;
		break;
	}

	Skin->LayoutToolButton(this); // get the margins?
	ChangeHeight(Border.TotalVerticalPad() + reqHeight);
	ChangeWidth(Border.TotalHorizontalPad() + reqWidth);

	// TODO: Align image and text to content by Align paramaters of this object

	/**
	 * Hmm best solution so far.
	 */
	if (Parent)
	{
		GObject* p = (GObject*)Parent;

		TLinkedListEnumerator<GObject*> le(p);
		while(le.MoveNext())
		{
			if (Height < le.Current->Height)
			{
				ChangeHeight(le.Current->Height);
			}
		}
	}

	UpdateContent();
	TextPosition.Y = (Content.Height - Font->Height) / 2;
}

void GToolStripButton::Clicked( int x, int y, int button )
{
	Click.call();
}

GToolStrip::GToolStrip()
{
	ClassID = GTOOLSTRIP_CLASSID;
	Layouter = &GLayoutHorizontalOverflow::Instance;
}

void GToolStrip::Render()
{
	Skin->RenderToolStrip(this);
	this->GObject::Render();
}

void GToolStrip::Layout()
{
	Content.SetRectangle(2,2,Width,Height-4);
	Layouter->Layout(this,false);
}

GToolStripButton* GToolStrip::AddButton( const String& buttonName, GImage& image, NoArgEvent* event, bool imageOnly /*= true*/ )
{
	GToolStripButton* nb = new GToolStripButton();
	nb->SetRectangle(0,0,16,16);
	nb->Text = buttonName;
	nb->Image.SetImage(image);
	if (event)
	{
		nb->Click += event;
	}
	
	if (!imageOnly)
	{
		nb->DrawStyle = GDS_IMAGETEXT;
	}

	AddChild(nb);

	return nb;
}

GToolStripButton* GToolStrip::AddButton( const String& buttonName, NoArgEvent* event )
{
	GToolStripButton* nb = new GToolStripButton();
	nb->SetRectangle(0,0,16,16);
	nb->Text = buttonName;
	if (event)
	{
		nb->Click += event;
	}

	nb->DrawStyle = GDS_TEXT;

	AddChild(nb);

	return nb;
}