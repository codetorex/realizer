#include "stdafx.h"
#include "gscrollbar.h"
#include "vgui.h"
#include "gfont.h"

GScrollBarDrag::GScrollBarDrag()
{
	ClassID = GSCROLLBARDRAG_CLASSID;
	Dragging = false;
}

void GScrollBarDrag::OnMouseDown( int x,int y, int button )
{
	DragPoint.SetVector(Master->X,Master->Y);
	DragPos.SetVector(this->X,this->Y);
	Dragging = true;
}

void GScrollBarDrag::OnMouseUp( int x,int y,int button )
{
	Dragging = false;
}

void GScrollBarDrag::Update()
{
	if (Dragging)
	{
		GScrollBar* p = (GScrollBar*)Parent;

		IPosition Diff(Master->X,Master->Y);
		Diff -= DragPoint;

		if (p->Orientation == GO_VERTICAL)
		{
			MoveY(DragPos.Y + Diff.Y);
		}
		else
		{
			MoveX(DragPos.X + Diff.X);
		}
		
		p->SetValueFromDragPos();
	}

	this->GObject::Update();
}

void GScrollBarDrag::OnMouseWheel( int x,int y, int delta )
{
	GScrollBar* p = (GScrollBar*)Parent;
	p->OnMouseWheel(x,y,delta); // forward to parent
}

GScrollBarButton::GScrollBarButton(): GTimeEffectActivation(true)
{
	InitializeTimeEffect(400,60);
	Direction = BD_LEFT;
	ClassID = GSCROLLBARBUTTON_CLASSID;
}

void GScrollBarButton::Update()
{
	this->GObject::Update();
	if (MouseInside)
	{
		UpdateTimeEffect(); // TODO: these timing stuff should kept somewhere we sure about going to updated regularly?
	}
}

void GScrollBarButton::OnMouseUp( int x,int y,int button )
{
	DisableTimeEffect();
}

void GScrollBarButton::OnMouseDown( int x,int y, int button )
{
	EnableTimeEffect();
}
class GScrollBarButtonUp: public GScrollBarButton
{
public:
	void Pulse()
	{
		GScrollBar* p = (GScrollBar*)Parent;
		p->setValue(p->Value - p->SmallChange);
	}
};

class GScrollBarButtonDown: public GScrollBarButton
{
public:
	void Pulse()
	{
		GScrollBar* p = (GScrollBar*)Parent;
		p->setValue(p->Value + p->SmallChange);
	}
};

GScrollBar::GScrollBar(): GTimeEffectActivation(true)
{
	InitializeTimeEffect(400,60);
	MinValue = 0;
	MaxValue = 100;
	Value = 0;
	SmallChange = 1;
	LargeChange = 10;
	ClassID = GSCROLLBAR_CLASSID;
	UpButton = new GScrollBarButtonUp();
	DownButton = new GScrollBarButtonDown();
	DragBar = new GScrollBarDrag();
	setOrientation(GO_VERTICAL);
}

void GScrollBar::Render()
{
	Skin->RenderScrollBar(this);

	/*byte tmp[512];
	TStringBuilder sb(tmp,512);
	sb.Append("Value:");
	sb.Append(Value);
	sb.Append("  ");
	sb.Append("DragY:");
	sb.Append(DragBar->Top);
	sb.Append("  ");
	sb.Append("DragHeight:");
	sb.Append(DragBar->Height);

	Font->Render(sb,ScreenRegion.X +150,ScreenRegion.Y+150,TColors::Wheat);

	sb.UnbindByteArray();*/
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
	Value = Math.Clamp<int>(MinValue,MaxValue - LargeChange,newValue);
	if (Value != oldVal)
	{
		Layout();
		ValueChanged.call();
	}
}

void GScrollBar::Pulse()
{
	if (Orientation == GO_VERTICAL)
	{
		if (mY < DragBar->Y)
		{
			setValue(Value - LargeChange);
		}
		else
		{
			setValue(Value + LargeChange);
		}
	}
	else
	{
		if (mX < DragBar->X)
		{
			setValue(Value - LargeChange);
		}
		else
		{
			setValue(Value + LargeChange);
		}
	}
}

void GScrollBar::Update()
{
	this->GObject::Update();
	if (MouseInside)
	{
		UpdateTimeEffect(); // TODO: these timing stuff should kept somewhere we sure about going to updated regularly?
	}
}

void GScrollBar::OnMouseMove( int x,int y )
{
	this->GButtonBase::OnMouseMove(x,y);
	mX = x;
	mY = y;
}

void GScrollBar::OnMouseExit()
{
	this->GButtonBase::OnMouseExit();
	DisableTimeEffect();
}

void GScrollBar::OnMouseUp( int x,int y,int button )
{
	DisableTimeEffect();
}

void GScrollBar::OnMouseDown( int x,int y, int button )
{
	EnableTimeEffect();
}

void GScrollBar::setOrientation( GOrientation newOrientation )
{
	Orientation = newOrientation;
	if (newOrientation == GO_VERTICAL)
	{
		UpButton->Direction = GScrollBarButton::BD_UP;
		DownButton->Direction = GScrollBarButton::BD_DOWN;
	}
	else
	{
		UpButton->Direction = GScrollBarButton::BD_LEFT;
		DownButton->Direction = GScrollBarButton::BD_RIGHT;
	}
}

void GScrollBar::OnMouseWheel( int x,int y, int delta )
{
	setValue(Value - (delta * SmallChange));
}