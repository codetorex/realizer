#ifndef GOBJECT_H
#define GOBJECT_H

#include <tevent.h>
#include <tlinkedlist.h>
#include <tstring.h>
#include "genums.h"
#include "gskin.h"
#include <tregion.h>
#include "tenumerator.h"
#include "glayout.h"

class GFont;
class VGUI;
class GLayout;

class GPadding
{
public:
	int Left;
	int Top;
	int Right;
	int Bottom;

	GPadding()
	{
		Set(0);
	}

	GPadding& operator += (const GPadding& other)
	{
		Left += other.Left;
		Top += other.Top;
		Right += other.Right;
		Bottom += other.Bottom;
		return *this;
	}

	inline void Set(int pad)
	{
		Left = pad;
		Top = pad;
		Right = pad;
		Bottom = pad;
	}
};

/**
 * Region defines total width, total height
 */
class GBoxModel: public TRegion
{
public:
	GPadding Margin; // margin around
	GPadding Padding; // client rect padding
	GPadding Border; // skin border

	/// Relative content rectangle
	TRectangle Content;

	/// Where actual draw and event handling occurs
	TRegion DrawRegion;

	/**
	 * Call this after Margin, Padding or Border changes.
	 */
	void UpdateContent()
	{
		GPadding sum = Margin;
		sum += Padding;
		sum += Border;

		Content.SetRectangle(sum.Left,sum.Top,Width - (sum.Right + sum.Left),Height - (sum.Top + sum.Bottom));
	}

	/**
	 * Updates draw region by parents draw region
	 * You should call this whenever greatest parent's draw region changes.
	 * @param p is parent
	 */
	void UpdateDrawRegion(GBoxModel& p)
	{
		int nX = p.DrawRegion.X + p.Content.X + X;
		int nY = p.DrawRegion.Y + p.Content.Y + Y;
		DrawRegion.SetSize(nX,nY,Width,Height);
	}
};

class GObject: public TListNode< GObject* >, public TList< GObject* >, public TRegion
{
public:
	typedef delegate0<void>				NoArgEvent;
	typedef delegate3<void,int,int,int> MouseEvent;
	typedef delegate1<void,ui32>		KeyEvent;

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
	}

	TRegion ScreenRegion;
	TRectangle ObjectRegion; // similar to client area

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
	virtual void KeyPress(ui32 keyID) {};
	virtual void KeyDown(ui32 keyID) {};
	virtual void KeyUp(ui32 keyID) {};

	bool	Focused;
	bool	Enabled;
	bool	Visible;

	bool	MouseInside;
	bool	MouseEntered;
	bool	MousePressed;

	VGUI*	Master;
	GSkin*	Skin;
	GFont*	Font;
	TString	Tooltip;
	GUIClassID	ClassID;
	DockType Dock;

	TString	Text;
	ContentAlignment TextAlign;
	int	TextPixelWidth;
	TColor32 ForeColor;

	GLayout* Layouter;

	inline bool IsActive()
	{
		return Parent->LastItem == this;
	}

	inline virtual void Initialize() 
	{ 
		Layout(); 
	};
	
	inline virtual void Update()
	{
		GObject* p = (GObject*)Parent;
		ScreenRegion.SetSize(p->ScreenRegion.X + X + p->ObjectRegion.X,p->ScreenRegion.Y + Y + p->ObjectRegion.Y,Width,Height);

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
		ObjectRegion.SetRectangle(0,0,Width,Height);
		LayoutChilds(); 
	}; // done when resize happens

	void LayoutChilds()
	{
		TLinkedListEnumerator< GObject* > c(this);

		while(c.MoveNext())
		{
			c.Current->Layout();
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
		Initialize();
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

#endif