#ifndef CINPUTINTERFACE_H
#define CINPUTINTERFACE_H

#include "realizertypes.h"

class IKeyboardObserver
{
public:
	virtual void KeyDown(int keyID) = 0;
	virtual void KeyUp(int keyID) = 0;
	virtual void KeyUnicode(ch16 keyChar) = 0;
};

class IMouseObserver
{
public:
	virtual void MouseMove(int x,int y) = 0;
	virtual void MouseDown(int x,int y, int button) = 0;
	virtual void MouseUp(int x,int y, int button) = 0;
	virtual void MouseWheel(int x,int y,int delta) = 0;
};

class IJoystickObserver
{
public:
	virtual void JoystickKey(int keyID) = 0;
	virtual void JoystickAxis(int axis, float value) = 0;
};

#endif


