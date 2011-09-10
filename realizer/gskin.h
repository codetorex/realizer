#ifndef GSKIN_H
#define GSKIN_H

#include "tcolor.h"

class GWindow;
class GButton;
class GObject;
class GFont;
class GLabel;
class GCheckBox;
class GRadioButton;
class GProgressBar;
class GTextBox;

class GSkin
{
public:
	GFont*	 SystemFont;
	TColor32 DefaultFontColor;

	virtual void RenderWindow(GWindow* window) = 0;
	virtual void LayoutWindow(GWindow* window) = 0;

	virtual void RenderButton(GButton* button) = 0;

	virtual void RenderLabel(GLabel* label) = 0;

	virtual void RenderCheckBox(GCheckBox* checkbox) = 0;

	virtual void RenderRadioButton(GRadioButton* radiobutton) = 0;

	virtual void RenderProgressBar(GProgressBar* progressbar) = 0;

	virtual void RenderTextBox(GTextBox* textbox) = 0;

	virtual void RenderSunkEdge(GObject* object) = 0;
};

#endif