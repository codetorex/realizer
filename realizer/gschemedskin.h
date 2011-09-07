#ifndef GSCHEMEDSKIN_H
#define GSCHEMEDSKIN_H

#include <tcolor.h>
#include <tstring.h>

#include "gskin.h"
#include "gquad.h"
#include "tpackedrectangle.h"
#include "genums.h"

class VTexture;
class GFont;
class TBitmap;

// Skin class that can use window blinds skins.


class GSchemedSkin: public GSkin
{
public:
	VTexture* SkinTexture;

	VTexturePart WhitePart;

	GScalableQuadParted WindowQuad[2]; // 0= Active, 1= Inactive
	TColor32 ButtonFaceWindowBackgroundColor;
	GFont* WindowTitleFont; // normal font, pressed font
	ContentAlignment WindowTitleAlign;
	TColor32 WindowTitleColor[2];

	GScalableQuad ButtonQuad[5]; // 0 = Normal, 1 = Pressed, 2 = Disabled, 3 = Mouse Over, 4 = Focus & Default
	GFont* ButtonFont;

	void SaveSkin(const TString& path);
	void LoadSkin(const TString& path);

	void RenderText(GFont* font, GObject* obj, ContentAlignment alignment = CA_MiddleCenter );

public:

	// Implementation
	void RenderWindow(GWindow* window);
	void LayoutWindow(GWindow* window);

	void RenderButton(GButton* button);
};





#endif