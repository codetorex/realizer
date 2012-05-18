#include "stdafx.h"
#include "gdesktop.h"
#include "cengine.h"


void FillRectanglePadding(const IRectangle& baseRect, const IPadding& padding, const TColor32& color)
{
	// leftMost part
	Engine.Draw.FillRectangle(baseRect.X,baseRect.Y,padding.Left,baseRect.Height,color);


	int topWidth = baseRect.Width - (padding.Left + padding.Right);
	// top part
	Engine.Draw.FillRectangle(baseRect.X + padding.Left,baseRect.Y,topWidth,padding.Top,color);

	int bottomY = (baseRect.Y + baseRect.Height) - padding.Bottom;
	// bottom part
	Engine.Draw.FillRectangle(baseRect.X + padding.Left,bottomY,topWidth,padding.Bottom,color);

	int rightX = (baseRect.X + baseRect.Width) - padding.Right;
	// right part
	Engine.Draw.FillRectangle(rightX,baseRect.Y,padding.Right,baseRect.Height,color);
}

void GDesktop::RenderDebug( GObject* obj )
{
	//Engine.Draw.DrawRectangle(obj->DrawRegion,TColors::Yellow);
	
	IRegion marginRegion(obj->DrawRegion);
	marginRegion.Inflate(obj->Margin);
	Engine.Draw.DrawRectangle(marginRegion,TColors::GreenYellow);

	
	TColor32 padColor(0,192,192,64);
	FillRectanglePadding(obj->DrawRegion,obj->Border,padColor);


	TColor32 contentColor(192,192,0,64);
	IRectangle paddingRect(obj->DrawRegion);
	paddingRect.Shrink(obj->Padding);
	paddingRect.Shrink(obj->Border);

	Engine.Draw.FillRectangle(paddingRect.X,paddingRect.Y,paddingRect.Width,paddingRect.Height,contentColor);
	Engine.Draw.DrawRectangle(paddingRect,TColors::Red);

	

	//Engine.Draw.DrawRectangle(obj->DrawRegion.X + obj->Content.X,obj->DrawRegion.Y + obj->Content.Y,obj->Content.Width,obj->Content.Height,TColors::Purple);
}
