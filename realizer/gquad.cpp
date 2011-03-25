#include "stdafx.h"
#include "gquad.h"
#include "cengine.h"


void GScalableQuadParted::Render( GObject* obj )
{
	float cx,cy,topWidth;
	cx = (float)obj->ScreenPosition.X;
	cy = (float)obj->ScreenPosition.Y;
	topWidth = (float)(obj->Width - TopLeft.Width - TopRight.Width);

	TopLeft.Draw(cx,cy);
	cx += TopLeft.Width;
	Top.DrawPart(cx,cy,topWidth,Top.Height);
	cx += topWidth;
	TopRight.Draw(cx,cy);
	cx = (float)obj->ScreenPosition.X;
	cy += Top.Height;

	// TODO: draw left right and bottom


	
}