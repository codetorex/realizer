#ifndef GDROPDOWN_H
#define GDROPDOWN_H

#include "gobject.h"

class GDropDown: public GObject
{
private:
	GObject* LastFocus;

public:
	GLayout* Layouter; // use a vertical layouter

	GDropDown();

	void Render();
	void Update();
	void Layout();

	void Show(int x,int y);
	void Show();
	void Hide();
};


#endif