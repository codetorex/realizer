#ifndef GMENUITEM_H
#define GMENUITEM_H

#include "gobject.h"
#include "gimage.h"

class GMenuItem: public GObject
{
private:
	bool Seperator;
	int DrawStyle;
	bool DropDownChild;

	inline bool IsParentDropDown()
	{
		return (((GObject*)Parent)->ClassID == GDROPDOWN_CLASSID);
	}

	inline bool IsParentMenuStrip()
	{
		return (((GObject*)Parent)->ClassID == GMENUSTRIP_CLASSID);
	}

	int TextX; // left side will be used for icons
	int TextY;

public:

	GMenuItem();

	CheckStates CheckState;


	int GraphicState;

	event<NoArgEvent> Click;

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	void Render();
	void Layout();
	void Update();

	GImage Image;



	inline void set_Seperator( bool _seperator )
	{
		Seperator = true;
		GraphicState = 1;
	}

	inline bool get_Seperator()
	{
		return Seperator;
	}

	GMenuItem* AddSeperator();
};

#endif