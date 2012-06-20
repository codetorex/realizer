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

void GObjectResizerGrip::OnMouseDown( int x,int y, int button )
{
	Draging = true;
	DragPos.SetVector(Master->X,Master->Y);
}

void GObjectResizerGrip::OnMouseUp( int x,int y,int button )
{
	UpdateDrag();
	Draging = false;
}

void GObjectResizerGrip::UpdateDrag()
{
	if (!Draging)
		return;


	IPosition diff(Master->X,Master->Y);
	diff -= DragPos;

	GObjectResizer* resizer = (GObjectResizer*)Parent;
	GObject* obj = resizer->ResizingObject;

	switch(GripType)
	{
	case GT_TOPLEFT:
		obj->X += diff.X;
		obj->Y += diff.Y;
		obj->Width -= diff.X;
		obj->Height -= diff.Y;
		break;

	case GT_TOP:
		obj->Y += diff.Y;
		obj->Height -= diff.Y;
		break;

	case GT_TOPRIGHT:
		obj->Width += diff.X;
		obj->Y += diff.Y;
		obj->Height -= diff.Y;
		break;

	case GT_LEFT:
		obj->X += diff.X;
		obj->Width -= diff.X;
		break;

	case GT_RIGHT:
		obj->Width += diff.X;
		break;

	case GT_BOTTOMLEFT:
		obj->X += diff.X;
		obj->Width -= diff.X;
		obj->Height += diff.Y;
		break;

	case GT_BOTTOM:
		obj->Height += diff.Y;
		break;

	case GT_BOTTOMRIGHT:
		obj->Width += diff.X;
		obj->Height += diff.Y;
		break;
	}

	obj->Update();
	obj->Layout();
	resizer->WrapItem();
	
	DragPos.SetVector(Master->X,Master->Y);
}

void GObjectResizerGrip::Update()
{
	this->GObject::Update();
	if (Draging)
	{
		UpdateDrag();
	}
}

GObjectResizerGrip::GObjectResizerGrip()
{
	Draging = false;
}

void GObjectResizer::Render()
{
	TColor32 dColor = TColors::Gray;
	dColor.a = 128;

	IRectangle dRect(DrawRegion);
	dRect.Shrink(GripSize/2);

	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(dRect,dColor);

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

	Grips[0].SetVector(0,0);
	Grips[1].SetVector(halfX-halfGrip,0);
	Grips[2].SetVector(Width-GripSize,0);

	Grips[3].SetVector(0,halfY-halfGrip);
	Grips[4].SetVector(Width-GripSize,halfY-halfGrip);

	Grips[5].SetVector(0,Height-GripSize);
	Grips[6].SetVector(halfX-halfGrip,Height-GripSize);
	Grips[7].SetVector(Width-GripSize,Height-GripSize);

	// 2px offset

	Grips[0].GripType = GObjectResizerGrip::GT_TOPLEFT;
	Grips[1].GripType = GObjectResizerGrip::GT_TOP;
	Grips[2].GripType = GObjectResizerGrip::GT_TOPRIGHT;

	Grips[3].GripType = GObjectResizerGrip::GT_LEFT;
	Grips[4].GripType = GObjectResizerGrip::GT_RIGHT;

	Grips[5].GripType = GObjectResizerGrip::GT_BOTTOMLEFT;
	Grips[6].GripType = GObjectResizerGrip::GT_BOTTOM;
	Grips[7].GripType = GObjectResizerGrip::GT_BOTTOMRIGHT;

	this->GObject::Layout();
}

GObjectResizer::GObjectResizer()
{
	GripSize = 6;
	Draging = false;
	ResizingObject = 0;
}

void GObjectResizer::setGripSize( int newGripSize )
{
	GripSize = newGripSize;
	Layout();
}

