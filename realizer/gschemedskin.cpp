#include "stdafx.h"
#include "gschemedskin.h"
#include "vtexture.h"
#include "tbitmap.h"
#include "cengine.h"
#include "gfont.h"

#include "gwindow.h"
#include "gbutton.h"


void GSchemedSkin::RenderWindow( GWindow* window )
{
	int windowState = window->Active ? 1 : 0;

	Engine.Draw.SetTexture(SkinTexture);
	WindowQuad[windowState].Render(window);
	
	// TODO: cache string pixel width in somewhere
	WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,WindowTitleAlign,WindowTitleColor[windowState] , WindowQuad->TopLeft.Width ,-1 );
	//WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft,WindowTitleColor[windowState] );
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetSize(0,0,window->Width,WindowQuad->Top.Height);
}

void GSchemedSkin::RenderButton( GButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ButtonQuad[button->GraphicState].Render(button);

	button->Font->Render(button->Text,button->ScreenRegion,button->TextAlign,button->ForeColor,0,-1);
}

void GSchemedSkin::RenderText( GFont* font, GObject* obj, ContentAlignment alignment )
{

}