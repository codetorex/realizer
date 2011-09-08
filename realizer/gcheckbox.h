#ifndef GCHECKBOX_H
#define GCHECKBOX_H

#include "gobject.h"

class GCheckBox: public GObject
{
private:
	inline void SetGraphicState(GenericGraphicState id);

public:
	GCheckBox();

	void MouseMove(int x, int y);
	void MouseExit();
	void MouseUp(int x, int y, int button);

	void Render();

	bool ThreeState;
	CheckStates CheckState;
	bool Checked;

	ContentAlignment CheckAlign;

	int GraphicState;
};
#endif