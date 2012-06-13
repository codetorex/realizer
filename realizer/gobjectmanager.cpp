#include "stdafx.h"
#include "gobjectmanager.h"
#include "gcomponents.h"

GObjectManager GObjectManager::Instance;

class GButtonFactory: public GObjectType
{
public:
	GButtonFactory()
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

GObjectManager::GObjectManager()
{
	RegisterFactory(new GButtonFactory());
}