#ifndef GENUMS_H
#define GENUMS_H

enum TextVerticalAlign
{
	TVA_TOP,
	TVA_CENTER,
	TVA_BOTTOM,
};

enum TextHorizontalAlign
{
	THA_LEFT,
	THA_CENTER,
	THA_RIGHT,
};

enum ContentAlignment
{
	CA_TopLeft,
	CA_TopCenter,
	CA_TopRight,
	CA_MiddleLeft,
	CA_MiddleCenter,
	CA_MiddleRight,
	CA_BottomLeft,
	CA_BottomCenter,
	CA_BottomRight,
};

enum DockType
{
	DCK_NODOCK,
	DCK_LEFT,
	DCK_TOP,
	DCK_RIGHT,
	DCK_BOTTOM,
	DCK_FILL,
};

enum GUIClassID
{
	GOBJECT_CLASSID,
	GWINDOW_CLASSID,
	GBUTTON_CLASSID,
	GLABEL_CLASSID,
	GCHECKBOX_CLASSID,
	GRADIOBUTTON_CLASSID,
	GPROGRESSBAR_CLASSID,
	GTIMER_CLASSID,
	GTEXTBOXBASE_CLASSID,
	GTEXTBOX_CLASSID,
};

enum CheckStates
{
	CS_Unchecked = 0,
	CS_Checked = 1,
	CS_Indeterminate = 2,
};

enum GenericGraphicState
{
	GGS_NORMAL,
	GGS_OVER,
	GGS_DOWN,
	GGS_DISABLED, // TODO: do this
};



#endif