#ifndef GWINDOW_H
#define GWINDOW_H

#include "gcontainer.h"

class GWindow: public GObject
{
public:

	str16 Caption;

	void CenterToScreen();

	void Render();
	void Update();
	void Initialize();
};


#endif