#ifndef GMENUITEM_H
#define GMENUITEM_H

#include "gobject.h"
#include "gimage.h"
#include "gdropdown.h"
#include "gtimer.h"

class GMenuItem;

class GDropDownTimer: public GTimeEffectSingle
{
public:
	GMenuItem* Parent;

	void Initialize(GMenuItem* p,int delay_)
	{
		this->Parent = p;
		this->Delay = delay_;
	}

	void Activate();
};

class GMenuItem: public GObject
{
private:
	bool Seperator;
	int DrawStyle;

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

	GDropDownTimer ShowTimer;

	void HideRelativeMenus();

	void ShowSubMenu();
	void HideParentMenu();

	friend class GDropDownTimer;

public:

	GMenuItem();

	CheckStates CheckState;

	GDropDown SubItems;

	int GraphicState;

	event<NoArgEvent> Click;

	void MouseEnter();
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