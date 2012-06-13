#ifndef RGUIVIEW_H
#define RGUIVIEW_H

#include "rdocumentview.h"
#include "rdocument.h"
#include "rguidocument.h"

#include "gsplitcontainer.h"
#include "gtoolstrip.h"
#include "gtreeview.h"

class GObjectResizerGrip: public GObject
{
public:
	void Render();
};

class GObjectResizer: public GObject
{
protected:
	int GripSize;

public:
	GObjectResizer();

	// TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight
	GObjectResizerGrip Grips[8];

	inline int getGripSize()
	{
		return GripSize;
	}

	void setGripSize(int newGripSize);

	void Render();
	void Layout();
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
public:

	GGUICanvas();

	/// There should be only one re sizer
	GObjectResizer Resizer;

	bool DrawGrid;
	bool GridLines;
	int GridSpacing;

	GGUIItem* RootNode;

	void Layout();

	void Render();
};

// TODO: implement property grid



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

	void DocumentChanged();


	inline RGUIDocument* GetGUIDocument()
	{
		return (RGUIDocument*)Document;
	}

	void Layout();


	/// VIEW COMMANDS COMES HERE

	void CreateButton();
};

#endif