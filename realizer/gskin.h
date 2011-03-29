#ifndef GSKIN_H
#define GSKIN_H

class GWindow;
class GButton;
class GObject;

class GSkin
{
public:
	virtual void RenderWindow(GObject* window) = 0;
	virtual void LayoutWindow(GWindow* window) = 0;

	virtual void RenderButton(GButton* button) = 0;
};


#endif