#ifndef GSKIN_H
#define GSKIN_H

#include "tcolor.h"

class GWindow;
class GButton;
class GObject;
class GFont;

class GSkin
{
public:
	GFont*	 SystemFont;
	TColor32 DefaultFontColor;

	virtual void RenderWindow(GWindow* window) = 0;
	virtual void LayoutWindow(GWindow* window) = 0;

	virtual void RenderButton(GButton* button) = 0;
};


#endif