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

	/**
	 * Popups at given position
	 */
	void Show(int x,int y);
	
	/**
	 * Popups at current cursor position
	 */
	void Show();

	void Hide();
};


#endif