#include "stdafx.h"
#include "rstartpage.h"
#include "cengine.h"
#include "reditor.h"
#include "gschemedskin.h"

void RStartPage::Render()
{
	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(DrawRegion, Editor.EditorSkin->Colors.ButtonFace);
	Engine.Draw.DrawImage(Resources.StartPageTexture,DrawRegion.X(),DrawRegion.Y());

	Engine.Draw.DrawImage(Resources.RealizerLogo,DrawRegion.X() + 75.0f,DrawRegion.Y() - 125.0f, 0.6f);
}

RStartPage::RStartPage()
{
	Name = "Start Page";
	Document = 0;
}