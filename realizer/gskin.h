#ifndef GSKIN_H
#define GSKIN_H

class GWindow;

class GSkin
{
public:
	virtual void RenderWindow(GWindow* window) = 0;
	virtual void LayoutWindow(GWindow* window) = 0;

};


#endif