#ifndef GDESKTOP_H
#define GDESKTOP_H

#include "gobject.h"

enum GDebugMode
{
	GDB_DISABLED,
	GDB_MOUSEON,
	GDB_ALL
};

class GDesktop: public GObject
{
private:
	inline void UpdateTopObjects()
	{
		TArrayEnumerator< GObject* > c(OnTopObjects);
		while(c.MoveNext())
		{
			c.Current->Update();
		}
	}

	inline void RenderTopObjects()
	{
		TArrayEnumerator< GObject* > c(OnTopObjects);
		while(c.MoveNext())
		{
			c.Current->Render();
		}
	}

public:

	GDesktop()
	{
		ClassID = GDESKTOP_CLASSID;
		MouseOn = this;
		DebugMode = GDB_DISABLED;
	}

	GDebugMode DebugMode;

	/**
	 * Disappearing objects after use but should stay on top.
	 * objects like menus, message boxes, combo box list things like that which is 
	 */
	TArray< GObject* > OnTopObjects;

	GObject* MouseOn;

	inline void MouseIsOn(GObject* obj)
	{
		if (MouseOn != obj)
		{
			MouseOn->MouseExit();
			MouseOn->MouseEntered = false;
		}

		MouseOn = obj;
		if ( !MouseOn->MouseEntered )
		{
			MouseOn->MouseEntered = true;
			MouseOn->MouseEnter();
		}
	}

	inline GObject* FindDesktopObject()
	{
		GObject* found;

		TArrayEnumerator< GObject* > c(OnTopObjects);
		while(c.MoveNext())
		{
			found = c.Current->FindObject();
			if (found != 0)
			{
				MouseIsOn(found);
				return found;
			}
		}

		found = FindObject();
		if (found)
		{
			MouseIsOn(found);
		}

		return found;
	}

	inline void SetTopObject(GObject* obj)
	{
		obj->Parent = this;
		OnTopObjects.Add(obj);
		obj->Update();
	}

	inline void UnsetTopObject(GObject* obj)
	{
		OnTopObjects.RemovePreserve(obj);
	}

	inline void Update()
	{
		UpdateTopObjects();
		this->GObject::Update();
	}

	void RenderDebug(GObject* obj);

	inline void Render()
	{
		this->GObject::Render();
		RenderTopObjects();

		if (DebugMode == GDB_MOUSEON)
		{
			RenderDebug(MouseOn);
		}
		else if (DebugMode == GDB_ALL)
		{
			// NOT IMPLEMENTED YET LOL
		}
	}
};

#endif