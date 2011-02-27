#ifndef CINPUT_H
#define CINPUT_H

#include "realizertypes.h"
#include "thashmap.h"
// TODO: there should be actions defined and Inputs can be set for that actions.



class RDLL CAction
{
public:
	str8 actionName;
};

class CTriggerAction: public CAction
{
public:
	virtual void Trigger(bool value) = 0; // button press will use this
};

class CAxisAction: public CAction
{
	virtual void Axis(float value) = 0; // axis of joystick will use this
};

class CMoveAction: public CAction
{
	virtual void Move(int value) = 0; // mouse will use this?
};

/**
* Input is digital, like true or false signals. But output will be analog between 0.0f and 1.0f.
*/
class CDiglogAction: public CTriggerAction
{
public:

};

/**
* This one can be used to implement flag based action map.
*/
class CFlagAction: public CTriggerAction
{
public:
	dword* flagAddr;
	dword bit;

	CFlagAction(dword* _flagAddr, dword _bit)
	{
		flagAddr = _flagAddr;
		bit = _bit;
	}

	inline void Trigger(bool value)
	{
		if (value)
		{
			*flagAddr |= bit;
		}
		else
		{
			*flagAddr &= ~bit;
		}
	}
};

class CBoolChangerAction: public CTriggerAction
{
public:
	bool* valueToChange;

	CBoolChangerAction(bool* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Trigger(bool value)
	{
		*valueToChange = value;
	}
};

class CFloatChangerAction: public CTriggerAction
{
public:
	float* valueToChange;

	CFloatChangerAction(float* dataPtr)
	{
		valueToChange = dataPtr;
	}

	inline void Axis(float value)
	{
		*valueToChange = value;
	}
};

/*class RDLL CValueChangerAction: public CAction
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
};*/

template <class T>
class RDLL CCallerAction: public CTriggerAction
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

private:
	T*			realObject;
	TFunction	objectFunction;
};

template <class T>
class RDLL CSensor
{
public:
	T* Action;

	CSensor()
	{
		Action = 0;
	}
	
	void BindAction(T* action)
	{
		Action = action;
	}

	void UnbindAction()
	{
		Action = 0;
	}
};

/**
* Digital boolean sensor.
*/
class RDLL CButton: public CSensor<CTriggerAction>
{
public:
	bool state; // true for down, false for up

	CButton()
	{
		state = false;
	}

	inline void Down()
	{
		if ( Action == 0 ) return;
		Action->Trigger(true);
	}

	inline void Up()
	{
		if ( Action == 0 ) return;
		Action->Trigger(false);
	}
};

/**
* Floating point based sensor.
*/
class RDLL CAxis: public CSensor<CAxisAction>
{

};

/**
* Integer based sensor.
*/
class RDLL CPosition: public CSensor<CMoveAction>
{
public:
	int Absolute;
	int Diff;

	CPosition()
	{
		Absolute = 0;
		Diff = 0;
	}

	CPosition(const int val)
	{
		Absolute = val;
		Diff = 0;
	}

	inline void Set(int newPos)
	{
		Diff = newPos - Absolute;
		Absolute = newPos;
	}

	inline CPosition& operator = (int newPos)
	{
		Diff = newPos - Absolute;
		Absolute = newPos;
		return *this;
	}

	inline operator int (void)
	{
		return Absolute;
	}
};

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

class RDLL IKeyboardEventListener
{
public:
	virtual void KeyDown(int keyID) = 0;
	virtual void KeyUp(int keyID) = 0;
	virtual void KeyUnicode(ch16 keyChar) = 0;
};

class RDLL IMouseEventListener
{
public:
	virtual void MouseMove(int x,int y) = 0;
	virtual void MouseDown(int x,int y, int button) = 0;
	virtual void MouseUp(int x,int y, int button) = 0;
	virtual void MouseWheel(int x,int y,int delta) = 0;
};


