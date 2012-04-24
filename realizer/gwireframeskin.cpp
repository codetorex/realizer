#include "stdafx.h"
#include "gwireframeskin.h"
#include "cengine.h"
#include "gcomponents.h"

void GWireFrameSkin::RenderWindow( GWindow* window ) 
{
	Engine.Draw.NoTexture();

	Engine.Draw.DrawRectangle(window->ScreenRegion, TColors::maroon);
	Engine.Draw.DrawRectangle(window->TitleBar->ScreenRegion, TColors::aquamarine);

	window->Font->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft, TColors::wheat,10 );

	//Engine.Draw.DrawQuad(window->ScreenRegion, TColors::aqua);
	Engine.Draw.Flush();
}

void GWireFrameSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetSize(0,0,window->Width,20);
}

void GWireFrameSkin::RenderLabel( GLabel* label )
{
	label->Font->Render(label->Text,label->ScreenRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GWireFrameSkin::RenderSunkEdge( GObject* object )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(object->ScreenRegion, TColors::cyan);
}

void GWireFrameSkin::RenderTextBox( GTextBox* textbox )
{
	RenderSunkEdge(textbox);
}

void GWireFrameSkin::RenderButton( GButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->ScreenRegion, TColors::antiquewhite);
	button->Font->Render(button->Text,button->ScreenRegion,CA_MiddleCenter,button->ForeColor);
}