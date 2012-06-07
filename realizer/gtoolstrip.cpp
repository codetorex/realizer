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
		Font->Render(Text,dRect.X + TextX,dRect.Y + TextY,ForeColor);
	}

}

GToolStripButton::GToolStripButton()
{
	ClassID = GTOOLBUTTON_CLASSID;
	GraphicState = 0;
	DrawStyle = GDS_IMAGE;
}

void GToolStripButton::OnMouseMove( int x,int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			GraphicState = 2;
		}
		else
		{
			GraphicState = 1;
		}
	}
	else
	{
		GraphicState = 1;
	}
}

void GToolStripButton::OnMouseExit()
{
	GraphicState = 0;
}

void GToolStripButton::OnMouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		Click.call();
	}
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
		reqHeight = MathDriver::Max(Image.Height, Font->Size);
		Image.Move(0,(Content.Height - Image.Height) / 2);
		Image.SetParent(this);
		TextX = Image.Right() + 4;
		break;

	case GDS_TEXT:
		reqWidth = textWidth;
		reqHeight = Font->Size;
		TextX = 4;
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
	TextY = (Content.Height - Font->Height) / 2;
}

void GToolStripButton::Update()
{
	this->GObject::Update();
	Image.Update();
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

GToolStripButton* GToolStrip::AddButton( const TString& buttonName, GImage& image, NoArgEvent* event, bool imageOnly /*= true*/ )
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

GToolStripButton* GToolStrip::AddButton( const TString& buttonName, NoArgEvent* event )
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