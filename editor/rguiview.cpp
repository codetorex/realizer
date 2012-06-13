#include "stdafx.h"
#include "rguiview.h"
#include "gcomponents.h"
#include "cengine.h"
#include "reditor.h"


void GObjectResizerGrip::Render()
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(DrawRegion,TColors::Black);
}

void GObjectResizer::Render()
{
	TColor32 dColor = TColors::Gray;
	dColor.a = 128;

	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(DrawRegion,dColor);

	this->GObject::Render(); // render grips
}

void GObjectResizer::Layout()
{
	if (!(Grips[0].Parent))
	{
		for (int i=0;i<8;i++)
		{
			AddChild(&(Grips[i]));
		}
	}

	/**
	 * In case of skin change or something, children will be updated
	 */
	for (int i=0;i<8;i++)
	{
		OwnObject(&(Grips[i]));
		Grips[i].SetSize(GripSize,GripSize);
	}

	int halfGrip = GripSize / 2;
	int halfX = (Width / 2);
	int halfY = (Height / 2);

	Grips[0].SetVector(-halfGrip,-halfGrip);
	Grips[1].SetVector(halfX - halfGrip,-halfGrip);
	Grips[2].SetVector(Width-halfGrip,-halfGrip);

	Grips[3].SetVector(-halfGrip,halfY-halfGrip);
	Grips[4].SetVector(Width-halfGrip,halfY-halfGrip);

	Grips[5].SetVector(-halfGrip,Height-halfGrip);
	Grips[6].SetVector(halfX-halfGrip,Height-halfGrip);
	Grips[7].SetVector(Width-halfGrip,Height-halfGrip);

	// 2px offset

	this->GObject::Layout();
}

GObjectResizer::GObjectResizer()
{
	GripSize = 6;
}

void GObjectResizer::setGripSize( int newGripSize )
{
	GripSize = newGripSize;
	Layout();
}

void GGUICanvas::Layout()
{
	if (!Resizer.Parent)
	{
		// TODO: THIS REALLY GETS ANNOYING, IMPLEMENT, FIX INITILIZE FUNCTION AND DO THESE THINGS IN THERE
		AddChild(&Resizer);

		Resizer.Visible = false;
	}

	this->GObject::Layout();
}

void GGUICanvas::Render()
{
	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(DrawRegion,TColors::White);

	if (RootNode)
	{
		TArrayEnumerator< GTreeNode* > ne(RootNode->Nodes);
		while(ne.MoveNext())
		{
			GGUIItem* curItem = (GGUIItem*)(ne.Current);
			OwnObject(curItem->Object);
			curItem->Object->Render();
		}
	}
	
	this->GObject::Render();

	
}

GGUICanvas::GGUICanvas()
{
	RootNode = 0;
}

RGUIView::RGUIView()
{
	Document = 0;
	Name = "GUI Editor";

	Canvas.Dock = DCK_FILL;
	ViewTool.Dock = DCK_FILL;
	ToolProperty.Dock = DCK_FILL;
	Tools.Dock = DCK_TOP;
	CanvasTree.Dock = DCK_FILL;
	ToolProperty.Orientation = GO_HORIZONTAL;

	CanvasTree.ShowLines = true;
	CanvasTree.ShowPlusMinus = true;
	CanvasTree.ShowRoot = true;
}

void RGUIView::Layout()
{
	/// Objects not set yet lets set them
	if (!ViewTool.Parent)
	{
		AddChild(&ViewTool);
		ViewTool.Panel2.AddChild(&ToolProperty);
		ViewTool.Panel1.AddChild(&Canvas);

		ToolProperty.Panel1.AddChild(&Tools);
		ToolProperty.Panel1.AddChild(&CanvasTree);

		GObject* p = (GObject*)Parent;
		ViewTool.SplitterDistance = (int)((float)p->Width * 0.80f);
		ToolProperty.SplitterDistance = p->Height / 2;

		Tools.AddButton("Button",*Resources.UIButton,0);
	}

	this->GObject::Layout();
}

void RGUIView::DocumentChanged()
{
	if (!Document)
	{
		Document = new RGUIDocument();
		Document->InitializeEmptyDocument();
	}
	CanvasTree.RootNode = GetGUIDocument()->RootNode;
	CanvasTree.RootNode->TreeView = &CanvasTree;
	CanvasTree.RootNode->SetTreeViewForAllChilds();
}

void RGUIView::CreateButton()
{
	if (!CanvasTree.SelectedNode)
	{
		return;
	}

	GGUIItem* newItem = new GGUIItem()
}
