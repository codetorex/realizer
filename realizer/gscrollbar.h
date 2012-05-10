#ifndef GSCROLLBAR_H
#define GSCROLLBAR_H

#include "gbuttonbase.h"

class GScrollBarButton: public GButtonBase
{
public:
	enum SButtonDirection
	{
		BD_LEFT,
		BD_RIGHT,
		BD_UP,
		BD_DOWN,
		/*BD_HORIZONTALDOT, // these are not really used but loaded in schemed skin
		BD_VERTICALDOT,*/
	};

	GScrollBarButton();

	SButtonDirection Direction;
};

enum GScrollBarOrientation
{
	SBO_VERTICAL,
	SBO_HORIZONTAL,
};

class GScrollBarDrag: public GButtonBase
{
private:
	bool Dragging;
	vec2i DragPoint;
	int DragPos;

public:
	GScrollBarOrientation Orientation;

	GScrollBarDrag();

	void MouseDown(int x,int y, int button);
	void MouseUp(int x,int y,int button);

	void Update();

	void Clicked(int x, int y, int button);
};

class GScrollBar: public GButtonBase
{
private:
	friend class GScrollBarDrag;
	void SetValueFromDragPos();

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


	void Clicked(int x, int y, int button);

	// event<IntEvent> OnValueChange;

	void Render();

	void Layout();
};


#endif