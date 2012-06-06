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

	void OnMouseEnter();
	void OnMouseExit();
	void OnMouseUp(int x,int y,int button);
	void OnMouseMove(int x,int y);

	void Render();
	void Layout();
	void Update();

	GImage Image;

	void AddSubMenu(GMenuItem* item);
	GMenuItem* AddSubMenu(const TString& menuItem, NoArgEvent* onClick = 0);
	GMenuItem* AddSubMenu(const TString& menuItem, GImage& img, NoArgEvent* onClick = 0);


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