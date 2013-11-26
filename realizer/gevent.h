
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

class EventArgs
{
public:
	int type;
};

struct Joystick
{
	enum TAxis
	{
		AxisX,
		AxisY,
		AxisZ,
		AxisR,
		AxisU,
		AxisV,
		AxisPOV,
	};
};


class MouseEventArgs: public EventArgs
{
public:
	MouseEventArgs(int _x,int _y,int _delta,int _button)
	{
		x = _x;
		y = _y;
		delta = _delta;
		button = _button;
	}

	int x;
	int y;
	int delta;
	int button;
};


/*class EventArgs
{
public:
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

	int e_type;

	union
	{
		struct m  
		{
			int x; // used by mouse events
			int y; // used by mouse events
			bool inside; // used by mouse events
			union
			{
				int button; // used by mouse
				int delta; // used by mouse wheel
			};
		};

		dword t_unicode; // used in text writing event

		struct
		{
			int k_code; // used by key events
			bool k_alt; // used by key events
			bool k_ctrl; // used by key events
			bool k_shft; // used by key events
		};

		struct
		{
			dword	j_id;
			int		j_axis;
			union
			{
				float	j_pos;
				dword	j_button;
			};
			
		};
	};

};*/

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
	inline void dispatch(EventArgs* data){Call(data);}

private:
	virtual void Call(EventArgs*) = 0;
};

template <class T>
class GClassDispatch: public GDispatch
{
public:
	typedef void (T::*mfunc)(EventArgs*);
	GClassDispatch(T* obje,mfunc memfn) : mobj(obje), mobjfnc(memfn) {};
	
	inline void Call(EventArgs* mevent){((mobj)->*mobjfnc)(mevent);}

private:
	T*		mobj;
	mfunc	mobjfnc;
};

class RDLL GEvent
{
public:
	Array<GDispatch*> dispatchers;

	GEvent& operator += (GDispatch& evnt)
	{
		dispatchers.Add(&evnt);
		return *this;
	}

	GEvent& operator -= (GDispatch& evnt)
	{
		dispatchers.Remove(&evnt);
		return *this;
	}
};

template <class T>
inline GDispatch* EventHandler(T* obj, void (T::*memFn)(EventArgs*))
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
inline void InvokeMember(void* pcls,void* fnc,EventArgs* dt)	// fastest possible member call by hack. fast as normal call.
{
	_asm
	{
		push dt
		mov ecx,pcls
		call fnc
	}
}

typedef void (*EFNC)(EventArgs* gev); // Event Function Definition GIGA ELECTRON VOLT

#endif