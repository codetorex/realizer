#ifndef GCHECKBOX_H
#define GCHECKBOX_H

#include "gbuttonbase.h"

class GCheckBox: public GButtonBase
{
public:
	GCheckBox();

	void Clicked(int x,int y, int button);

	void Render();

	event<NoArgEvent> CheckedChanged;

	bool ThreeState;
	CheckStates CheckState;
	bool Checked;

	ContentAlignment CheckAlign;
};
#endif