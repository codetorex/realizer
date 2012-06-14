#include "stdafx.h"
#include "gobjectmanager.h"
#include "gcomponents.h"

GObjectManager GObjectManager::Instance;

class GButtonType: public GObjectType
{
public:
	GButtonType()
	{
		ClassID = GBUTTON_CLASSID;
		ObjectName = "button";
	}

	GObject* CreateObject()
	{
		GButton* newButton = new GButton();
		return newButton;
	}
};

class GWindowType: public GObjectType
{
public:
	GWindowType()
	{
		ClassID = GWINDOW_CLASSID;
		ObjectName = "window";
	}

	GObject* CreateObject()
	{
		GWindow* newWindow = new GWindow();
		return newWindow;
	}
};

GObjectManager::GObjectManager()
{
	RegisterFactory(new GWindowType());
	RegisterFactory(new GButtonType());
}