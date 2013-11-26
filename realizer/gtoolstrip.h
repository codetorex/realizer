#ifndef GTOOLSTRIP_H
#define GTOOLSTRIP_H

#include "gobject.h"
#include "gimage.h"
#include "gbuttonbase.h"


/// TODO: make this derived from gbutton
class GToolStripButton: public GButtonBase
{
public:
	GToolStripButton();

	GItemDrawStyles DrawStyle;

	event<NoArgEvent> Click;

	void Clicked(int x, int y, int button);

	void Render();
	void Layout();
};

class GToolStripSeperator: public GObject
{
public:
	GToolStripSeperator();

	void Render();
};


class GToolStrip: public GObject
{
public:
	GLayout* Layouter;

	GToolStrip();

	/**
	 * Creates a button with name and image then returns it.
	 */
	GToolStripButton* AddButton( const String& buttonName, GImage& image, NoArgEvent* event, bool imageOnly = true);

	/**
	 * Creates a text only button then returns it.
	 */
	GToolStripButton* AddButton( const String& buttonName, NoArgEvent* event );

	void AddSeperator();

	void Render();

	void Layout();
};

#endif