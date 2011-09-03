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
	int GetInt(const TString& key, int defaultValue);
	
	/**
	* Returns TString::Empty if key not found.
	*/
	TString* GetValue(const TString& key);

	
	inline TString* GetValueOrNull(const TString& key)
	{
		TString* rv = Variables.GetValue(key);
		return rv;
	}

	/**
	* Throws exception if key not found.
	*/
	TString* GetMustValue(const TString& key);

	/**
	* Returns color, if not found, returns white.
	*/
	TColor32ARGB GetColor(const TString& key);
};

class GSchemeFont
{
public:
	TString* Name;
	TString* FontName;
	int FontHeight;
	int FontWidth;

	int ShadowR;
	int ShadowG;
	int ShadowB;

	void LoadFont(GSchemeClass* cls);
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

	TString* ImagePath;
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
	TColor32ARGB Scrollbar;
	TColor32ARGB ActiveTitle;
	TColor32ARGB InactiveTitle;
	TColor32ARGB Menu;
	TColor32ARGB Window;
	TColor32ARGB MenuText;
	TColor32ARGB WindowText;
	TColor32ARGB TitleText;
	TColor32ARGB ActiveBorder;
	TColor32ARGB InactiveBorder;
	TColor32ARGB AppWorkSpace;
	TColor32ARGB Hilight;
	TColor32ARGB HilightText;
	TColor32ARGB ButtonFace;
	TColor32ARGB ButtonShadow;
	TColor32ARGB GrayText;
	TColor32ARGB ButtonText;
	TColor32ARGB InactiveTitleText;
	TColor32ARGB ButtonHilight;
	TColor32ARGB ButtonDkShadow;
	TColor32ARGB ButtonLight;
	TColor32ARGB InfoText;
	TColor32ARGB InfoWindow;
	TColor32ARGB ButtonAlternateFace;
	TColor32ARGB HotTrackingColor;
	TColor32ARGB GradientActiveTitle;
	TColor32ARGB GradientInactiveTitle;
	TColor32ARGB MenuHilight;
	TColor32ARGB MenuBar;
	TColor32ARGB Background;
	TColor32ARGB WindowFrame;

	void LoadColors(GSchemeClass* cls);
};

class GSchemeFile: public TINIParser
{
public:
	GSchemeClass* GetClass(const TString& className);
	GSchemeLayer GetLayer(const TString& className);
	GSchemeText GetTextLayer(const TString& className);
};



class GSchemedSkinBuilder
{
private:
	GSchemedSkin* Skin;
	TBitmap* SkinBitmap;

	TPackedRectangle* Pack;

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


	/**
	* Loads skin from a scheme file. Which is window blinds UIS file.
	* Note: this function will close the stream after usage.
	*/
	void LoadFromScheme(TStream* srcStream, bool usePerPixel = true);
};


#endif


