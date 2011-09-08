#ifndef GRADIOBUTTON_H
#define GRADIOBUTTON_H

#include "gobject.h"

class GRadioButton: public GObject
{
private:
	inline void SetGraphicState(GenericGraphicState id);

public:
	GRadioButton();

	void MouseMove(int x, int y);
	void MouseExit();
	void MouseUp(int x, int y, int button);

	void Render();

	void set_Checked(bool value);

	bool Checked;
	ContentAlignment CheckAlign;

	int GraphicState;
};

#endif