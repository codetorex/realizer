#ifndef GBUTTONBASE_H
#define GBUTTONBASE_H

#include "gobject.h"
#include "gimage.h"

class GButtonBase: public GObject
{
protected:
	/**
	 * Pre calculated text position
	 */
	IPosition TextPosition;

public:
	GButtonBase();

	virtual void MouseExit();
	virtual void MouseUp(int x,int y,int button);
	virtual void MouseMove(int x,int y); 

	virtual void Clicked(int x, int y, int button) { };

	GImage Image;
	Alignment ImageAlign;

	/**
	 * Gap between text and image if they are aligned in same place.
	 */
	int ImageGap;

	inline void SetGraphic(GButtonGraphics graphic)
	{
		ButtonGraphic = graphic;
	}

	virtual void setText(const TString& newText);

	void Layout();
	void Update();

	int ButtonGraphic;
};


#endif