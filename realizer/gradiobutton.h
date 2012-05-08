#ifndef GRADIOBUTTON_H
#define GRADIOBUTTON_H

#include "gbuttonbase.h"

class GRadioButton: public GButtonBase
{
public:
	GRadioButton();

	void Clicked(int x, int y, int button);

	void Render();

	void set_Checked(bool value);

	bool Checked;
	ContentAlignment CheckAlign;
};

#endif