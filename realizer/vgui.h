#ifndef VGUI_H
#define VGUI_H

#include "gobject.h"
#include "gdesktop.h"
#include "tkeyboard.h"
#include "tmouse.h"

#include "gskinmanager.h"
#include "gfontmanager.h"

class REngine;

class VGUI: public IMouseObserver, public IKeyboardObserver
{
public:	
	GFontManager Fonts;
	GSkinManager Skins;

	// A monospaced font, will be used for rendering of simple stuff.
	GFont* ConsoleFont;

	VGUI()
	{
		Desktop = 0;
		Focused = 0;
		Enabled = false;
		ConsoleFont = 0;
	}

	int X;
	int Y;
	bool ButtonState[5];

	/// true means active desktop will capture the input
	bool Enabled;

	REngine* Parent;
	GDesktop* Desktop;

	GObject* Focused;

	void ActivateDesktop(GDesktop* newDesktop);

	void InitializeDesktop(GDesktop* _desktop);

	GDesktop* CreateDesktop(bool activate)
	{
		GDesktop* newDesktop = new GDesktop();
		newDesktop->Master = this;
		if (activate)
		{
			ActivateDesktop(newDesktop);
		}
		else
		{
			InitializeDesktop(newDesktop);
		}
		return newDesktop;
	}

	void MouseMove(int x,int y)
	{
		X = x;
		Y = y;
	}

	void MouseDown(int x,int y, int button)
	{
		X = x;
		Y = y;
		ButtonState[button] = true;

		GObject* obj = Desktop->FindObject();
		if (obj)
		{
			obj->SetFocus();
			obj->MouseDown(x - obj->ScreenRegion.X, y - obj->ScreenRegion.Y, button);
			obj->ActivateRoot();
		}
	}

	void MouseUp(int x,int y, int button)
	{
		X = x;
		Y = y;
		ButtonState[button] = false;

		Focused->MouseUp(x - Focused->ScreenRegion.X,y - Focused->ScreenRegion.Y,button);
	}

	void MouseWheel(int x,int y,int delta)
	{
		X = x;
		Y = y;
		GObject* obj = Desktop->FindObject();
		if (obj)
		{
			obj->MouseWheel(x - obj->ScreenRegion.X,y - obj->ScreenRegion.Y,delta);
		}
	}

	void KeyDown(int KeyID)
	{
		Focused->KeyDown(KeyID);
	}

	void KeyUp(int keyID)
	{
		Focused->KeyUp(keyID);
	}

	void KeyUnicode(ch16 keyChar)
	{
		Focused->KeyPress(keyChar);
	}

	/**
	* Activates this as MouseObserver and KeyboardObserver.
	* @param defSkin if desktopObj is going to be created, this param will be its skin
	* @param desktopObj activates this desktop, if 0 then checks if no desktop then creates new and activates it.
	*/
	void EnableGUI(GSkin* defSkin = 0,GDesktop* desktopObj = 0);

	/**
	* Removes this from MouseObservation and KeyboardObservation.
	*/
	void DisableGUI();

	inline void RenderDesktop(GDesktop* dsktp)
	{
		dsktp->MouseInside = true;
		dsktp->Update();

		GObject* obj = dsktp->FindObject();
		if (obj)
		{
			obj->MouseMove(X - obj->ScreenRegion.X, Y - obj->ScreenRegion.Y);
		}

		dsktp->Render();
	}

	inline void Render()
	{
		RenderDesktop(Desktop);
	}
};


#endif