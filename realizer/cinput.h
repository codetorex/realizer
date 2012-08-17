#ifndef CINPUT_H
#define CINPUT_H

#include "tinputmap.h"
#include "tjoystick.h"
#include "tarray.h"
#include "thashmap.h"

#include "tmousemapped.h"
#include "tkeyboardmapped.h"
#include "texception.h"



class CInputMode: public TInputModeBase
{
public:
	enum CInputMoveFrom
	{
		IM_MOUSE_X,
		IM_MOUSE_Y,
	};

	TKeyboardMapped	KeyboardMap;
	TMouseMapped	MouseMap;
	//TJoystickMapped JoystickMaps[8];
	
	void BindKey(int key, TTriggerAction* action)
	{
		if (action == NULL)
		{
			throw Exception("Null parameter");
		}

		if (key < 0 || key >= 256)
		{
			throw Exception("Invalid keycode");
		}

		KeyboardMap.BindKey(key,action);
	}

	void BindMouseButton(int button, TTriggerAction* action)
	{
		if (action == NULL)
		{
			throw Exception("Null parameter");
		}

		if (button < 0 || button >= 5)
		{
			throw Exception("Invalid button");
		}

		MouseMap.BindButton(button, action);
	}

	/**
	 * Gets a trigger action, if it not exists it will be created and attached to default key
	 */
	TTriggerAction* GetCreateTriggerAction(const TString& name, int defaultKey)
	{
		TTriggerAction* trigger = GetTriggerAction(name);
		if (trigger == 0)
		{
			trigger = CreateTriggerAction(name);
			BindKey(defaultKey, trigger);
		}

		return trigger;
	}

	void GetMouseMoveAction(TMoveAction*& xMove, const TString& xName, TMoveAction*& yMove, const TString& yName)
	{
		xMove = GetMoveAction(xName);
		yMove = GetMoveAction(yName);

		if (xMove == 0)
		{
			xMove = CreateMoveAction(xName);
			MouseMap.X.BindAction(xMove);
		}

		if (yMove == 0)
		{
			yMove = CreateMoveAction(yName);
			MouseMap.Y.BindAction(yMove);
		}
	}

};

class CKeyboardRelay: public IKeyboardObserver
{
public:
	TArray<CInputMode*>* Chain;

	void KeyDown(int keyID)
	{
		ui32 i = Chain->Count;

		while(i--)
		{
			CInputMode* curMode = Chain->Item[i];
			if ( curMode->KeyboardMap.Keys[keyID].Down() )
			{
				return;
			}
		}
	}

	void KeyUp(int keyID)
	{
		ui32 i = Chain->Count;

		while(i--)
		{
			CInputMode* curMode = Chain->Item[i];
			if ( curMode->KeyboardMap.Keys[keyID].Up() )
			{
				return;
			}
		}
	}

	void KeyUnicode(ch16 keyChar)
	{

	}
};

class CMouseRelay: public IMouseObserver
{
public:
	TArray<CInputMode*>* Chain;

	void MouseMove(int x,int y)
	{
		CInputMode* curMode = Chain->GetLast();
		curMode->MouseMap.MouseMove(x,y);
	}

	void MouseDown(int x,int y, int button)
	{
		ui32 i = Chain->Count;

		while(i--)
		{
			CInputMode* curMode = Chain->Item[i];
			if ( curMode->MouseMap.Buttons[button].Down() )
			{
				return;
			}
		}
	}

	void MouseUp(int x,int y, int button)
	{
		ui32 i = Chain->Count;

		while(i--)
		{
			CInputMode* curMode = Chain->Item[i];
			if ( curMode->MouseMap.Buttons[button].Up() )
			{
				return;
			}
		}
	}

	void MouseWheel(int x,int y,int delta)
	{
		
	}
};

/**
 * Low level Input manager based on observer pattern.
 * There can be unlimited observers for input for any device.
 */
class CInputManager
{
public:
	TKeyboard					Keyboard;
	TMouse						Mouse;
	TJoystick					Joysticks[8];
	//TTouchPad					Pad; // we should not forget about this shit

	THashMap<TInputAction*>		Actions; // maybe use dictionary in future.
	TArray<CInputMode*>			Modes;

	TArray<CInputMode*>			Chain; // chain of responsibility

	CInputMode*					BaseMode;

	CInputManager()
	{
		BaseMode = 0;
		
		CKeyboardRelay* KeyboardRelay = new CKeyboardRelay();
		KeyboardRelay->Chain = &Chain;
		Keyboard.Attach(KeyboardRelay);

		CMouseRelay* MouseRelay = new CMouseRelay();
		MouseRelay->Chain = &Chain;
		Mouse.Attach(MouseRelay);
	}

	CInputMode* CreateBaseInputMode()
	{
		if (BaseMode)
		{
			return BaseMode;
		}
		BaseMode = GetCreateMode("Base");
		EnterMode(BaseMode);
		return BaseMode;
	}


	CInputMode* CreateMode(const TString& name)
	{
		CInputMode* newMode = new CInputMode();
		newMode->Name = name;
		RegisterMode(newMode);
		return newMode;
	}

	/**
	 * Gets a mode by name if it is exists.
	 * It will be created if it not exists.
	 */
	CInputMode* GetCreateMode(const TString& name)
	{
		TArrayEnumerator<CInputMode*> e(Modes);
		while(e.MoveNext())
		{
			if (e.Current->Name == name)
			{
				return e.Current;
			}
		}

		return CreateMode(name);
	}



	void RegisterMode(CInputMode* mode)
	{
		if (Modes.Contains(mode))
		{
			return;
		}

		Modes.Add(mode);
	}

	void UnregisterMode(CInputMode* mode)
	{
		Modes.Remove(mode);
		ExitMode(mode);
	}

	void EnterMode(CInputMode* mode)
	{
		Chain.Add(mode);
	}

	void ExitMode(CInputMode* mode)
	{
		Chain.RemovePreserve(mode);
	}


	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	/*void BindKey(int keycode,const TString& actionName)
	{
		TTriggerAction* action = (TTriggerAction*)Actions.GetValue(actionName);
		BindKey(keycode,action);
	}*/

	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	/*void BindKey(int keycode, TTriggerAction* action)
	{
		if (action == NULL)
		{
			throw Exception("Null");
		}

		if (keycode < 0 && keycode > 256)
		{
			throw Exception("Invalid keycode");
		}

		if (ActiveMappedKeyboard == NULL)
		{
			throw Exception("No keyboard");
		}

		ActiveMappedKeyboard->BindKey(keycode,action);
	}

	TTriggerAction* CreateHandler(const TString& actionName, bool* boolPtr )
	{
		TBoolChangerAction* act = new TBoolChangerAction(boolPtr);
		act->handlerName = actionName;
		RegisterAction(act);
		return act;
	}

	TEventAction* CreateHandler(const TString& actionName, TEventAction::TriggerEvent* handler )
	{
		TEventAction* act = new TEventAction();
		act->handlerName = actionName;
		act->OnTrigger += handler;
		return act;
		
	}*/
};



#endif