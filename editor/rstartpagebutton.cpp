#include "stdafx.h"
#include "rstartpagebutton.h"
#include "vgui.h"
#include "reditorskin.h"
#include "gfont.h"

RStartPageButton::RStartPageButton()
{
	TextAlign = CA_MiddleLeft;
	ImageAlign = CA_MiddleLeft;
	ImageGap = 10;
}

void RStartPageButton::Render()
{
	REditorSkin* es = (REditorSkin*)Skin;
	Engine.Draw.SetTexture(es->SkinTexture);
	es->StartPageButton.Render(this);

	if (Image.Visible)
	{
		Image.Render();
	}

	Font->Render(Text,DrawRegion.X() + TextPosition.X, DrawRegion.Y() + TextPosition.Y,ForeColor);
}

