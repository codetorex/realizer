#include "stdafx.h"
#include "gtreeview.h"
#include "gfont.h"
#include "cengine.h"


GTreeView::GTreeView()
{
	ClassID = GTREEVIEW_CLASSID;
	SelectedNode = 0;
	CheckBoxes = false;

	RootNode = new GTreeNode();
	RootNode->TreeView = this;
	RootNode->Text = "Root"; // TODO: change this

	VBar.SetRectangle(0,0,100,100);
	VBar.Dock = DCK_RIGHT;

	ShowRoot = false;

	EditDropDown.Edited += GetHandler(this, &GTreeView::EditDropDown_Edited);
	EditingNode = 0;
}

void GTreeView::Render()
{
	Skin->RenderSunkEdge(this);

	Engine.Draw.PreTranslate( (float)DrawRegion.X(), (float)DrawRegion.Y(), 0.0f );

	IRectangle clipRect (Content);
	clipRect += DrawRegion;

	Engine.Draw.SetClip(clipRect); // CURRENTLY DISABLED FOR DEBUG PURPOSES

	drawX = 0;
	drawY = -(VBar.Value % NodeHeight);

	// lets test it first
	//RenderNode(&RootNode,startX,startY);
	// 
	// 

	TArrayEnumerator< GTreeNode* > rn(RenderNodes);
	while(rn.MoveNext())
	{
		Skin->RenderTreeNode(rn.Current,drawX,drawY);
		drawY += NodeHeight;
		//RenderNode(rn.Current);
	}	


	Engine.Draw.ResetTranslation();

	this->GObject::Render();

	Engine.Draw.ResetClip();
}

void GTreeView::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&VBar);
		VBar.Value = 0;
	}

	OwnObject(&VBar);
	OwnObject(&EditDropDown);
	Skin->LayoutSunkEdge(this);
	UpdateContent();

	NodeHeight = 16 + 4; // 2 px from top 2 px from bottom
	//ViewHeight = RootNode.GetHeight();
	VBar.LargeChange = Content.Height;
	TextYOffset = (NodeHeight - Font->Height) / 2;
	
	if (ViewHeight > Content.Height)
	{
		VBar.SmallChange = NodeHeight;
		VBar.MaxValue = ViewHeight;
		VBar.Visible = true;
		GLayout::Instance.Layout(this,false);
		VBar.Layout();
	}
	else
	{
		VBar.Value = 0;
		VBar.Visible = false;
	}
}

void GTreeView::UpdateRenderNode( GTreeNode* nd )
{
	if (!(!ShowRoot && nd == RootNode))
	{
		updateY += NodeHeight;
		if (updateY > 0 && updateY < (Content.Height + NodeHeight))
		{
			RenderNodes.Add(nd);
			nd->IsVisible = true;
			if (moY < updateY && moY > (updateY - NodeHeight))
			{
				nd->MouseOver = true;
			}
			else
			{
				nd->MouseOver = false;
			}
		}
	}

	if (nd->Expanded)
	{
		TArrayEnumerator< GTreeNode* > ae(nd->Nodes);
		while(ae.MoveNext())
		{
			UpdateRenderNode(ae.Current);
		}
	}
}

void GTreeView::Update()
{
	GObject::Update();

	/// Updates render list when required.
	//if (UpdateRender)
	{
		TArrayEnumerator< GTreeNode* > ae(RenderNodes);
		while(ae.MoveNext())
		{
			ae.Current->IsVisible = false;
		}
		RenderNodes.Clear();
		updateY = -VBar.Value;
		int updateYStart = updateY;
		UpdateRenderNode(RootNode);
		int updateYDiff = updateY - updateYStart;
		if (updateYDiff != ViewHeight)
		{
			ViewHeight = updateYDiff;
			Layout();

			// TODO: fix scrollbar 1 px bug
			// TODO: some bug on text rendering. clips them when their y goes negative ?
		}
		UpdateRender = false;
	}
}

void GTreeView::OnMouseUp( int x,int y,int button )
{
	GTreeNode* cNode = GetNodeAt(x,y);
	if (!cNode)
	{
		return;
	}

	if (button == 0)
	{
		if (SelectedNode == cNode)
		{
			cNode->Expanded = !cNode->Expanded;
			Layout();
		}
	}

	SelectedNode = cNode;
}

