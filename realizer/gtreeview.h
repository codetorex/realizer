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
	bool MouseOver;

	/**
	 * So nodes can hold other objects.
	 */
	void* Tag;

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

	bool UpdateRender;

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

	/**
	 * Finds node using RenderNodes
	 */
	GTreeNode* FindNode(int x, int y);

	friend class GTreeNode;

public:
	GTreeView();

	//GTreeNode* MouseOverNode;
	GTreeNode* SelectedNode;
	GTreeNode RootNode;
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

	
	void MouseWheel(int x,int y, int delta);
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);


	void Render();
	void Layout();
	void Update();

};

#endif