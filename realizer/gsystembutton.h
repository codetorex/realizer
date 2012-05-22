#ifndef GSYSTEMBUTTON_H
#define GSYSTEMBUTTON_H

#include "gbuttonbase.h"

class GSystemButton: public GButtonBase
{
public:
	GSystemButton();

	enum SystemButtonType
	{
		BT_TOOLWINDOW_CLOSE,
		BT_WINDOW_MINIMIZE,
		BT_WINDOW_MAXIMIZE,
		BT_WINDOW_RESTORE,
		BT_WINDOW_CLOSE,

	};

	SystemButtonType ButtonType;

	void Render();
	void Layout();
};


#endif