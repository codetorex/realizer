#ifndef GSCHEMEDSKIN_H
#define GSCHEMEDSKIN_H

#include <tcolor.h>
#include <tstring.h>

#include "gskin.h"
#include "gquad.h"
#include "tpackedrectangle.h"


class VTexture;
class GFont;
class TBitmap;

// Skin class that can use window blinds skins.


class GSchemedSkin: public GSkin
{
public:
	VTexture* SkinTexture;

	GFont* BaseFont;
	TColor32 DefaultFontColor;

	VTexturePart WhitePart;

	GScalableQuadParted WindowQuad[2]; // 0= Active, 1= Inactive
	TColor32 ButtonFaceWindowBackgroundColor;

	GScalableQuad ButtonQuad[5]; // 0 = Normal, 1 = Pressed, 2 = Disabled, 3 = Mouse Over, 4 = Focus & Default


	void SaveSkin(const str8& path);
	void LoadSkin(const str8& path);

public:

	// Implementation
	void RenderWindow(GObject* window);
	void LayoutWindow(GWindow* window);

	void RenderButton(GButton* button);
};





#endif