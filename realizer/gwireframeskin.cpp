#include "stdafx.h"
#include "gwireframeskin.h"
#include "cengine.h"
#include "gcomponents.h"

void GWireFrameSkin::RenderWindow( GWindow* window ) 
{
	Engine.Draw.NoTexture();

	Engine.Draw.DrawRectangle(window->DrawRegion, TColors::Maroon);
	Engine.Draw.DrawRectangle(window->TitleBar->DrawRegion, TColors::Aquamarine);

	window->Font->Render(window->Text,window->TitleBar->DrawRegion,CA_MiddleLeft, TColors::Wheat,10 );

	//Engine.Draw.DrawQuad(window->ScreenRegion, TColors::aqua);
	Engine.Draw.Flush();
}

void GWireFrameSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetRectangle(0,0,window->Width,20);
	window->Content.SetRectangle(0,0,window->Width,window->Height);
}

void GWireFrameSkin::RenderLabel( GLabel* label )
{
	label->Font->Render(label->Text,label->DrawRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GWireFrameSkin::RenderSunkEdge( GObject* object )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(object->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderTextBox( GTextBox* textbox )
{
	RenderSunkEdge(textbox);
}

void GWireFrameSkin::RenderButton( GButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->DrawRegion, TColors::AntiqueWhite);
	button->Font->Render(button->Text,button->DrawRegion,CA_MiddleCenter,button->ForeColor);
}

void GWireFrameSkin::RenderMenuStrip( GMenuStrip* menustrip )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menustrip->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderDropDown( GDropDown* dropdown )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(dropdown->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutDropDown( GDropDown* dropdown )
{
	dropdown->Content.SetRectangle(0,0,dropdown->Width,dropdown->Height);
}

void GWireFrameSkin::RenderMenuItem( GMenuItem* menuItem )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menuItem->DrawRegion, TColors::Cyan);
	//menuItem->Font->Render(menuItem->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft, TColors::Wheat,10 );
}

void GWireFrameSkin::RenderMenuStripItem( GMenuItem* menuItem )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(menuItem->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutMenuStripItem( GMenuItem* menuItem )
{
	menuItem->Content.SetRectangle(0,0,menuItem->Width,menuItem->Height);
}

void GWireFrameSkin::RenderToolStrip( GToolStrip* toolbox )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(toolbox->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::RenderToolButton( GToolStripButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutToolButton( GToolStripButton* button )
{
	button->Content.SetRectangle(0,0,button->Width,button->Height);
}

void GWireFrameSkin::RenderScrollBarButton( GScrollBarButton* button )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(button->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutScrollBarButton( GScrollBarButton* button )
{
	button->Width = 16;
	button->Height = 16;
}

void GWireFrameSkin::RenderScrollBar( GScrollBar* scrollbar )
{
	Engine.Draw.NoTexture();
	Engine.Draw.DrawRectangle(scrollbar->DrawRegion, TColors::Cyan);
}

void GWireFrameSkin::LayoutScrollBar( GScrollBar* scrollbar )
{
	LayoutScrollBarButton(scrollbar->UpButton);
	LayoutScrollBarButton(scrollbar->DownButton);

	throw NotImplementedException();
}

void GWireFrameSkin::LayoutSunkEdge( GObject* object )
{
	object->Content.SetRectangle(0,0,object->Width,object->Height);
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

void GWireFrameSkin::LayoutTabPage( GTabPage* tabp )
{
	throw NotImplementedException();
}

void GWireFrameSkin::RenderTabButton( GButtonBase* tabb, bool active )
{
	throw NotImplementedException();
}

void GWireFrameSkin::RenderSystemButton( GSystemButton* sysb )
{
	throw NotImplementedException();
}

void GWireFrameSkin::LayoutSystemButton( GSystemButton* sysb )
{
	throw NotImplementedException();
}