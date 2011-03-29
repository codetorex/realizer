#ifndef GSCHEMEDSKINBUILDER_H
#define GSCHEMEDSKINBUILDER_H

#include "gschemedskin.h"
#include <tiniparser.h>

class GSchemeClass: public TINIClass
{
public:

	/**
	* Returns default value if key not found.
	*/
	int GetInt(const str8& key, int defaultValue);
	
	/**
	* Returns str8::Empty if key not found.
	*/
	str8* GetValue(const str8& key);

	
	inline str8* GetValueOrNull(const str8& key)
	{
		str8* rv = Variables.GetValue(key);
		return rv;
	}

	/**
	* Throws exception if key not found.
	*/
	str8* GetMustValue(const str8& key);

	/**
	* Returns color, if not found, returns white.
	*/
	TColor32 GetColor(const str8& key);
};

class GSchemeLayer
{
public:
	enum GSchemePaintStyle
	{
		UseMargins,
		StretchVertical, // not implemented
		StretchHorizontal, // not implemented
		Centre, // not implemented
	};

	enum GSchemeTile
	{
		T_Stretch, // other than this maybe not implemented
		T_TileMiddle,
		T_TileHorizontal,
		T_StretchHorizontalMiddle,
		T_CentreMiddle,
		T_DontPaintMiddle,
	};

	GSchemePaintStyle PaintStyle;
	GSchemeTile Tiling;

	str8* ImagePath;
	int LeftMargin;
	int RightMargin;
	int TopMargin;
	int BottomMargin;

	inline bool IsVertical() const
	{
		return ((TopMargin + BottomMargin) > 0);
	}

	inline bool IsHorizontal() const
	{
		return ((LeftMargin + RightMargin) > 0);
	}

	void CopyTo(GScalableQuad* qd) const;
	void LoadLayer(GSchemeClass* cls);
};



class GSchemeText: public GSchemeLayer
{
public:
	enum GSchemeTextXAlign
	{
		XA_OSDefault,
		XA_Left,
		XA_Center,
		XA_Right,
	};

	enum GSchemeTextYAlign
	{
		YA_OSDefault,
		YA_Top,
		YA_Center,
		YA_Bottom, 
	};

	GSchemeTextXAlign XAlign;
	GSchemeTextXAlign XAlignPressed;

	GSchemeTextXAlign YAlign;
	GSchemeTextXAlign YAlignPressed;

	int TextAlpha;
	int TextAlphaInactive;

	int NormalColor;
	int PressedColor;
	int DisabledColor;
	int FocusColor;
	int DefaultColor;

	int NormalFont;
	int PressedFont;
	int DisabledFont;
	int FocusFont;
	int DefaultFont;

	int ContentLeft;
	int ContentRight;
	int ContentTop;
	int ContentBottom;

	void LoadTextLayer(GSchemeClass* cls);
};

/**
* Used for holding system color info of skin.
*/
class GSchemeColors
{
public:
	TColor32 Scrollbar;
	TColor32 ActiveTitle;
	TColor32 InactiveTitle;
	TColor32 Menu;
	TColor32 Window;
	TColor32 MenuText;
	TColor32 WindowText;
	TColor32 TitleText;
	TColor32 ActiveBorder;
	TColor32 InactiveBorder;
	TColor32 AppWorkSpace;
	TColor32 Hilight;
	TColor32 HilightText;
	TColor32 ButtonFace;
	TColor32 ButtonShadow;
	TColor32 GrayText;
	TColor32 ButtonText;
	TColor32 InactiveTitleText;
	TColor32 ButtonHilight;
	TColor32 ButtonDkShadow;
	TColor32 ButtonLight;
	TColor32 InfoText;
	TColor32 InfoWindow;
	TColor32 ButtonAlternateFace;
	TColor32 HotTrackingColor;
	TColor32 GradientActiveTitle;
	TColor32 GradientInactiveTitle;
	TColor32 MenuHilight;
	TColor32 MenuBar;
	TColor32 Background;
	TColor32 WindowFrame;

	void LoadColors(GSchemeClass* cls);
};

class GSchemeFile: public TINIParser
{
public:
	GSchemeClass* GetClass(const str8& className);
	GSchemeLayer GetLayer(const str8& className);
	GSchemeText GetTextLayer(const str8& className);
};



class GSchemedSkinBuilder
{
private:
	GSchemedSkin* Skin;
	TBitmap* SkinBitmap;

	TPackedRectangle* Pack;
	GFont* BaseFont;

	GSchemeFile* Scheme;
	GSchemeColors Colors;

public:

	void Begin(int w,int h);
	GSchemedSkin* Finish();

	VTexturePart* InsertImage(TBitmap* bmp);

	void LoadWindowTop   (const GSchemeText& borderData, bool corners);
	void LoadWindowBottom(const GSchemeLayer& borderData, bool corners);
	void LoadWindowLeft  (const GSchemeLayer& borderData, bool corners);
	void LoadWindowRight (const GSchemeLayer& borderData, bool corners);

	void LoadButtons     (const GSchemeText& buttonData);



	void LoadFromScheme(const str8& filePath, bool usePerPixel = true);
};


#endif


