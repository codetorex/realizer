#ifndef GOBJECT_H
#define GOBJECT_H

#include <tevent.h>
#include <tlinkedlist.h>
#include <tstring.h>
#include "genums.h"
#include "gskin.h"
#include <tregion.h>

class GFont;
class VGUI;

class GObject: public TListNode< GObject >, public TList< GObject >, public TRegion
{
public:
	typedef delegate0<void>				NoArgEvent;
	typedef delegate3<void,int,int,int> MouseEvent;
	typedef delegate1<void,dword>		KeyEvent;

	GObject()
	{
		Visible = true;
		Focused = false;
		Enabled = true;
		MouseInside = false;
		MouseEntered = false;
		MousePressed = false;
		ClassID = GOBJECT_CLASSID;
		Parent = 0;
		Font = 0;
		Active = false;
		TextAlign = CA_TopRight;
	}

	TRegion ScreenRegion;

	/*event<NoArgEvent>	MouseEnter;
	event<NoArgEvent>	MouseExit;
	event<MouseEvent>	MouseWheel;
	event<MouseEvent>	MouseDown;
	event<MouseEvent>	MouseUp;
	event<MouseEvent>	MouseMove;
	event<NoArgEvent>	GotFocus;
	event<NoArgEvent>	LostFocus;
	event<KeyEvent>		KeyPress;
	event<KeyEvent>		KeyDown;
	event<KeyEvent>		KeyUp;*/

	virtual void MouseEnter() {};
	virtual void MouseExit() {};
	virtual void MouseWheel(int x,int y, int delta) {};
	virtual void MouseDown(int x,int y, int button) {};
	virtual void MouseUp(int x,int y,int button) {};
	virtual void MouseMove(int x,int y) {};
	virtual void GotFocus() {};
	virtual void LostFocus() {};
	virtual void KeyPress(dword keyID) {};
	virtual void KeyDown(dword keyID) {};
	virtual void KeyUp(dword keyID) {};

	bool	Focused;
	bool	Enabled;
	bool	Visible;
	bool	Active;

	bool	MouseInside;
	bool	MouseEntered;
	bool	MousePressed;

	VGUI*	Master;
	GSkin*	Skin;
	GFont*	Font;
	TString	Tooltip;
	dword	ClassID;

	TString	Text;
	ContentAlignment TextAlign;
	int	TextPixelWidth;
	TColor32 ForeColor;

	virtual void Initialize() { Layout(); };
	virtual void Update();
	virtual void Render();
	virtual void Layout() {}; // done when resize happens

	void Dock(DockType dock);

	void ActivateObject(GObject* obj); // makes this object last object.

	inline void AddChild(GObject* obj)
	{
		Add(obj);
		obj->Master = Master;
		obj->Skin = Skin;
		if (obj->Font == 0)
		{
			obj->Font = Font;
			obj->ForeColor = ForeColor;
		}
		obj->Initialize();
	}

	inline void RemoveChild(GObject* obj)
	{
		Remove(obj);
	}

	void SetFocus();

	void SetMouseInside(bool inside);
	
	/**
	* Checks for mouse enter and exit events.
	* Optimizes with using mouseInside information.
	* Takes parameters from Master VGUI.
	*/
	GObject* FindObject();

	/**
	* Doesn't have any relation with VGUI. Only for tool and design usage.
	*/
	GObject* FindObject(int x,int y);
};

#endif