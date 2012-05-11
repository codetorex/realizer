#ifndef GBUTTONBASE_H
#define GBUTTONBASE_H

#include "gobject.h"



class GButtonBase: public GObject
{
public:
	GButtonBase();

	virtual void MouseExit();
	virtual void MouseUp(int x,int y,int button);
	virtual void MouseMove(int x,int y); 

	virtual void Clicked(int x, int y, int button) { };

	inline void SetGraphic(GButtonGraphics graphic)
	{
		ButtonGraphic = graphic;
	}

	int ButtonGraphic;
};


#endif