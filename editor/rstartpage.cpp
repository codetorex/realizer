#include "stdafx.h"
#include "rstartpage.h"
#include "cengine.h"
#include "reditor.h"
#include "gschemedskin.h"
#include "rstartpagebutton.h"

RStartPage::RStartPage()
{
	Name = "Start Page";
	Document = 0;

	NewProjectButton.Text = "New Project";
	OpenProjectButton.Text = "Open Project";
}

void RStartPage::Render()
{
	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(DrawRegion, Editor.Skin->Colors.ButtonFace);
	Engine.Draw.DrawImage(Resources.StartPageTexture,DrawRegion.X(),DrawRegion.Y());

	Engine.Draw.DrawImage(Resources.RealizerLogo,DrawRegion.X() + 75.0f,DrawRegion.Y() - 125.0f, 0.6f);

	this->GObject::Render();
}

void RStartPage::Layout()
{
	if (NewProjectButton.Parent != this)
	{
		AddChild(&NewProjectButton);
		AddChild(&OpenProjectButton);
	}

	OwnObject(&NewProjectButton);
	OwnObject(&OpenProjectButton);

	NewProjectButton.SetRectangle(100,350,300,36);
	OpenProjectButton.SetRectangle(NewProjectButton);
	OpenProjectButton.TranslateVector(0,40);

	this->GObject::Layout();
}