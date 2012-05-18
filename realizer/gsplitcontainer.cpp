#include "stdafx.h"
#include "gsplitcontainer.h"
#include "vgui.h"

GSplitContainer::GSplitContainer()
{
	ClassID = GSPLITCONTAINER_CLASSID;
	SplitterWidth = 4;
	SplitterDistance = 100;
	Orientation = GO_VERTICAL;
	Dragging = false;
	AlwaysLayout = true;
	View = GSV_ALL;
	Panel1MinSize = 25;
	Panel2MinSize = 25;
}

void GSplitContainer::MouseDown( int x,int y, int button )
{
	// WE CAN CHANGE CURSOR AT THIS POINT
	DragPoint.SetVector(Master->X,Master->Y);
	DragDistance = SplitterDistance;
	Dragging = true;
}

void GSplitContainer::MouseUp( int x,int y,int button )
{
	Dragging = false;
	Layout();
}

void GSplitContainer::Update()
{
	if (Dragging)
	{
		IPosition Diff(Master->X,Master->Y);
		Diff -= DragPoint;

		int MaxSplitterDistance;

		if (Orientation == GO_VERTICAL)
		{
			SplitterDistance = DragDistance + Diff.X;
			MaxSplitterDistance = (Width - Panel2MinSize) - SplitterWidth;
		}
		else
		{
			SplitterDistance = DragDistance + Diff.Y;
			MaxSplitterDistance = (Height - Panel2MinSize) - SplitterWidth;
		}

		SplitterDistance = MathDriver::Clamp<int>(Panel1MinSize,MaxSplitterDistance,SplitterDistance);

		if (AlwaysLayout)
		{
			Layout();
		}
		
	}

	this->GObject::Update();
}

void GSplitContainer::Layout()
{
	if (Panel1.Parent == 0)
	{
		AddChild(&Panel1);
		AddChild(&Panel2);
	}

	Content.SetRectangle(0,0,Width,Height);

	switch (View)
	{
	case GSV_ALL:
		{
			Panel1.Visible = true;
			Panel2.Visible = true;

			int otherStart = SplitterDistance+SplitterWidth;

			if (Orientation == GO_VERTICAL)
			{
				Panel1.SetRectangle(0,0,SplitterDistance,Height);
				Panel2.SetRectangle(otherStart,0,Width-otherStart,Height);
			}
			else
			{
				Panel1.SetRectangle(0,0,Width,SplitterDistance);
				Panel2.SetRectangle(0,otherStart,Width,Height-otherStart);
			}

			Panel1.Layout();
			Panel2.Layout();
		}
		break;

	case GSV_PANEL1ONLY:
		{
			Panel1.Visible = true;
			Panel2.Visible = false;

			Panel1.SetRectangle(0,0,Width,Height);
			Panel1.Layout();
		}
		break;

	case GSV_PANEL2ONLY:
		{
			Panel1.Visible = false;
			Panel2.Visible = true;

			Panel2.SetRectangle(0,0,Width,Height);
			Panel2.Layout();
		}
		break;
	}
}

void GSplitContainer::Render()
{
	this->GObject::Render();

	const IRectangle& dRect = DrawRegion;

	//if (Dragging)
	{
		if (Orientation == GO_VERTICAL)
		{
			Skin->RenderHilight(dRect.X + SplitterDistance, dRect.Y,SplitterWidth,Height);
		}
		else
		{
			Skin->RenderHilight(dRect.X,dRect.Y + SplitterDistance,Width,SplitterWidth);
		}
	}
}

void GSplitContainer::setView( GSplitView newView )
{
	View = newView;
	Layout();
}