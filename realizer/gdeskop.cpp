#include "stdafx.h"
#include "gdesktop.h"
#include "cengine.h"


void GDesktop::RenderDebug( GObject* obj )
{
	Engine.Draw.DrawRectangle(obj->ScreenRegion,TColors::Red);
	Engine.Draw.DrawRectangle(obj->ScreenRegion.X + obj->ObjectRegion.X,obj->ScreenRegion.Y + obj->ObjectRegion.Y,obj->ObjectRegion.Width,obj->ObjectRegion.Height,TColors::Purple);
}
