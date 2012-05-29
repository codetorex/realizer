#include "stdafx.h"
#include "gbuttonbase.h"
#include "vgui.h"
#include "gfont.h"

GButtonBase::GButtonBase()
{
	SetGraphic(GBG_NORMAL);
	ClassID = GBUTTONBASE_CLASSID;
	//Image = 0;
	TextAlign = CA_MiddleCenter;
	ImageAlign = CA_MiddleLeft;
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

void GButtonBase::Layout()
{
	this->GObject::Layout();
	OwnObject(&Image);

	IRectangle textRect;
	int StringWidth = 10;
	if (Text.Length > 0)
	{
		TCharacterEnumerator ce(Text);
		StringWidth = Font->GetStringWidth(ce);
	}

	this->GObject::Layout(); // update stuff?
	
	textRect.SetRectangle(0,0,StringWidth,Font->Height);
	Align(textRect,TextAlign);

	if (Image.Visible)
	{
		AlignBox(Image,ImageAlign);
		if (ImageAlign == TextAlign)
		{
			if ( textRect.IsColliding(Image) )
			{
				switch(ImageAlign)
				{
				case CA_MiddleLeft:
					textRect.X += Image.Right() + Image.Margin.Right + Image.Margin.Left + ImageGap;
					break;

				default:
					throw NotImplementedException();
				}
			}
		}
	}

	TextPosition = textRect;
}

void GButtonBase::setText( const TString& newText )
{
	Text = newText;
	Layout();
}

void GButtonBase::Update()
{
	this->GObject::Update();
	if (Image.Visible)
	{
		Image.Update();
	}
}