void GObjectResizer::OnMouseDown( int x,int y, int button )
{
	GGUICanvas* canvas = (GGUICanvas*)Parent;

	if (canvas->SelectItemByClick)
	{
		GObject* oldObj = ResizingObject ;
		IPosition nPos(x,y);
		nPos += *this;

		GGUIItem* item = canvas->GetItemAt(x,y);
		
		if (item)
		{
			if (ResizingObject != item->Object)
			{
				canvas->OnMouseDown(nPos.X,nPos.Y,button);
			}
		}
		else
		{
			if (ResizingObject != 0)
			{
				canvas->OnMouseDown(nPos.X,nPos.Y,button);
			}
		}
		
		

		

		//canvas->OnMouseDown(nPos.X,nPos.Y,button);
		
		/*if ( oldObj != ResizingObject) // its changed
		{
			throw Exception("IT WORKS");
		}*/
	}

	if (ResizingObject)
	{
		DragObjectPos.SetVector(*ResizingObject);
		DragPos.SetVector(Master->X, Master->Y);
		Draging = true;
	}
}

void GObjectResizer::OnMouseUp( int x,int y,int button )
{
	UpdateDrag();
	Draging = false;
}

void GObjectResizer::UpdateDrag()
{
	if (!Draging)
		return;

	if (!ResizingObject)
		return;

	IPosition mousePos(Master->X,Master->Y);
	mousePos -= DragPos;
	mousePos += DragObjectPos;

	ResizingObject->SetVector(mousePos);
	ResizingObject->GObject::Update();
	WrapItem();
	
	this->GObject::Update();
}

void GObjectResizer::Update()
{
	this->GObject::Update();

	UpdateDrag();
}

void GObjectResizer::WrapItem()
{
	if(!ResizingObject) 
		return;

	GObject* p = (GObject*)Parent;
	IRectangle sr = ResizingObject->DrawRegion;
	if (p)
	{
		sr -= p->DrawRegion;
	}
	sr.Inflate(GripSize);
	sr.Width -= 1;
	sr.Height -= 1;

	SetRectangle(sr);
	Layout();
}

void GObjectResizer::OnKeyDown( ui32 keyID )
{
	if (!ResizingObject)
		return;

	if (keyID == Keys::Ctrl)
	{
		Ctrl = true;
		return;
	}

	if (keyID == Keys::Shift)
	{
		Shift = true;
		return;
	}

	/// TODO: 
	/// TODO: if shift then it should 1 else it should be big as grid size?
	int scale = Shift ? 5 : 1;

	if (Ctrl)
	{
		switch (keyID)
		{
		case Keys::Up:
			ResizingObject->Height -= scale;
			break;

		case Keys::Down:
			ResizingObject->Height += scale;
			break;

		case Keys::Left:
			ResizingObject->Width -= scale;
			break;

		case Keys::Right:
			ResizingObject->Width += scale;
			break;
		}
	}
	else
	{
		switch (keyID)
		{
		case Keys::Up:
			ResizingObject->Y -= scale;
			break;

		case Keys::Down:
			ResizingObject->Y += scale;
			break;

		case Keys::Left:
			ResizingObject->X -= scale;
			break;

		case Keys::Right:
			ResizingObject->X += scale;
			break;
		}
	}

	ResizingObject->GObject::Update();
	ResizingObject->Layout();
	WrapItem();
}

void GObjectResizer::OnKeyUp( ui32 keyID )
{
	if (keyID == Keys::Ctrl)
	{
		Ctrl = false;
	}

	if (keyID == Keys::Shift)
	{
		Shift = false;
	}
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
	SelectedItem = 0;
	SelectItemByClick= true;
}

void GGUICanvas::Update()
{
	this->GObject::Update();

	if (SelectedItem && SelectedItem->Object)
	{
		Resizer.ResizingObject = SelectedItem->Object;
		Resizer.Visible = true;
		Resizer.WrapItem();
	}
	else
	{
		Resizer.Visible = false;
	}

	if (RootNode)
	{
		TArrayEnumerator< GTreeNode* > ne(RootNode->Nodes);
		while(ne.MoveNext())
		{
			GGUIItem* curItem = (GGUIItem*)(ne.Current);
			OwnObject(curItem->Object);
			curItem->Object->Update();
		}
	}
}

