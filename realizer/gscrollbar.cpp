#include "stdafx.h"
#include "gscrollbar.h"
#include "vgui.h"

// normal, pressed, disabled, mouseover


GScrollBarDrag::GScrollBarDrag()
{
	ClassID = GSCROLLBARDRAG_CLASSID;
}

void GScrollBarDrag::Clicked( int x, int y, int button )
{

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
	// check the click poisition and add or subtract large change to there
}

