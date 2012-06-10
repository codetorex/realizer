#ifndef RNEWPROJECTDIALOG_H
#define RNEWPROJECTDIALOG_H

#include "gwindow.h"
#include "glabel.h"
#include "gtextbox.h"

class RNewProjectDialog: public GWindow
{
public:
	/// There should only one instance at a time
	static RNewProjectDialog* Instance;

	GLabel lbl_Name;
	GLabel lbl_Location;

	GTextBox txt_Name;
	GTextBox txt_Location;

	void InitializeComponents();
};

#endif