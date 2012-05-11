#include "stdafx.h"
#include "gscrollbar.h"
#include "vgui.h"
#include "gfont.h"

GScrollBarDrag::GScrollBarDrag()
{
	ClassID = GSCROLLBARDRAG_CLASSID;
	Dragging = false;
}

void GScrollBarDrag::Clicked( int x, int y, int button )
{
	
}

void GScrollBarDrag::MouseDown( int x,int y, int button )
{
	DragPoint.Set(Master->X,Master->Y);
	DragPos = this->Y;
	Dragging = true;
}

void GScrollBarDrag::MouseUp( int x,int y,int button )
{
	Dragging = false;
}

void GScrollBarDrag::Update()
{
	if (Dragging)
	{
		vec2i Diff(Master->X,Master->Y);
		Diff -= DragPoint;

		SetTop(DragPos + Diff.y);

		GScrollBar* p = (GScrollBar*)Parent;
		p->SetValueFromDragPos();
	}

	this->GObject::Update();
}

GScrollBarButton::GScrollBarButton()
{
	Direction = BD_LEFT;
	ClassID = GSCROLLBARBUTTON_CLASSID;
}

class GScrollBarButtonUp: public GScrollBarButton
{
public:
	GScrollBarButtonUp()
	{
		Direction = BD_UP;
	}

	void Clicked(int x, int y, int button)
	{
		GScrollBar* p = (GScrollBar*)Parent;
		p->setValue(p->Value - p->SmallChange);
	}
};

class GScrollBarButtonDown: public GScrollBarButton
{
public:
	GScrollBarButtonDown()
	{
		Direction = BD_DOWN;
	}

	void Clicked(int x, int y, int button)
	{
		GScrollBar* p = (GScrollBar*)Parent;
		p->setValue(p->Value + p->SmallChange);
	}
};


GScrollBar::GScrollBar()
{
	MinValue = 0;
	MaxValue = 100;
	Value = 0;
	SmallChange = 1;
	LargeChange = 10;
	ClassID = GSCROLLBAR_CLASSID;
	UpButton = new GScrollBarButtonUp();
	DownButton = new GScrollBarButtonDown();
	DragBar = new GScrollBarDrag();
	Orientation = SBO_VERTICAL;
}

void GScrollBar::Render()
{
	Skin->RenderScrollBar(this);

	byte tmp[512];
	TStringBuilder sb(tmp,512);
	sb.Append("Value:");
	sb.Append(Value);
	sb.Append("  ");
	sb.Append("DragY:");
	sb.Append(DragBar->Top);
	sb.Append("  ");
	sb.Append("DragHeight:");
	sb.Append(DragBar->Height);

	Font->Render(sb,100,100,TColors::Wheat);

	sb.UnbindByteArray();
}

void GScrollBar::Layout()
{
	if (ItemCount < 3)
	{
		AddChild(UpButton);
		AddChild(DownButton);
		AddChild(DragBar);
	}

	Skin->LayoutScrollBar(this);
}

void GScrollBar::Clicked( int x, int y, int button )
{
	if (y < DragBar->Top)
	{
		setValue(Value - LargeChange);
	}
	else
	{
		setValue(Value + LargeChange);
	}
	// check the click position and add or subtract large change to there
}

void GScrollBar::SetValueFromDragPos()
{
	GScrollbarLayout st(*this);
	st.Calculate(false);
	st.CalculateValue(); // calculate value first
	st.CalculateDragPosition(); // then calculate position from just calculated value to fix itself
	st.SetDragPos(); // set it

	//Layout(); // Self fixing lol
}

void GScrollBar::setValue( int newValue )
{
	int oldVal = Value;
	Value = MathDriver::Clamp<int>(MinValue,MaxValue - LargeChange,newValue);
	if (Value != oldVal)
	{
		Layout();
	}
}