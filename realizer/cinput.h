#ifndef CINPUT_H
#define CINPUT_H

#include "tinputmap.h"
#include "tjoystick.h"
#include "tarray.h"
#include "thashmap.h"

/**
* Low level Input manager based on observer pattern.
* There can be unlimited observers for input for any device.
*/
class CInputManager
{
public:
	TKeyboard			Keyboard;
	TMouse				Mouse;
	TJoystick			Joysticks[8];

	THashMap<TInputAction*>	Actions; // maybe use dictionary in future.
	TMappedKeyboard*	ActiveMappedKeyboard;

	CInputManager()
	{
		ActiveMappedKeyboard = 0;
	}

	TMappedKeyboard* CreateMappedKeyboard(bool attach = true)
	{
		TMappedKeyboard* keybrd = new TMappedKeyboard();
		if (attach)
		{
			Keyboard.Attach(keybrd);
			ActiveMappedKeyboard = keybrd;
		}
		return keybrd;
	}

	void RegisterAction(TInputAction* action)
	{
		Actions.Add(action->actionName,action);
	}

	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	void BindKey(int keycode,const TString& actionName)
	{
		TTriggerAction* action = (TTriggerAction*)Actions.GetValue(actionName);;
		BindKey(keycode,action);
	}

	/**
	* Binds a key to ActiveMappedKeyboard if there is one.
	* Actually useless shortcut function, but yeah it's looks cool.
	*/
	void BindKey(int keycode, TTriggerAction* action)
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

	TTriggerAction* CreateAction(const TString& actionName, bool* boolPtr )
	{
		TBoolChangerAction* act = new TBoolChangerAction(boolPtr);
		act->actionName = actionName;
		RegisterAction(act);
		return act;
	}

	TEventAction* CreateAction(const TString& actionName, TEventAction::TriggerEvent* handler )
	{
		TEventAction* act = new TEventAction();
		act->actionName = actionName;
		act->OnTrigger += handler;
		return act;
		
	}
};



#endif