GTreeNode* GTreeView::GetNodeAt( int x, int y )
{
	int my = -(VBar.Value % NodeHeight);

	TArrayEnumerator< GTreeNode* > ae(RenderNodes);
	while(ae.MoveNext())
	{
		my += NodeHeight;
		if (y < my)
		{
			return ae.Current;
		}
	}
	return 0;
}

void GTreeView::OnMouseWheel( int x,int y, int delta )
{
	if (VBar.Visible)
	{
		VBar.OnMouseWheel(x,y,delta);
	}
}

void GTreeView::OnMouseMove( int x,int y )
{
	moX = x;
	moY = y;
}



void GTreeNode::AddNode( GTreeNode* node )
{
	node->Index = Nodes.Count;
	Nodes.Add(node);
	node->Parent = this;
	node->TreeView = TreeView;
	node->Level = Level + 1;
}

GTreeNode* GTreeNode::AddNode( const TString& nodeText, ui32 image, ui32 selectedImage )
{
	GTreeNode* newNode = new GTreeNode();
	newNode->Text = nodeText;

	if (TreeView->ImageList)
	{
		newNode->Image = &TreeView->ImageList->GetImage(image);
		newNode->SelectedImage = &TreeView->ImageList->GetImage(selectedImage);
	}

	AddNode(newNode);
	return newNode;
}

GTreeNode* GTreeNode::AddNode( const TString& nodeText )
{
	return AddNode(nodeText, TreeView->ImageIndex, TreeView->ImageIndex);
}

ui32 GTreeNode::GetHeight()
{
	ui32 result = TreeView->NodeHeight;
	if (Nodes.Count == 0)
		return result;
	
	result += Nodes.Count * TreeView->NodeHeight;
	
	TArrayEnumerator< GTreeNode* > nd(Nodes);
	while(nd.MoveNext())
	{
		if (nd.Current->Expanded)
		{
			result += nd.Current->GetHeight();
		}
	}

	return result;
}

IPosition GTreeNode::GetTextPosition()
{
	return TextPosition;
	/*if (!IsVisible)
	{
		return IPosition(0,0);
	}


*/

	/*IPosition result;
	result.X =  + (16 * (TreeView->ShowRoot ? Level : Level - 1));
	if (Image)
	{
		result.X += Image->Width + 5;
	}

	result.Y = -(TreeView->VBar.Value % TreeView->NodeHeight);
	int ItemIndex = TreeView->RenderNodes.IndexOf(this);
	if (ItemIndex == -1)
	{
		throw Exception("wtf");
	}
	result.Y += ItemIndex * TreeView->NodeHeight;
	result.Y += (TreeView->NodeHeight - TreeView->Font->Height) / 2;

	result += TreeView->Content;

	return result;*/
}

void GTreeView::EditDropDown_Edited()
{
	NodeLabelEditEventArgs e;
	e.Node = EditingNode;
	e.Label = EditDropDown.EditBox.get_Text();

	AfterLabelEdit.call(this, e);
	EditingNode = 0;

	if (e.CancelEdit)
	{
		return;
	}

	e.Node->Text = e.Label;
	EditDropDown.Hide();
	SetFocus();
}

void GTreeNode::BeginEdit()
{
	if (TreeView->EditingNode)
	{
		return; // ALREADY EDÝTÝNG SOMETHING ELSE
	}

	EnsureVisible();

	TreeView->EditingNode = this;
	IPosition textPos = GetTextPosition();
	int editWidth = TreeView->Width - (textPos.X) - 10;
	textPos += TreeView->DrawRegion;
	TreeView->EditDropDown.Show(textPos.X,textPos.Y,editWidth);
	TreeView->EditDropDown.SetText(Text);
	TreeView->EditDropDown.SelectAll();
}

void GTreeNode::EnsureVisible()
{
	GTreeNode* p = Parent;
	while(p)
	{
		p->Expanded = true;
		p = p->Parent;
	}

	// TODO: calculate position of this shit and move treeview scrollbar accordignly

	/**
	 * WELL THIS IS A HACKY WAY TO GET TEXT POSITION :(
	 * AND I AM TOO LAZY TO FIX GetTextPosition
	 * But WHO Cares?
	 */
	TreeView->Update();
	TreeView->Render();
}