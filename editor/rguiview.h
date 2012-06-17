#ifndef RGUIVIEW_H
#define RGUIVIEW_H

#include "rdocumentview.h"
#include "rdocument.h"
#include "rguidocument.h"

#include "gsplitcontainer.h"
#include "gtoolstrip.h"
#include "gtreeview.h"
#include "gpropertypage.h"

class GGUICanvas;

class GObjectResizerGrip: public GObject
{
protected:
	IPosition DragPos;
	bool Draging;

public:
	GObjectResizerGrip();

	void OnMouseDown(int x,int y, int button);

	void OnMouseUp(int x,int y,int button);

	void UpdateDrag();

	enum EGripType
	{
		GT_TOPLEFT,
		GT_TOP,
		GT_TOPRIGHT,
		GT_LEFT,
		GT_RIGHT,
		GT_BOTTOMLEFT,
		GT_BOTTOM,
		GT_BOTTOMRIGHT,
	};

	EGripType GripType;

	void Render();
	void Update();
};

class GObjectResizer: public GObject
{
protected:
	/// TODO : make dragging an tool object
	int GripSize;
	IPosition DragPos;
	IPosition DragObjectPos;
	

public:
	GObjectResizer();

	void OnMouseDown(int x,int y, int button);
	void OnMouseUp(int x,int y,int button);

	void UpdateDrag();

	// TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight
	GObjectResizerGrip Grips[8];

	inline int getGripSize()
	{
		return GripSize;
	}

	/// The object getting resized
	GObject* ResizingObject;

	bool Draging;

	/*void setResizingObject(GObject* rObj)
	{
		ResizingObject = rObj;
		if (rObj)
		{
			Visible = true;
			Layout();
		}
		else
		{
			Visible = false;
			// set visible to false?
		}
	}*/

	/// Wraps selected item of canvas
	void WrapItem();

	void setGripSize(int newGripSize);

	void Render();
	void Layout();
	void Update();
};

enum GGridStyle
{
	GS_DOTTED,
	GS_LINE,
};

/**
 * Allows you to place objects and move them around canvas
 */
class GGUICanvas: public GObject
{
private:
	GGUIItem* SelectedItem;

public:

	GGUICanvas();

	/// There should be only one re sizer
	GObjectResizer Resizer;

	bool DrawGrid;
	bool GridLines;
	int GridSpacing;

	GGUIItem* RootNode;

	void setSelectedItem(GGUIItem* item)
	{
		SelectedItem = item;
		
		if (SelectedItem && SelectedItem->Object)
		{
			Resizer.ResizingObject = item->Object;	
		}
		else
		{
			Resizer.Visible = false;
			Resizer.ResizingObject = 0;
		}
	}

	bool SelectItemByClick;

	void OnMouseDown(int x,int y, int button);

	GGUIItem* GetItemAt(int x, int y);

	GGUIItem* getSelectedItem()
	{
		return SelectedItem;
	}

	void Layout();

	void Render();
	void Update();
};

// TODO: implement property grid


class RGUIView;

class RGUIItemCreateButton: public GToolStripButton
{
public:
	RGUIView* View;
	GObjectType* Type;

	RGUIItemCreateButton(GObjectType* typ, RGUIView* viw);

	GImage* GetImage();

	void Clicked(int x, int y, int button);
};

/**
 * There should be toolbox on rightSide and document structure and properties on right side
 * For making it easier we can put everything on right side?
 * What about 3dsmax style floating stuff?
 */
class RGUIView: public RDocumentView
{
public:
	RGUIView();

	/// Splitter used for splitting view from tools
	GSplitContainer ViewTool;

	/// Splitter used for splitting tools from properties
	GSplitContainer ToolProperty;

	/// Where actual editing occurs
	GGUICanvas Canvas;

	GToolStrip Tools;

	GTreeView CanvasTree;

	GPropertyPage PropertyView;

	void DocumentChanged();


	inline RGUIDocument* GetGUIDocument()
	{
		return (RGUIDocument*)Document;
	}

	void ItemSelected(void* sender, TreeViewEventArgs& e);

	void Layout();
};

#endif