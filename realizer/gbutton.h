#ifndef GBUTTON_H
#define GBUTTON_H

#include "gobject.h"

class GButton: public GObject
{
public:
	GButton();

	str16 Caption;

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	event<NoArgEvent> Clicked;

	void Render();

	int GraphicState;
};

#endif