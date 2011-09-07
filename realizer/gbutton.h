#ifndef GBUTTON_H
#define GBUTTON_H

#include "gobject.h"

class GButton: public GObject
{
public:
	GButton();

	void MouseExit();
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	event<NoArgEvent> Click;

	void Render();

	int GraphicState;
};

#endif