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
protected:
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

	void setValue(int newValue);

	void Clicked(int x, int y, int button);

	// event<IntEvent> OnValueChange;

	void Render();

	void Layout();
};

/**
 * Tools for calculating drag stuff about scrollbar.
 * Kept outside of scrollbar class to not make it look like shit.
 */
class GScrollbarLayout
{
public:
	GScrollBar& base;

	int TotalValues;
	int DragRange;
	int DragSize;
	int DragPosition;
	int RemainingSpace;
	float PixelPerItem;
	bool Vertical;

	inline GScrollbarLayout(GScrollBar& sb): base(sb)
	{
		Vertical = base.Orientation == SBO_VERTICAL ? true : false;
	}

	inline void Calculate(bool calculateDragSize = true)
	{

		
		TotalValues = base.MaxValue - base.MinValue;
		if (Vertical)
		{
			DragRange = base.DownButton->Top - base.UpButton->Bottom;
		}
		else
		{
			DragRange = base.DownButton->Left - base.UpButton->Right;
		}
		/////////////////////////////////////////////////////////
		
		// TODO: total values should be something like - LargeChange if we consider LargeChange as size of dragbar
		// Or we should do this in master application by setting maxValue = availableValues - screenSpace like thing

		// IMPLEMENT NEW TECHNIQUES AND IDEAS YOU ACQUIRED FROM ORIGINAL IMPLEMENTATION


		if (calculateDragSize)
		{
			DragSize = ((base.LargeChange * DragRange) / TotalValues);
		}
		else
		{
			DragSize = GetDragSize();
		}
		RemainingSpace = DragRange - DragSize;
		PixelPerItem = (float)RemainingSpace / (float)(TotalValues - base.LargeChange);
	}

	inline void CalculateDragPosition()
	{
		DragPosition = (int)((float)(base.Value - base.MinValue) * PixelPerItem);
	}

	inline void CalculateValue()
	{
		int cDragPos = GetDragPos();
		int cValue = ((float)cDragPos / PixelPerItem);
		cValue -= base.MinValue;
		base.setValue(cValue);
	}

	inline void SetDragPos()
	{
		if (Vertical)
		{
			base.DragBar->SetTop( base.UpButton->Bottom + DragPosition );
		}
		else
		{
			base.DragBar->SetLeft( base.UpButton->Right + DragPosition );
		}
	}

	inline void SetDragSize()
	{
		if (Vertical)
		{
			base.DragBar->SetHeight(DragSize); 
		}
		else
		{
			base.DragBar->SetWidth(DragSize);
		}
	}

	inline int GetDragSize()
	{
		if (Vertical)
		{
			return base.DragBar->Height;
		}
		else
		{
			return base.DragBar->Width;
		}
	}

	inline int GetDragPos()
	{
		if (Vertical)
		{
			return base.DragBar->Top - base.UpButton->Bottom;
		}
		else
		{
			return base.DragBar->Left - base.UpButton->Right;
		}
	}
};


#endif