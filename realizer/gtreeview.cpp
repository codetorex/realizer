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

	IRectangle clipRect (Content);
	clipRect += DrawRegion;

	//Engine.Draw.SetClip(clipRect); CURRENTLY DISABLED FOR DEBUG PURPOSES

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

#include "gschemedskin.h"

void GTreeView::RenderNode( GTreeNode* nd )
{
	GSchemedSkin* ss = (GSchemedSkin*)Skin;
	Engine.Draw.SetTexture(ss->SkinTexture);

	int curX = drawX; // 0 this time

	int lineimg = 1;
	if (nd->Parent)
	{
		if (nd == nd->Parent->Nodes.GetLast())
		{
			lineimg = 2;
		}
		else
		{
			lineimg = 1;
		}

		int lvl = nd->Level-1;
		if (!ShowRoot)
		{
			lvl--;
		}

		while(lvl-- > 0)
		{
			ss->DotLine[0].Draw(curX,drawY);
			curX += 16;
		}
		bool isRootChild = nd->Parent == &RootNode;
		if ((isRootChild && ShowRoot) || !isRootChild)
		{
			ss->DotLine[lineimg].Draw(curX,drawY);
			curX += 16;
		}

	}

	if (nd->Image)
	{
		int imageY = ((int)NodeHeight - nd->Image->Height) / 2;
		nd->Image->Render(curX,drawY + imageY);
		curX += nd->Image->Width + 3;
	}

	Font->Render(nd->Text,curX,TextYOffset + drawY,ForeColor);
	drawY += NodeHeight;
}

/*void GTreeView::RenderNode( GTreeNode* nd)
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
}*/

void GTreeView::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&VBar);
		VBar.Value = 0;
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
	if (!(!ShowRoot && nd == &RootNode))
	{
		updateY += NodeHeight;
		if (updateY > 0 && updateY < (Content.Height + NodeHeight))
		{
			RenderNodes.Add(nd);
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
		RenderNodes.Clear();
		updateY = -VBar.Value;
		UpdateRenderNode(&RootNode);
		UpdateRender = false;
	}
}

void GTreeView::MouseUp( int x,int y,int button )
{
	GTreeNode* cNode = FindNode(x,y);
	if (!cNode)
	{
		return;
	}

	if (button == 0)
	{
		cNode->Expanded = !cNode->Expanded;
		Layout();
	}
	
}

GTreeNode* GTreeView::FindNode( int x, int y )
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

void GTreeView::MouseWheel( int x,int y, int delta )
{
	if (VBar.Visible)
	{
		VBar.MouseWheel(x,y,delta);
	}
}

void GTreeView::MouseMove( int x,int y )
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