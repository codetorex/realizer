#ifndef GENUMS_H
#define GENUMS_H

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
	GDESKTOP_CLASSID,
	GWINDOW_CLASSID,
	GBUTTON_CLASSID,
	GLABEL_CLASSID,
	GCHECKBOX_CLASSID,
	GRADIOBUTTON_CLASSID,
	GPROGRESSBAR_CLASSID,
	GTIMER_CLASSID,
	GTEXTBOXBASE_CLASSID,
	GTEXTBOX_CLASSID,
	GDROPDOWN_CLASSID,
	GMENUITEM_CLASSID,
	GMENUSTRIP_CLASSID,
	GIMAGE_CLASSID,
	GTOOLSTRIP_CLASSID,
	GTOOLBUTTON_CLASSID,
	GSCROLLBARBUTTON_CLASSID,
	GSCROLLBARDRAG_CLASSID,
	GSCROLLBAR_CLASSID,
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

enum GItemDrawStyles
{
	GDS_TEXT = 1,
	GDS_IMAGE = 2,
	GDS_IMAGETEXT = 3,
};



#endif