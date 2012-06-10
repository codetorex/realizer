#include "stdafx.h"
#include "rnewprojectdialog.h"
#include "gcomponents.h"

RNewProjectDialog* RNewProjectDialog::Instance = 0;

void RNewProjectDialog::InitializeComponents()
{


	AddChild(&lbl_Name);
	AddChild(&lbl_Location);
	AddChild(&txt_Name);
	AddChild(&txt_Location);
}