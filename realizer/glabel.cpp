#include "stdafx.h"
#include "glabel.h"
#include "vgui.h"

GLabel::GLabel()
{
	ClassID = GLABEL_CLASSID;
}

void GLabel::Render()
{
	Skin->RenderLabel(this);
}