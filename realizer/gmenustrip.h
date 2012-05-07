#ifndef GMENUSTRIP_H
#define GMENUSTRIP_H

#include "gobject.h"

class GMenuItem;

class GMenuStrip: public GObject
{
public:
	GLayout* Layouter;

	GMenuStrip();

	GMenuItem* AddItem(const TString& menuName, NoArgEvent* event);

	GMenuItem* AddItem(const TString& menuItem);

	GMenuItem* AddSeperator();

	void Render();
	void Layout();
};


#endif