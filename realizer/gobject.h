#ifndef GOBJECT_H
#define GOBJECT_H

#include <tevent.h>
#include <tlinkedlist.h>
#include <tstring.h>
#include "genums.h"
#include "gskin.h"
#include "tenumerator.h"
#include "glayout.h"
#include "tboxmodel.h"

class GFont;
class VGUI;
class GLayout;

class KeyEventArgs
{
public:
	KeyEventArgs()
	{
		Handled = 0;
	}

	TChar KeyChar;
	bool Handled;
};

class GObject: public TListNode< GObject* >, public TList< GObject* >, public TBoxModel
{
public:
	typedef delegate0<void>						NoArgEvent;
	typedef delegate3<void,int,int,int>			MouseEvent;
	typedef delegate2<void,void*,KeyEventArgs&>	KeyEvent;

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
		TextAlign = CA_TopLeft;
		Dock = DCK_NODOCK;
		Layouter = &GLayout::Instance;
		Initialized = false;
	}

	/*TRegion DrawRegion;
	TRectangle Content; // similar to client area*/

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

	virtual void OnMouseEnter() {};
	virtual void OnMouseExit() {};
	virtual void OnMouseWheel(int x,int y, int delta) {};
	virtual void OnMouseDown(int x,int y, int button) {};
	virtual void OnMouseUp(int x,int y,int button) {};
	virtual void OnMouseMove(int x,int y) {};
	virtual void OnGotFocus() {};
	virtual void OnLostFocus() {};
	virtual void OnKeyPress(ui32 keyID) {};
	virtual void OnKeyDown(ui32 keyID) {};
	virtual void OnKeyUp(ui32 keyID) {};

	bool	Initialized;

	bool	Focused;
	bool	Enabled;
	bool	Visible;

	bool	MouseInside;
	bool	MouseEntered;
	bool	MousePressed;

	VGUI*	Master;
	GSkin*	Skin;
	GFont*	Font;
	String	Tooltip;
	GUIClassID	ClassID;
	DockType Dock;

	String	Text;
	Alignment TextAlign;
	int	TextPixelWidth;
	TColor32 ForeColor;

	GLayout* Layouter;

	inline bool IsActive()
	{
		return Parent->LastItem == this;
	}

	/**
	 * Called only once after this object became child of other object.
	 * ( which is the time its skin, master and other information were set )
	 */
	inline virtual void Initialize() 
	{ 
		Layout(); 
	};
	
	inline virtual void Update()
	{
		GObject* p = (GObject*)Parent;
		//DrawRegion.SetSize(p->DrawRegion.X + X + p->Content.X,p->DrawRegion.Y + Y + p->Content.Y,Width,Height);

		UpdateDrawRegion(*p);

		GObject* curObj = FirstItem;
		int i = ItemCount;
		while(i--)
		{
			if (curObj->Visible)
			{
				curObj->Update();
			}
			curObj = curObj->NextItem;
		}
	}

	inline virtual void Render()
	{
		GObject* curObj = FirstItem;
		int i = ItemCount;
		while(i--)
		{
			if (curObj->Visible)
			{
				curObj->Render();
			}
			curObj = curObj->NextItem;
		}
	}

	inline virtual void Layout() 
	{ 
		UpdateContent();
		if (Layouter)
		{
			Layouter->Layout(this,false);
		}
		else
		{
			LayoutChilds(); // which is unlikely
		}
	}; // done when resize happens

	void LayoutChilds()
	{
		TLinkedListEnumerator< GObject* > CmdMgr(this);

		while(CmdMgr.MoveNext())
		{
			CmdMgr.Current->Layout();
		}
	}

	void ActivateObject(GObject* obj); // makes this object last object.

	/**
	 * Activates all objects from this object through master parent.
	 * Good for making window on top.
	 */
	void ActivateRoot();

	/*inline void SetParent(GObject* obj)
	{
		obj->Parent = this;
		obj->Master = Master;
		obj->Skin = Skin;
		if (obj->Font == 0)
		{
			obj->Font = Font;
			obj->ForeColor = ForeColor;
		}
		obj->Initialize();
	}*/

	/**
	 * Adopts the objects as child.
	 */
	inline void OwnObject(GObject* newChild)
	{
		newChild->SetParent(this);
	}

	/**
	 * Sets parent of this object.
	 */
	inline void SetParent(GObject* newParent)
	{
		Parent = newParent;
		Master = newParent->Master;
		Skin = newParent->Skin;
		if (Font == 0)
		{
			Font = newParent->Font;
			ForeColor = newParent->ForeColor;
		}

		if (!Initialized)
		{
			Initialize();
			Initialized = true;
		}
	}

	inline void AddChild(GObject* obj)
	{
		Add(obj);
		OwnObject(obj);
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
	* Basically Chain of responsibility pattern used for mouse events.
	*/
	virtual GObject* FindObject();

	/**
	* Doesn't have any relation with VGUI. Only for tool and design usage.
	*/
	GObject* FindObjectByCoord(int x,int y);
};

typedef TLinkedListEnumerator<GObject*> GObjectEnumerator;

#include "gobjectmanager.h"

#endif