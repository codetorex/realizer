#include "stdafx.h"
#include "vgui.h"
#include "cengine.h"


void VGUI::InitializeDesktop( GObject* _desktop )
{
	_desktop->Master = this;
	Focused = _desktop;
	_desktop->Parent = _desktop;
	_desktop->SetSize(0,0,Engine.Renderer.vWidth,Engine.Renderer.vHeight);
}

void VGUI::ActivateDesktop(GObject* newDesktop)
{
	Desktop = newDesktop;
	InitializeDesktop(newDesktop);
}

void VGUI::EnableGUI(GSkin* defSkin,GObject* desktopObj)
{
	if (desktopObj == 0)
	{
		if (Desktop == 0)
		{
			desktopObj = CreateDesktop(true);
		}
	}
	else
	{
		ActivateDesktop(desktopObj);
	}

	if (defSkin != 0)
	{
		desktopObj->Skin = defSkin;
		desktopObj->Font = desktopObj->Skin->SystemFont;
		desktopObj->ForeColor = desktopObj->Skin->DefaultFontColor;
	}
	
	Engine.Inputs.Mouse.Attach(this);
	Engine.Inputs.Keyboard.Attach(this);

	Enabled = true;
}

void VGUI::DisableGUI()
{
	Engine.Inputs.Mouse.Detach(this);
	Engine.Inputs.Keyboard.Detach(this);
	Enabled = false;
}

