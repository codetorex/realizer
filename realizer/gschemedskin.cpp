#include "stdafx.h"
#include "gschemedskin.h"
#include "vtexture.h"
#include "tbitmap.h"
#include "cengine.h"


#include "gwindow.h"
#include "gbutton.h"


void GSchemedSkin::RenderWindow( GObject* window )
{
	Engine.Draw.SetTexture(SkinTexture);
	WindowQuad[0].Render(window);
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetSize(0,0,window->Width,WindowQuad->Top.Height);
}

void GSchemedSkin::RenderButton( GButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ButtonQuad[button->GraphicState].Render(button);
}