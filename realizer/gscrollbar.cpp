#include "stdafx.h"
#include "gscrollbar.h"
#include "vgui.h"

// normal, pressed, disabled, mouseover


GScrollBarDrag::GScrollBarDrag()
{
	GraphicState = 0;
	ClassID = GSCROLLBARDRAG_CLASSID;
}

void GScrollBarDrag::MouseMove( int x,int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			GraphicState = 1;
		}
		else
		{
			GraphicState = 2;
		}
	}
	else
	{
		GraphicState = 2;
	}
}

void GScrollBarDrag::MouseExit()
{
	GraphicState = 0;
}

void GScrollBarDrag::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		
	}
}


GScrollBarButton::GScrollBarButton()
{
	Direction = BD_LEFT;
	GraphicState = 0;
	ClassID = GSCROLLBARBUTTON_CLASSID;
}

void GScrollBarButton::MouseMove( int x,int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			GraphicState = 1;
		}
		else
		{
			GraphicState = 3;
		}
	}
	else
	{
		GraphicState = 3;
	}
}

void GScrollBarButton::MouseExit()
{
	GraphicState = 0;
}

void GScrollBarButton::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		Clicked();
	}
}


class GScrollBarButtonUp: public GScrollBarButton
{
public:
	GScrollBarButtonUp()
	{
		Direction = BD_UP;
	}

	void Clicked()
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

	void Clicked()
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
	GraphicState = 0;
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

void GScrollBar::MouseMove( int x,int y )
{
	if (Master->Focused == this)
	{
		if (Master->ButtonState[0] && MouseInside)
		{
			GraphicState = 1;
		}
		else
		{
			GraphicState = 3;
		}
	}
	else
	{
		GraphicState = 3;
	}
}

void GScrollBar::MouseExit()
{
	GraphicState = 0;
}

void GScrollBar::MouseUp( int x,int y,int button )
{
	if (MouseInside)
	{
		// check if y is in upper part of the drag sub large change
		// if its lower than drag add large change
	}
}
