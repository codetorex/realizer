#include "stdafx.h"
#include "gproxy.h"

GProxy::GProxy()
{
	ClassID = GPROXY_CLASSID;
	ActiveObject = 0;
}

void GProxy::Render()
{
	if (ActiveObject)
	{
		ActiveObject->Render();
	}
}

void GProxy::Update()
{
	GObject::Update();
	if (ActiveObject)
	{
		OwnObject(ActiveObject);
		ActiveObject->Update();
	}
}

void GProxy::Layout()
{
	if (ActiveObject)
	{
		ActiveObject->SetRectangle(0,0,Width,Height);
		ActiveObject->Layout();
	}
}

GObject* GProxy::FindObject()
{
	if (ActiveObject)
	{
		return ActiveObject->FindObject();
	}
	return this;
}