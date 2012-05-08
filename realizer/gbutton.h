#ifndef GBUTTON_H
#define GBUTTON_H

#include "gbuttonbase.h"

class GButton: public GButtonBase
{
public:
	GButton();

	event<NoArgEvent> Click;

	void Clicked(int x, int y, int button);

	void Render();
};

#endif