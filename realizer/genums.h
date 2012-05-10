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
	GBUTTONBASE_CLASSID,
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
	GLISTBOX_CLASSID,
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
	GGS_DISABLED,
};

enum GItemDrawStyles
{
	GDS_TEXT = 1,
	GDS_IMAGE = 2,
	GDS_IMAGETEXT = 3,
};

enum GButtonGraphics
{
	GBG_NORMAL,
	GBG_OVER,
	GBG_DOWN,
	GBG_DISABLED,
	GBG_FOCUSED,
};


#endif