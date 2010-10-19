#ifndef CINPUT_H
#define CINPUT_H

#include "realizertypes.h"
#include "thashmap.h"
// TODO: there should be actions defined and Inputs can be set for that actions.

class RDLL CAction
{
public:
	str8 actionName;

	virtual void Trigger(bool value) {}; // button press will use this
	virtual void Axis(float value) {}; // axis of joystick will use this
	virtual void Move(int value) {}; // mouse will use this?
};

class RDLL CValueChangerAction: public CAction
{
public:
	void* valueToChange;

	CValueChangerAction(void* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Trigger(bool value)
	{
		*(bool*)valueToChange = value;
	}

	inline void Axis(float value)
	{
		*(float*)valueToChange = value;
	}

	inline void Move(int value)
	{
		*(int*)valueToChange = value;
	}
};

template <class T>
class RDLL CCallerAction: public CAction
{
public:
	typedef void (T::*TFunction)(bool);

	CCallerAction(T* _object,TFunction _func)
	{
		realObject = _object;
		objectFunction = _func;
	}

	inline void Trigger(bool value)
	{
		realObject->objectFunction(value);
	}

	inline void Axis(float value){}
	inline void Move(int value){};

private:
	T*			realObject;
	TFunction	objectFunction;
};

class RDLL CSensor
{
public:
	int boundActions;
	CAction* Actions[3];

	CSensor()
	{
		boundActions = 0;
	}
	
	void BindAction(CAction* action)
	{
		Actions[boundActions++] = action;
	}

	void UnbindAction(CAction* action)
	{
		if (Actions[boundActions-1] == action)
		{
			boundActions--;
			return;
		}

		for (int i=0;i<boundActions;i++)
		{
			if (Actions[i] == action)
			{
				Actions[i] = Actions[--boundActions];
			}
		}
	}
};

class RDLL CButton: public CSensor
{
public:
	bool state; // true for down, false for up

	CButton()
	{
		state = false;
	}

	inline void Down()
	{
		int cba = boundActions;
		while(cba--)
		{
			Actions[cba]->Trigger(false);
		}
	}

	inline void Up()
	{
		int cba = boundActions;
		while(cba--)
		{
			Actions[cba]->Trigger(false);
		}
	}
};

class RDLL CInput
{
public:
	str8 inputName;
	bool connected;

	CInput()
	{
		connected = false;
	}

	virtual void KeyDown(int keyID);
	virtual void KeyUp(int keyID);
	virtual void Move(int axis,int amount);
};

class RDLL CKeyboard: public CInput
{
public:
	CButton Keys[256];

	CKeyboard()
	{
		inputName = "Keyboard";
	}
};

class RDLL CMouse: public CInput
{
public:
	int x; // absolute x position
	int y; // absolute y position
	int xdiff; // x difference for diff mode
	int ydiff; // y difference for diff mode
	CButton Buttons[5];

	CMouse()
	{
		x = 0;
		y = 0;
		inputName = "Mouse";
	}
};

class RDLL CJoystick: public CInput
{
public:

};


/**
* Low level Input manager.
*/
class RDLL CInputManager
{
public:
	CKeyboard			Keyboard;
	CMouse				Mouse;
	CJoystick			Joysticks[8];

	//TArray<CAction*>	Actions;

	THashMap<CAction*>	Actions; // maybe use dictionary in future.

	CInputManager()
	{
		
	}
	
	inline void UpdateMouse(int x,int y)
	{
		Mouse.x = x;
		Mouse.y = y;
	}

	void RegisterAction(CAction* action)
	{
		Actions.Add(action->actionName,action);
	}

	void BindKey(int keycode,const str8& actionName)
	{

	}

	CAction* CreateAction(const str8& actionName, bool* boolPtr )
	{
		CValueChangerAction* act = new CValueChangerAction(boolPtr);
		act->actionName = actionName;
		RegisterAction(act);
	}

};



#endif