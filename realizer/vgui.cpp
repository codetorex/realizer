#include "stdafx.h"
#include "vgui.h"
#include "cengine.h"

void VGUI::ActivateDesktop(GObject* newDesktop)
{
	Desktop = newDesktop;
	newDesktop->Master = this;
	Focused = Desktop;
	Desktop->Parent = Desktop;
	Desktop->X = 0;
	Desktop->Y = 0;
	Desktop->Width = Engine.Renderer.vWidth;
	Desktop->Height = Engine.Renderer.vHeight;
}

void VGUI::EnableGUI()
{
	Engine.Inputs.Mouse.Attach(this);
	Engine.Inputs.Keyboard.Attach(this);
}

