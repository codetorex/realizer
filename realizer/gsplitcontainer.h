#ifndef GSPLITCONTAINER_H
#define GSPLITCONTAINER_H

#include "gobject.h"

enum GSplitView
{
	GSV_ALL,
	GSV_PANEL1ONLY,
	GSV_PANEL2ONLY,
};

class GSplitContainer: public GObject
{
private:
	bool Dragging;
	int DragDistance;
	IPosition DragPoint;

public:
	GSplitContainer();

	void OnMouseDown(int x,int y, int button);
	void OnMouseUp(int x,int y,int button);

	GObject Panel1;
	int Panel1MinSize;

	GObject Panel2;
	int Panel2MinSize;

	GSplitView View;

	int SplitterWidth;
	int SplitterDistance;
	GOrientation Orientation;
	bool AlwaysLayout;

	void setView(GSplitView newView);

	void Layout();
	void Update();
	void Render();
};

#endif