#ifndef GWINDOW_H
#define GWINDOW_H

#include "gobject.h"

class GWindow: public GObject
{
public:
	GObject* TitleBar;

	GWindow();

	str16 Caption;

	void CenterToScreen();

	void Layout();
	void Render();
	void Update();
	void Initialize();
};


#endif