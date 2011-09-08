#include "stdafx.h"
#include "gprogressbar.h"
#include "vgui.h"


GProgressBar::GProgressBar()
{
	ClassID = GPROGRESSBAR_CLASSID;

	Value = 0;
	Minimum = 0;
	Maximum = 100;

	Text.Allocate(32); // pre allocate some space for easier setting

	PercentAlign = CA_MiddleCenter;

	ShowPercent = false;
}

void GProgressBar::Render()
{
	Skin->RenderProgressBar(this);
}

void GProgressBar::Update()
{
	this->GObject::Update();
	if (Value > Maximum)
	{
		Value = Maximum;
	}

	if (Value < Minimum)
	{
		Value = Minimum;
	}
}