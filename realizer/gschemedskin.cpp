#include "stdafx.h"
#include "gschemedskin.h"
#include "vtexture.h"
#include "tbitmap.h"
#include "cengine.h"


#include "gwindow.h"


void GSchemedSkin::RenderWindow( GWindow* window )
{
	WindowQuad->Render((GObject*)window);
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{

}