struct Keys
{
	enum TKeys
	{
		BackSpace = VK_BACK ,
		Tab = VK_TAB ,
		Enter = VK_RETURN ,
		Shift = VK_SHIFT ,
		Ctrl = VK_CONTROL ,
		Alt = VK_MENU ,
		Pause = VK_PAUSE ,
		CapsLock = VK_CAPITAL ,
		Esc = VK_ESCAPE ,
		Space = VK_SPACE ,
		PageUp = VK_PRIOR ,
		PageDown = VK_NEXT ,
		End = VK_END ,
		Home = VK_HOME ,
		Left = VK_LEFT ,
		Up = VK_UP ,
		Right = VK_RIGHT ,
		Down = VK_DOWN ,
		PrintScr = VK_SNAPSHOT ,
		Insert = VK_INSERT ,
		Delete = VK_DELETE ,
		Num0 = 0x30 ,
		Num1 = 0x31 ,
		Num2 = 0x32 ,
		Num3 = 0x33 ,
		Num4 = 0x34 ,
		Num5 = 0x35 ,
		Num6 = 0x36 ,
		Num7 = 0x37 ,
		Num8 = 0x38 ,
		Num9 = 0x39 ,
		A = 0x41 ,
		B = 0x42 ,
		C = 0x43 ,
		D = 0x44 ,
		E = 0x45 ,
		F = 0x46 ,
		G = 0x47 ,
		H = 0x48 ,
		I = 0x49 ,
		J = 0x4A ,
		K = 0x4B ,
		L = 0x4C ,
		M = 0x4D ,
		N = 0x4E ,
		O = 0x4F ,
		P = 0x50 ,
		Q = 0x51 ,
		R = 0x52 ,
		S = 0x53 ,
		T = 0x54 ,
		U = 0x55 ,
		V = 0x56 ,
		W = 0x57 ,
		X = 0x58 ,
		Y = 0x59 ,
		Z = 0x5A ,
		LSys = VK_LWIN ,
		RSys = VK_RWIN ,
		Menu = VK_APPS ,
		SLEEP = VK_SLEEP ,
		Numpad0 = VK_NUMPAD0 ,
		Numpad1 = VK_NUMPAD1 ,
		Numpad2 = VK_NUMPAD2 ,
		Numpad3 = VK_NUMPAD3 ,
		Numpad4 = VK_NUMPAD4 ,
		Numpad5 = VK_NUMPAD5 ,
		Numpad6 = VK_NUMPAD6 ,
		Numpad7 = VK_NUMPAD7 ,
		Numpad8 = VK_NUMPAD8 ,
		Numpad9 = VK_NUMPAD9 ,
		Multiply = VK_MULTIPLY ,
		Add = VK_ADD ,
		Seperator = VK_SEPARATOR ,
		Subtract = VK_SUBTRACT ,
		Decimal = VK_DECIMAL ,
		Divide = VK_DIVIDE ,
		F1 = VK_F1 ,
		F2 = VK_F2 ,
		F3 = VK_F3 ,
		F4 = VK_F4 ,
		F5 = VK_F5 ,
		F6 = VK_F6 ,
		F7 = VK_F7 ,
		F8 = VK_F8 ,
		F9 = VK_F9 ,
		F10 = VK_F10 ,
		F11 = VK_F11 ,
		F12 = VK_F12 ,
		F13 = VK_F13 ,
		F14 = VK_F14 ,
		F15 = VK_F15 ,
		NumLock = VK_NUMLOCK ,
		ScrollLock = VK_SCROLL ,
		LShift = VK_LSHIFT ,
		RShift = VK_RSHIFT ,
		LCtrl = VK_LCONTROL ,
		RCtrl = VK_RCONTROL ,
		LMenu = VK_LMENU ,
		RMenu = VK_RMENU ,
		Tilde = VK_OEM_3 ,
		Quote = VK_OEM_7 ,
	};
};

/**
* Mapped keyboard input.
*/
class RDLL CKeyboard: public CInputDevice
{
public:
	CButton Keys[256];
	IKeyboardEventListener* EventListener;

	CKeyboard()
	{
		inputName = "Keyboard";
		EventListener = NULL;
	}

	inline void KeyDown(int keyID)
	{
		Keys[keyID].Down();
		if (EventListener != 0) EventListener->KeyDown(keyID);
	}

	inline void KeyUp(int keyID)
	{
		Keys[keyID].Up();
		if (EventListener != 0) EventListener->KeyUp(keyID);
	}

	inline void KeyUnicode(ch16 keyChar)
	{
		if (EventListener != 0) EventListener->KeyUnicode(keyChar);
	}
};

struct MouseButtons
{
	enum TButtons
	{
		Left,
		Right,
		Middle,
		XButton1,
		XButton2,
		None,
	};
};

class RDLL CMouse: public CInputDevice
{
public:
	CPosition x;
	CPosition y;
	
	CButton Buttons[5];

	CMouse()
	{
		inputName = "Mouse";
	}

	inline void MouseMove(int _x,int _y)
	{
		x = _x;
		y = _y;
	}

	inline void MouseDown(int _x,int _y,int _button)
	{
		x = _x;
		y = _y;
		Buttons[_button].Down();
	}

	inline void MouseUp(int _x,int _y,int _button)
	{
		x = _x;
		y = _y;
		Buttons[_button].Up();
	}

	inline void MouseWheel(int _x, int _y, int _delta)
	{
		x = _x;
		y = _y;

	}
};

class RDLL CJoystick: public CInputDevice
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

	void RegisterAction(CAction* action)
	{
		Actions.Add(action->actionName,action);
	}

	void BindKey(int keycode,const str8& actionName)
	{
		Keyboard.Keys[keycode].Action = (CTriggerAction*)Actions.GetValue(actionName);
	}

	void BindKey(int keycode, CTriggerAction* action)
	{
		if (action == NULL)
		{
			// ERROR: null action
		}

		if (keycode < 0 && keycode > 256)
		{
			// ERROR: invalid key code
		}
		Keyboard.Keys[keycode].Action = action;
	}

	CAction* CreateAction(const str8& actionName, bool* boolPtr )
	{
		CBoolChangerAction* act = new CBoolChangerAction(boolPtr);
		act->actionName = actionName;
		RegisterAction(act);
		return act;
	}

};



#endif