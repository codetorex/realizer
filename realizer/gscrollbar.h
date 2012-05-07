#ifndef GSCROLLBAR_H
#define GSCROLLBAR_H

#include "gobject.h"

class GScrollBarButton: public GObject
{
public:
	enum SButtonDirection
	{
		BD_LEFT,
		BD_RIGHT,
		BD_UP,
		BD_DOWN,
	};

	GScrollBarButton();

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	virtual void Clicked() = 0;

	SButtonDirection Direction;
	int GraphicState;
};

enum GScrollBarOrientation
{
	SBO_VERTICAL,
	SBO_HORIZONTAL,
};

class GScrollBarDrag: public GObject
{
public:
	GScrollBarOrientation Orientation;

	GScrollBarDrag();

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	int GraphicState;
};

class GScrollBar: public GObject
{
public:
	GScrollBarButton* UpButton;
	GScrollBarButton* DownButton;
	GScrollBarDrag* DragBar;

public:
	GScrollBar();

	int MinValue;
	int MaxValue;

	int Value;

	int SmallChange;
	int LargeChange;
	
	GScrollBarOrientation Orientation;

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	// event<IntEvent> OnValueChange;

	void Render();

	void Layout();

	int GraphicState;
};


#endif