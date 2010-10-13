
#ifndef GEVENT_H
#define GEVENT_H

#include "raisetypes.h"
#include "tlinkedlist.h"

#define GEVENT_MOUSEDOWN	0x00000001
#define GEVENT_MOUSEUP		0x00000002
#define GEVENT_KEYDOWN		0x00000004
#define GEVENT_KEYUP		0x00000008
#define GEVENT_MOUSEWHEEL	0x00000010
#define GEVENT_KEYPRESS		0x00000020

class GObject;


// NEAR TOTAL COPY OF SFML
// THANKS "Laurent Gomila" I WILL NOT FORGET YOU


	namespace Mouse
	{
		enum Button
		{
			Left,
			Right,
			Middle,
			XButton1,
			XButton2,
		};
	}


	namespace Joy
	{
		enum Axis
		{
			gAxisX,
			gAxisY,
			gAxisZ,
			gAxisR,
			gAxisU,
			gAxisV,
			gAxisPOV,
		};
	}

	class EventArgs
	{
	public:

		struct MouseButtonEvent 
		{
			int X;
			int Y;
			bool Inside;
			Mouse::Button Button;
		};

		struct MouseMoveEvent
		{
			int X;
			int Y;
		};

		struct MouseWheelEvent
		{
			int Delta;
		};

		struct TextEvent
		{
			dword Unicode;
		};

		struct KeyEvent
		{
			int Code;
			bool Alt;
			bool Control;
			bool Shift;
		};

		struct JoyMoveEvent
		{
			dword JoystickID;
			Joy::Axis Axis;
			float Position;
		};

		struct JoyButtonEvent
		{
			dword JoystickID;
			dword Button;
		};

		enum EventType
		{
			MousePressed,
			MouseReleased,
			MouseWheelMoved,
			MouseMoved,
			MouseLeft,
			KeyPressed,
			KeyReleased,
			KeyChar,
			GotFocus,
			LostFocus,
			JoyPressed,
			JoyReleased,
			JoyMoved,
			Closed,
		};

		EventType Type;
		union
		{
			MouseButtonEvent	MouseButton;
			MouseMoveEvent		MouseMove;
			MouseWheelEvent		MouseWheel;
			TextEvent			TextData;
			KeyEvent			Key;
			JoyMoveEvent		JoyMove;
			JoyButtonEvent		JoyButton;
		};

	};

	class RDLL EventListener
	{
	public:
		virtual void OnEvent(EventArgs* evnt) = 0;
	};


/*union EData
{
	int		e_key;		// key data for keyboard based events
	int		e_delta;	// mouse wheel delta for wheel events
	int		e_mouse;	// mouse button for mouse based events
};

class RDLL GEventOLD	// event class, that gobject's can understand
{
public:
	int		e_x;			
	int		e_y;
	EData	e_data;
	bool	e_inside;
};*/



/*class RDLL GEventManager	// an event manager needed for every opengl contexts
{
public:
	GEventManager();

	// states
	int			MouseX;		// <- update these values every frame
	int			MouseY;		// <- update these values every frame
	bool		MouseState[2];	// states of buttons
	bool		KeyState[256];	// states of keys
	void UpdateMouse(int _x,int _y){MouseX=_x;MouseY=_y;};	// inline function

	// event data
	dword		e_type;		// event type
	int			e_x;		// event x of mouse
	int			e_y;		// location of mouse
	EData		e_data;		// wheel delta for wheel event, keycode for keyboard event, button number for mouse event
	bool		e_wfmup;	// wait for mouse up
	bool		e_cbox;		// combo box special ( todo: change this system )

	GEventOLD		e_event;	// the event, that gobjects can understand ( todo: make these events queued. )
	GObject*	focused;	// focused object, can never be 0 at least it must set to gdesktop self
	void SetMouseEvent(int _sx,int _sy);
	void SetKeyEvent();

	// event triggers
	void KeyDownHandler			(int _keycode);
	void KeyUpHandler			(int _keycode);
	void KeyPressHandler		(int _keycode);
	void MouseDownHandler		(int _x,int _y,int _button);
	void MouseUpHandler			(int _x,int _y,int _button);
	void MouseWheelHandler		(int _x,int _y,int _delta);

	// event scissor functions
	int		e_scissor_x;
	int		e_scissor_y;
	int		e_scissor_w;
	int		e_scissor_h;
	bool	e_scissor;

	bool ScissorTest			();	// returns true if mouse was in event scissor
	void EnableEventScissor		(int _x,int _y,int _w,int _h);
	void DisableEventScissor	(){e_scissor=false;};
};*/

class RDLL GDispatch
{
public:
	void dispatch(GEventOLD* data){Call(data);}

private:
	virtual void Call(GEventOLD*) = 0;
};

template <class T>
class GClassDispatch: public GDispatch
{
public:
	typedef void (T::*mfunc)(GEventOLD*);
	GClassDispatch(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};
	
	void Call(GEventOLD* mevent){((mobj)->*mobjfnc)(mevent);}

private:
	T*		mobj;
	mfunc	mobjfnc;
};	

template <class T>
inline GDispatch* EventHandler(T* obj, void (T::*memFn)(GEventOLD*))
{ 
	return (GDispatch*)(new GClassDispatch<T>(obj,memFn));
};

// TODO: implement normal function callback.

/*
	This function is absolutely an hack!
	It makes calling member function fast as calling a normal function
	Compiler just adds these instructions

	Drawbacks:
	- maybe compiler compability, only tested on vc++ 6.0 sp4
	- multi inheritance class maybe not supported, but not tested yet
*/
inline void InvokeMember(void* pcls,void* fnc,GEventOLD* dt)	// fastest possible member call by hack. fast as normal call.
{
	_asm
	{
		push dt
		mov ecx,pcls
		call fnc
	}
}

typedef void (*EFNC)(GEventOLD* gev); // Event Function Definition GIGA ELECTRON VOLT

#endif