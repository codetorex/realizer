#ifndef CINPUT_H
#define CINPUT_H

#include "cinputenums.h"
#include "cinputinterface.h"
#include "cinputmapped.h"
#include "tarray.h"
#include "thashmap.h"

class RDLL CInputDevice
{
public:
	str8 inputName;
	bool connected;

	CInputDevice()
	{
		connected = false;
	}
};

/**
* Mapped keyboard input.
*/
class RDLL CKeyboard: public CInputDevice
{
public:
	TArray< IKeyboardObserver* > Observers;

	CKeyboard()
	{
		inputName = "Keyboard";
	}

	inline void Attach( IKeyboardObserver* Observer )
	{
		Observers.Add(Observer);
	}

	inline void Detach( IKeyboardObserver* Observer )
	{
		Observers.Remove(Observer);
	}

	inline void KeyDown(int keyID)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->KeyDown(keyID);
		}
	}

	inline void KeyUp(int keyID)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->KeyUp(keyID);
		}
	}

	inline void KeyUnicode(ch16 keyChar)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->KeyUnicode(keyChar);
		}
	}
};

class RDLL CMouse: public CInputDevice
{
public:
	TArray< IMouseObserver* > Observers;

	CMouse()
	{
		inputName = "Mouse";
	}

	inline void Attach( IMouseObserver* Observer )
	{
		Observers.Add(Observer);
	}

	inline void Detach( IMouseObserver* Observer )
	{
		Observers.Remove(Observer);
	}

	inline void MouseMove(int _x,int _y)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseMove(_x,_y);
		}
	}

	inline void MouseDown(int _x,int _y,int _button)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseDown(_x,_y,_button);
		}
	}

	inline void MouseUp(int _x,int _y,int _button)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseUp(_x,_y,_button);
		}
	}

	inline void MouseWheel(int _x, int _y, int _delta)
	{
		int i = Observers.Count;
		while(i--)
		{
			Observers.Item[i]->MouseDown(_x,_y,_delta);
		}
	}
};

class RDLL CJoystick: public CInputDevice
{
public:
	// Not Implemented Yet.
};


/**
* Low level Input manager based on observer pattern.
* There can be unlimited observers for input for any device.
*/
class RDLL CInputManager
{
public:
	CKeyboard			Keyboard;
	CMouse				Mouse;
	CJoystick			Joysticks[8];

	THashMap<CAction*>	Actions; // maybe use dictionary in future.
	CMappedKeyboard*	ActiveMappedKeyboard;

	CInputManager()
	{
		ActiveMappedKeyboard = 0;
	}

	CMappedKeyboard* CreateMappedKeyboard(bool attach = true)
	{
		CMappedKeyboard* keybrd = new CMappedKeyboard();
		if (attach)
		{
			Keyboard.Attach(keybrd);
			ActiveMappedKeyboard = keybrd;
		}
		return keybrd;
	}

	void RegisterAction(CAction* action)
	{
		Actions.Add(action->actionName,action);
	}

	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	void BindKey(int keycode,const str8& actionName)
	{
		CTriggerAction* action = (CTriggerAction*)Actions.GetValue(actionName);;
		BindKey(keycode,action);
	}

	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	void BindKey(int keycode, CTriggerAction* action)
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

	CTriggerAction* CreateAction(const str8& actionName, bool* boolPtr )
	{
		CBoolChangerAction* act = new CBoolChangerAction(boolPtr);
		act->actionName = actionName;
		RegisterAction(act);
		return act;
	}
};



#endif