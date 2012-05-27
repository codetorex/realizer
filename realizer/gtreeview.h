#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include "gobject.h"
#include "gscrollbar.h"
#include "gimagelist.h"
#include "tenumerator.h"

class GTreeNode;
class GTreeView;

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

	void AddNode(GTreeNode* node);

	GTreeNode* AddNode(const TString& nodeText, ui32 image, ui32 selectedImage);

	GTreeNode* AddNode(const TString& nodeText);

	virtual void Clicked() { };

	GTreeNode* Parent;
	TString Text;
	TArray< GTreeNode* > Nodes;
	bool Expanded;

	ui32 Index;
	ui32 Level;

	GImage* Image;
	GImage* SelectedImage;

	GTreeView* TreeView;

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

	ui32 NodeHeight;
	ui32 ScreenSpace;
	ui32 ViewHeight;
	ui32 TextYOffset;

	int drawX;
	int drawY;

	void RenderNode(GTreeNode* nd);
	void RenderChildNodes(GTreeNode* nd);

	friend class GTreeNode;

public:
	GTreeView();

	GTreeNode* SelectedNode;
	GTreeNode RootNode;
	bool CheckBoxes;

	GImageList* ImageList;
	ui32 ImageIndex; // default index

	bool ShowRoot;
	bool ShowPlusMinus;
	bool ShowLines;


	void Render();
	void Layout();

};

#endif