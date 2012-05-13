#include "stdafx.h"
#include "gwireframeskin.h"
#include "cengine.h"
#include "gcomponents.h"

void GWireFrameSkin::RenderWindow( GWindow* window ) 
{
	Engine.Draw.NoTexture();

	Engine.Draw.DrawRectangle(window->ScreenRegion, TColors::Maroon);
	Engine.Draw.DrawRectangle(window->TitleBar->ScreenRegion, TColors::Aquamarine);

	window->Font->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft, TColors::Wheat,10 );

	//Engine.Draw.DrawQuad(window->ScreenRegion, TColors::aqua);
	Engine.Draw.Flush();
}

void GWireFrameSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetSize(0,0,window->Width,20);
	window->ObjectRegion.SetRectangle(0,0,window->Width,window->Height);
}

void GWireFrameSkin::RenderLabel( GLabel* label )
{
	label->Font->Render(label->Text,label->ScreenRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GWireFrameSkin::RenderSunkEdge( GObject* object )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(object->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderTextBox( GTextBox* textbox )
{
	RenderSunkEdge(textbox);
}

void GWireFrameSkin::RenderButton( GButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->ScreenRegion, TColors::AntiqueWhite);
	button->Font->Render(button->Text,button->ScreenRegion,CA_MiddleCenter,button->ForeColor);
}

void GWireFrameSkin::RenderMenuStrip( GMenuStrip* menustrip )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menustrip->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderDropDown( GDropDown* dropdown )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(dropdown->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutDropDown( GDropDown* dropdown )
{
	dropdown->ObjectRegion.SetRectangle(0,0,dropdown->Width,dropdown->Height);
}

void GWireFrameSkin::RenderMenuItem( GMenuItem* menuItem )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menuItem->ScreenRegion, TColors::Cyan);
	//menuItem->Font->Render(menuItem->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft, TColors::Wheat,10 );
}

void GWireFrameSkin::RenderMenuStripItem( GMenuItem* menuItem )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menuItem->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutMenuStripItem( GMenuItem* menuItem )
{
	menuItem->ObjectRegion.SetRectangle(0,0,menuItem->Width,menuItem->Height);
}

void GWireFrameSkin::RenderToolStrip( GToolStrip* toolbox )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(toolbox->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderToolButton( GToolStripButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutToolButton( GToolStripButton* button )
{
	button->ObjectRegion.SetRectangle(0,0,button->Width,button->Height);
}

void GWireFrameSkin::RenderScrollBarButton( GScrollBarButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutScrollBarButton( GScrollBarButton* button )
{
	button->SetWidth(16);
	button->SetHeight(16);
}

void GWireFrameSkin::RenderScrollBar( GScrollBar* scrollbar )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(scrollbar->ScreenRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutScrollBar( GScrollBar* scrollbar )
{
	LayoutScrollBarButton(scrollbar->UpButton);
	LayoutScrollBarButton(scrollbar->DownButton);

	throw NotImplementedException();
}

void GWireFrameSkin::LayoutSunkEdge( GObject* object )
{
	object->ObjectRegion.SetRectangle(0,0,object->Width,object->Height);
}

void GWireFrameSkin::RenderHilight( int x,int y, int w,int h )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawQuad(x,y,x+w,y+h,0,0,1,1, TColors::Cyan);
}

void GWireFrameSkin::RenderTabControl( GTabControl* tabc )
{
	throw NotImplementedException();
}