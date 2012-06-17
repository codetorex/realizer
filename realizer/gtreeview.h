#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include "gobject.h"
#include "gscrollbar.h"
#include "gimagelist.h"
#include "tenumerator.h"
#include "gdropdowntext.h"

class GTreeNode;
class GTreeView;

class NodeLabelEditEventArgs
{
public:
	NodeLabelEditEventArgs()
	{
		CancelEdit = false;
	}

	GTreeNode* Node;
	TString Label;
	bool CancelEdit;
};

class TreeViewEventArgs
{
public:
	TreeViewEventArgs()
	{
		Node = 0;
		Action = 0;
	}

	ui32 Action;
	GTreeNode* Node;
};

/**
 * TODO: derive this from GObject?
 */
class GTreeNode
{
public:
	GTreeNode()
	{
		Parent = 0;
		Expanded = false;
		TreeView = 0;
		Image = 0;
		SelectedImage = 0;
		Level = 0;
	}

	~GTreeNode()
	{
		Nodes.DeletePointers(); // this is correct right?
	}

	void AddNode(GTreeNode* node);

	GTreeNode* AddNode(const TString& nodeText, ui32 image, ui32 selectedImage);

	GTreeNode* AddNode(const TString& nodeText);

	virtual void Clicked() { };

	GTreeNode* Parent;
	TString Text;
	TArray< GTreeNode* > Nodes;
	bool Expanded;
	bool MouseOver;

	bool IsVisible;

	/**
	 * So nodes can hold other objects.
	 */
	void* Tag;

	ui32 Index;
	ui32 Level;

	GImage* Image;
	GImage* SelectedImage;

	GTreeView* TreeView;

	IPosition TextPosition; // useful

	/**
	 * Returns text position.
	 */
	IPosition GetTextPosition();

	void BeginEdit();
	void EndEdit(bool cancel);

	void EnsureVisible();

	void SetTreeViewForAllChilds();

	void Select();


	inline GTreeNode* NextNode()
	{
		if (Parent)
		{
			if (Parent->Nodes.Count > (Index + 1))
			{
				Parent->Nodes[Index + 1];
			}
		}

		return 0;
	}

	void UpdateChildIndexes()
	{
		for (ui32 i=0;i<Nodes.Count;i++)
		{
			Nodes[i]->Index = i;
		}
	}

	/**
	 * Recursively finds required height in pixels.
	 */
	ui32 GetHeight();
};

// TODO: make a gtreeview renderer class

class GTreeView: public GObject
{
private:
	// vertical scrollbar
	GScrollBar VBar;

	bool UpdateRender;

	GTreeNode* EditingNode;

	TArray< GTreeNode* > RenderNodes;

	
	ui32 ScreenSpace;
	ui32 ViewHeight;

	int drawX;
	int drawY;

	int moX, moY;

	int updateY;

	/**
	 * Recursively creates a rendering list of visible nodes
	 */
	void UpdateRenderNode(GTreeNode* nd);

	friend class GTreeNode;

public:
	typedef delegate2<void, void*, NodeLabelEditEventArgs& > NodeLabelEditEvent;
	typedef delegate2<void, void*, TreeViewEventArgs& > TreeViewEvent;

	GTreeView();

	event< NodeLabelEditEvent > AfterLabelEdit;
	event< TreeViewEvent > AfterSelect;

	/**
	 * This is used for editing texts of items.
	 */
	GDropDownText EditDropDown;

	void EditDropDown_Edited();

	//GTreeNode* MouseOverNode;
	GTreeNode* SelectedNode;
	GTreeNode* RootNode;
	bool CheckBoxes;

	/// Node height will be determined by font size
	ui32 NodeHeight;

	/// This will be used for making text appear middle of node
	ui32 TextYOffset;

	GImageList* ImageList;
	ui32 ImageIndex; // default index

	bool ShowRoot;
	bool ShowPlusMinus;
	bool ShowLines;

	
	void OnMouseWheel(int x,int y, int delta);
	void OnMouseUp(int x,int y,int button);
	void OnMouseMove(int x,int y);

	/**
	 * Finds node using RenderNodes
	 */
	GTreeNode* GetNodeAt(int x, int y);


	void Render();
	void Layout();
	void Update();

};

#endif