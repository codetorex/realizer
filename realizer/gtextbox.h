#ifndef GTEXTBOX_H
#define GTEXTBOX_H

#include "gobject.h"
#include "gtextboxbase.h"

class GTextBox: public GTextBoxBase
{
public:
	GTextBox();	

	void Render();
};

#endif