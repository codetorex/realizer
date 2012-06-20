#include "stdafx.h"
#include "gobjectmanager.h"
#include "gcomponents.h"

#include "ttypebuilder.h"
#include "ttypemanager.h"

GObjectManager GObjectManager::Instance;


TType CreatePositionType()
{
	TType typeInfo;
	IPosition tmpObj;

	TTypeBuilder tb(&typeInfo, &tmpObj);

	tb.SetName("Position");

	tb.AddField("X", &tmpObj.X, Types.ST_Int32);
	tb.AddField("Y", &tmpObj.Y, Types.ST_Int32);

	return typeInfo;
}

/// TODO: move this to somewhere else?
TType PositionType = CreatePositionType();

TType CreateSizeType()
{
	TType typeInfo;
	ISize tmpObj;

	TTypeBuilder tb(&typeInfo, &tmpObj);

	tb.SetName("Size");

	tb.AddField("Width", &tmpObj.Width, Types.ST_Int32);
	tb.AddField("Height", &tmpObj.Height, Types.ST_Int32);

	return typeInfo;
}

TType SizeType = CreateSizeType();

TType CreateObjectBaseType()
{
	TType typeInfo;
	GObject testObj;

	TTypeBuilder tb(&typeInfo, &testObj);
	
	IPosition* pos = &testObj;
	tb.AddField("Position", pos, &PositionType);

	ISize* size = &testObj;
	tb.AddField("Size", size, &SizeType);

	tb.AddField("Text", &testObj.Text, Types.ST_String);



	return typeInfo;
}

TType ObjectBaseType = CreateObjectBaseType();

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

TType* GObjectType::GetType()
{
	return &ObjectBaseType;
}