GGUIItem* GGUICanvas::GetItemAt( int x, int y )
{
	if(!RootNode)
		return 0;

	GObject* found = 0;

	TArrayEnumerator< GTreeNode* > ne(RootNode->Nodes);
	while(ne.MoveNext())
	{
		GGUIItem* curItem = (GGUIItem*)(ne.Current);
		GObject* obj = curItem->Object;
		found = obj->FindObject();

		if (found)
		{
			return RootNode->FindObject(found);
		}
	}

	return 0;
}

void GGUICanvas::OnMouseDown( int x,int y, int button )
{
	if (SelectItemByClick)
	{
		if (!RootNode)
			return;

		GGUIItem* item = GetItemAt(x,y);
		if (!item)
		{
			item = RootNode;
			Resizer.ResizingObject = 0;
			Resizer.Visible = false;
		}

		item->Select(); // so event fires and this will set again

		GObject* obj = item->Object;
		if (obj)
		{
			IPosition mp(x,y);
			mp -= *obj;

			Resizer.SetFocus();
			Resizer.OnMouseDown(mp.X,mp.Y,button);
		}
	}
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
	PropertyView.Dock = DCK_FILL;

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

		ToolProperty.Panel2.AddChild(&PropertyView);

		PropertyView.SetPropertyObject(Engine.GUI.Fonts.Cache.Entries[0], &GFontEntry::MemberInfo);

		GObject* p = (GObject*)Parent;
		ViewTool.SplitterDistance = (int)((float)p->Width * 0.80f);
		ToolProperty.SplitterDistance = p->Height / 2;

		TArrayEnumerator<GObjectType*> ae(GObjectManager::Instance.Factories);
		while(ae.MoveNext())
		{
			GObjectType* objType = ae.Current;
			RGUIItemCreateButton* cbutton = new RGUIItemCreateButton(objType,this);
			cbutton->SetRectangle(0,0,16,16);
			Tools.AddChild(cbutton);
			cbutton->Layout();
		}

		Tools.Layout();

		CanvasTree.AfterSelect += GetHandler(this, &RGUIView::ItemSelected);

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

	Canvas.RootNode = (GGUIItem*)CanvasTree.RootNode;
}

void RGUIView::ItemSelected( void* sender, TreeViewEventArgs& e )
{
	Canvas.setSelectedItem((GGUIItem*)e.Node);
}

RGUIItemCreateButton::RGUIItemCreateButton( GObjectType* typ , RGUIView* viw)
{
	Type = typ;

	GImage* img = GetImage();
	if (img) Image.SetImage(*img);

	View = viw;
	
	Text = Type->ObjectName;
}

void RGUIItemCreateButton::Clicked( int x, int y, int button )
{
	if (!View->CanvasTree.SelectedNode)
	{
		return;
	}

	GGUIItem* newItem = new GGUIItem();
	newItem->Object = Type->CreateObject();
	
	newItem->Object->SetRectangle(100,100,100,100); // LOL make this drag and droppable?
	
	newItem->Text = Type->ObjectName;
	newItem->Object->Text = Type->ObjectName;

	GGUIItem* parentItem = (GGUIItem*)View->CanvasTree.SelectedNode;

	if (View->CanvasTree.SelectedNode != View->CanvasTree.RootNode)
	{
		parentItem->Object->AddChild(newItem->Object);
	}

	GImage* img = GetImage();
	if (img)
	{
		newItem->Image = img;
	}

	parentItem->AddNode(newItem);

	newItem->EnsureVisible();
}

GImage* RGUIItemCreateButton::GetImage()
{
	GImage* img = (GImage*)Type->GetExtensionPtr("ToolboxIcon");
	if (!img)
	{
		img = Resources.UIUnknown;
	}
	return img;
}
