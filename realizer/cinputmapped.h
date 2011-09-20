#ifndef CINPUTMAPPED_H
#define CINPUTMAPPED_H

#include "cinputinterface.h"

// NOTE: these Actions will be result of sensor input. CSensor -> Take Data -> CAction
// ***************************************************************
// Those are actions for output....
// ***************************************************************

// TODO: there should be actions defined and Inputs can be set for that actions.

class RDLL CAction
{
public:
	TString actionName;
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
* Which is used in quake?
*/
class CFlagAction: public CTriggerAction
{
public:
	ui32* flagAddr;
	ui32 bit;

	CFlagAction(ui32* _flagAddr, ui32 _bit)
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

// ***************************************************************
// Those are sensors for input....
// ***************************************************************

/**
* CSensor class for detecting device input.
* Class T is CAction result of sensor.
*/

template <class T>
class RDLL CSensor
{
public:
	T* Action;

	CSensor()
	{
		Action = 0;
	}

	inline void BindAction(T* action)
	{
		Action = action;
	}

	inline void UnbindAction()
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


class CMappedKeyboard: public IKeyboardObserver
{
public:
	CButton Keys[256];

	void BindKey(int KeyID, CTriggerAction* Action)
	{
		Keys[KeyID].BindAction(Action);
	}

	void UnbindKey(int KeyID)
	{
		Keys[KeyID].UnbindAction();
	}
	
	void KeyDown(int keyID)
	{
		Keys[keyID].Down();
	}

	void KeyUp(int keyID)
	{
		Keys[keyID].Up();
	}

	void KeyUnicode(ch16 keyChar)
	{
		// HMM... Not any known uses? Maybe used for mapping stuff in non english keyboards?
	}
};

class CMappedMouse: public IMouseObserver
{
public:
	CButton Buttons[5];
	CPosition X;
	CPosition Y;

	void MouseMove(int x,int y)
	{
		X = x;
		Y = y;
	}

	void MouseDown(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Down();
	}

	void MouseUp(int x,int y, int button)
	{
		X = x;
		Y = y;
		Buttons[button].Up();
	}

	void MouseWheel(int x,int y,int delta)
	{
		X = x;
		Y = y;
		// wtf we going to do with wheel?
	}
};


#endif