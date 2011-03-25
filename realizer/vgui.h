#ifndef VGUI_H
#define VGUI_H

#include "gobject.h"
#include "cinputinterface.h"

class REngine;

class VGUI: public IMouseObserver, public IKeyboardObserver
{
public:
	TArray< GSkin* > Skins;
	TArray< GFont* > Fonts;


	bool EventScissor;
	TRegion Scissor;

	int X;
	int Y;
	bool ButtonState[5];

	REngine* Parent;
	GObject* Desktop;

	GObject* Focused;

	void ActivateDesktop(GObject* newDesktop);
	GObject* CreateDesktop(bool activate)
	{
		GObject* newDesktop = new GObject();
		if (activate)
		{
			ActivateDesktop(newDesktop);
		}
		return newDesktop;
	}

	void MouseMove(int x,int y)
	{
		X = x;
		Y = y;
		
		/*GObject* obj = Desktop->FindObject(x,y);
		if (!obj->MouseInside)
		{
			obj->MouseEnter();
			obj->MouseInside = true;
		}


		obj->MouseMove(x - obj->X,y - obj->Y);*/
	}

	void MouseDown(int x,int y, int button)
	{
		X = x;
		Y = y;
		ButtonState[button] = true;

		GObject* obj = Desktop->FindObject(x,y);
		obj->SetFocus();
		obj->MouseDown(x - obj->ScreenRegion.X, y - obj->ScreenRegion.Y, button);
	}

	void MouseUp(int x,int y, int button)
	{
		X = x;
		Y = y;
		ButtonState[button] = false;

		Focused->MouseUp(x - Focused->ScreenRegion.X,y - Focused->ScreenRegion.Y,button);

		/*GObject* obj = Desktop->FindObject(x,y);
		obj->MouseDown(x - obj->X, y - obj->Y, button);*/
		
		/*GObject* pobj = (GObject*)Focused->Parent;
		if (pobj == 0)
		{
			pobj = Desktop;
		}

		int rx = x - pobj->ScreenRegion.X;
		int ry = y - pobj->ScreenRegion.Y;

		if (Focused->IsInside(rx,ry))
		{
			Focused->MouseUp(rx-Focused->X,ry-Focused->Y,button);
		}*/
	}

	void MouseWheel(int x,int y,int delta)
	{
		GObject* obj = Desktop->FindObject(x,y);
		obj->MouseWheel(x - obj->ScreenRegion.X,y - obj->ScreenRegion.Y,delta);
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

	inline void EnableEventScissor(int _x,int _y,int _w,int _h)
	{
		Scissor.SetSize(_x,_y,_w,_h);
		EventScissor = true;
	}

	inline void DisableEventScissor()
	{
		EventScissor = false;
	}

	inline bool ScissorTest(int x,int y)
	{
		if (!EventScissor) return true;
		return Scissor.IsInside(x,y);
	}

	void EnableGUI(); // add as
	void DisableGUI();

	inline void Render()
	{
		Desktop->MouseInside = true;
		Desktop->Update();
		Desktop->DeliverMove();
		Desktop->Render();
	}
};


#endif