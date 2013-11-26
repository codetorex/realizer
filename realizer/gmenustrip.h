#ifndef GMENUSTRIP_H
#define GMENUSTRIP_H

#include "gobject.h"

class GMenuItem;

class GMenuStrip: public GObject
{
public:
	GLayout* Layouter;

	GMenuStrip();

	GMenuItem* AddItem(const String& menuName, NoArgEvent* event);

	GMenuItem* AddItem(const String& menuItem);

	GMenuItem* AddSeperator();

	void Render();
	void Layout();
};


#endif