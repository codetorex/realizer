#ifndef GTOOLSTRIP_H
#define GTOOLSTRIP_H

#include "gobject.h"
#include "gimage.h"
#include "gbutton.h"


/// TODO: make this derived from gbutton
class GToolStripButton: public GObject
{
private:
	int TextX, TextY;

public:
	GToolStripButton();

	GItemDrawStyles DrawStyle;

	GImage Image;

	int GraphicState;

	void OnMouseExit();
	void OnMouseUp(int x,int y,int button);
	void OnMouseMove(int x,int y);

	event<NoArgEvent> Click;

	void Render();
	void Layout();
	void Update();
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
	GToolStripButton* AddButton( const TString& buttonName, GImage& image, NoArgEvent* event, bool imageOnly = true);

	/**
	 * Creates a text only button then returns it.
	 */
	GToolStripButton* AddButton( const TString& buttonName, NoArgEvent* event );

	void AddSeperator();

	void Render();

	void Layout();
};

#endif