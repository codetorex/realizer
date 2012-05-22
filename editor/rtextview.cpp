#include "stdafx.h"
#include "rtextview.h"
#include "gfont.h"

RTextView::RTextView()
{
	Document = 0;
	Name = "Text Editor";
}

void RTextView::Render()
{
	Font->Render(Name,DrawRegion.X()+50.0f,DrawRegion.Y()+50.f,ForeColor);
}