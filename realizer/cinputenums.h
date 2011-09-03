#ifndef CINPUTENUMS_H
#define CINPUTENUMS_H

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


#endif

