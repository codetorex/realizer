#include "stdafx.h"
#include "gtreeview.h"
#include "gfont.h"
#include "cengine.h"

/**
 * Enumerates through every visible node
 */
class GTreeNodeEnumerator: public TEnumerator< GTreeNode* >
{
public:
	GTreeNode* baseNode;
	int curIndex;

	GTreeNodeEnumerator(GTreeNode* baseNode)
	{
		this->baseNode = baseNode;
	}

	void Reset()
	{
		curIndex = -1; // self
		Current = baseNode;
	}

	bool MoveNext()
	{
		if (curIndex == -1)
		{
			curIndex = 0;
			return true;
		}

		if (curIndex == Current->Nodes.Count)
		{
			Current = Current->Parent->NextNode();
			if (Current)
			{

				return true;
			}
		}
		else
		{
			Current = Current->NextNode();
		}


	}
};



GTreeView::GTreeView()
{
	ClassID = GTREEVIEW_CLASSID;
	SelectedNode = 0;
	CheckBoxes = false;
	RootNode.TreeView = this;
	RootNode.Text = "Root"; // TODO: change this

	VBar.SetRectangle(0,0,100,100);
	VBar.Dock = DCK_RIGHT;

	ShowRoot = false;
}

void GTreeView::Render()
{
	Skin->RenderSunkEdge(this);

	Engine.Draw.PreTranslate( (float)DrawRegion.X(), (float)DrawRegion.Y(), 0.0f );

	drawX = 0;
	drawY = 0;

	// lets test it first
	//RenderNode(&RootNode,startX,startY);
	// 
	// 
	if (ShowRoot)
	{
		RenderNode(&RootNode);
	}
	else
	{
		RenderChildNodes(&RootNode);
	}

	Engine.Draw.ResetTranslation();
}

void GTreeView::RenderNode( GTreeNode* nd)
{
	int curX = drawX;

	if (nd->Image)
	{
		int imageY = ((int)NodeHeight - nd->Image->Height) / 2;
		nd->Image->Render(curX,drawY + imageY);
		curX += nd->Image->Width + 3;
	}

	Font->Render(nd->Text,curX,TextYOffset + drawY,ForeColor);
	drawY += NodeHeight;

	if (nd->Expanded)
	{
		drawX += 16;
		RenderChildNodes(nd);
		drawX -= 16;
	}
}

void GTreeView::RenderChildNodes( GTreeNode* nd)
{
	TArrayEnumerator< GTreeNode* > ae(nd->Nodes);
	while(ae.MoveNext())
	{
		RenderNode(ae.Current);
	}
}

void GTreeView::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&VBar);
	}

	OwnObject(&VBar);
	Skin->LayoutSunkEdge(this);
	UpdateContent();

	NodeHeight = 16 + 4; // 2 px from top 2 px from bottom
	ViewHeight = RootNode.GetHeight();
	VBar.LargeChange = Content.Height;
	TextYOffset = (NodeHeight - Font->Height) / 2;
	
	if (ViewHeight > Content.Height)
	{
		VBar.SmallChange = NodeHeight;
		VBar.MaxValue = ViewHeight/2;
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

void GTreeNode::AddNode( GTreeNode* node )
{
	Nodes.Add(node);
	node->Parent = this;
	node->TreeView = TreeView;
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