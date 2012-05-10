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
	/*if (moving)
	{
		int dx,dy;
		dx = Master->X - gx;
		dy = Master->Y - gy;

		GObject* p = (GObject*)Parent;
		p->SetLeft(windowX + dx);
		p->SetTop(windowY + dy);
	}*/
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
		p->Value -= p->SmallChange;
		if (p->Value < p->MinValue)
		{
			p->Value = p->MinValue;
		}

		p->Layout();
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
		p->Value += p->SmallChange;
		if (p->Value > p->MaxValue)
		{
			p->Value = p->MaxValue;
		}

		p->Layout();
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

	int TotalValues = MaxValue - MinValue;
	int AvailableHeight = DownButton->Top - UpButton->Bottom;
	float DragHeight = ((float)LargeChange / (float)TotalValues) * (float)AvailableHeight;

	float AvailableHeight2 = (float)AvailableHeight - DragHeight;
	float PixelPerItem = AvailableHeight2 / (float)TotalValues;
	//float DragHeight = PixelPerItem * (float)LargeChange;
	float DragY = (float)UpButton->Bottom + (((float)Value - (float)MinValue) * PixelPerItem);

	/*if (DragHeight < Skin->smallestDrag)
	{
		DragHeight = skin->SmallestDragHeight;
	}*/

	DragBar->SetHeight(DragHeight);
	DragBar->SetTop(DragY);
}

void GScrollBar::Clicked( int x, int y, int button )
{
	if (y < DragBar->Top)
	{
		Value -= LargeChange;
		if (Value < MinValue)
		{
			Value = MinValue;
		}
	}
	else
	{
		Value += LargeChange;
		if (Value > MaxValue)
		{
			Value = MaxValue;
		}
	}

	Layout();
	// check the click poisition and add or subtract large change to there
}

void GScrollBar::SetValueFromDragPos()
{
	int TotalValues = MaxValue - MinValue;
	int AvailableHeight = DownButton->Top - UpButton->Bottom;

	float AvailableHeight2 = (float)AvailableHeight - (float)DragBar->Height;
	float PixelPerItem = AvailableHeight2 / (float)TotalValues;

	int rtop = DragBar->Top - UpButton->Bottom;
	float rval = (float)rtop / PixelPerItem;
	rval += (float)MinValue;
	
	if (rval < MinValue)
	{
		rval = MinValue;
	}

	if (rval > MaxValue)
	{
		rval = MaxValue;
	}
	
	Value = rval;
	Layout(); // Self fixing lol
}
