#ifndef GWINDOW_H
#define GWINDOW_H

#include "gobject.h"
#include "glayout.h"

class GWindow: public GObject
{
public:
	GObject* TitleBar;

	GLayout* Layouter;

	GWindow();

	string Text;

	void CenterToScreen();

	void Layout();
	void Render();
	void Update();
	void Initialize();
};


#endif