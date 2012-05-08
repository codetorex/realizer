#ifndef GSCHEMEDSKINBUILDER_H
#define GSCHEMEDSKINBUILDER_H

#include "gschemedskin.h"
#include <tiniparser.h>
#include "genums.h"
#include <tgraphics.h>

/**
 * Extends TINIClass functionality. Cant hold data!
 */
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
	TColor32 GetColor(const TString& key);
};

class GSchemeColor
{
public:
	int ColorID;
	TColor32 mColor;

	void LoadColor(GSchemeClass* cls);
};

class GSchemeFont
{
public:
	GSchemeFont()
	{
		LoadedFont = 0;
	}

	int FontID;

	TString FntName;
	int FntHeight;
	int FntWeight;

	int DrawingStyle; // I don't know how this is used.

	int ShadowR;
	int ShadowG;
	int ShadowB;

	int ShadowOffset;
	int ShadowOffsetY;

	GFont* LoadedFont;

	void LoadFontToEngine();

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

	GSchemeTextYAlign YAlign;
	GSchemeTextYAlign YAlignPressed;

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

	int UseOSFont;

	static ContentAlignment SchemeToContentAlign(const GSchemeTextXAlign& x,const GSchemeTextYAlign& y,ContentAlignment defaultvalue = CA_TopRight)
	{
		if (x == XA_Left)
		{
			switch(y)
			{
			case YA_Top:
				return CA_TopLeft;

			case YA_Center:
				return CA_MiddleLeft;

			case YA_Bottom:
				return CA_BottomLeft;
			}
		}

		if (x == XA_Center)
		{
			switch(y)
			{
			case YA_Top:
				return CA_TopCenter;

			case YA_Center:
				return CA_MiddleCenter;

			case YA_Bottom:
				return CA_BottomCenter;
			}
		}

		if (x == XA_Right)
		{
			switch(y)
			{
			case YA_Top:
				return CA_TopRight;

			case YA_Center:
				return CA_MiddleRight;

			case YA_Bottom:
				return CA_BottomRight;
			}
		}

		return defaultvalue;
	}

	ContentAlignment GetContentAlign(ContentAlignment defaultvalue = CA_TopRight) const
	{
		return SchemeToContentAlign(XAlign,YAlign,defaultvalue);
	}

	ContentAlignment GetPressedContentAlign(ContentAlignment defaultvalue = CA_TopRight) const
	{
		return SchemeToContentAlign(XAlignPressed,YAlignPressed,defaultvalue);
	}

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

	void LoadColorsClass(GSchemeClass* cls);
};

class GSchemeFile: public TINIParser
{
public:
	GSchemeClass* GetSchemeClass(const TString& className);
	GSchemeLayer GetLayer(const TString& className);
	GSchemeText GetTextLayer(const TString& className);
};



class GSchemedSkinBuilder
{
private:
	GSchemedSkin* Skin;
	TBitmap* SkinBitmap;
	TBitmapGraphics Gfx;

	TPackedRectangle* Pack;

	GSchemeFile* Scheme;
	GSchemeColors Colors;

	TArray< GSchemeColor* > NumberedColors;
	TArray< GSchemeFont* > NumberedFonts;
	TArray< GSchemeFont* > NumberedSystemFonts;

public:

	GSchemedSkinBuilder()
	{
		UsePerPixel = true;
		KeepPack = false;
	}

	enum FontKinds
	{
		FK_CUSTOMFONTS,
		FK_SYSTEMFONTS,
	};

	/**
	 * Creates a bitmap and pack structure and initializes them.
	 */
	void Begin(int w,int h, bool _keeppack = true);

	/**
	 * Uses existing bitmap and pack structure.
	 */
	void Begin( TBitmap* bmp, TPackedRectangle* pck);

	GSchemedSkin* Finish();

	VTexturePart* InsertImage(TBitmap* bmp);

	void LoadWindowTop          (const GSchemeText& borderData, bool corners);
	void LoadWindowBottom       (const GSchemeLayer& borderData, bool corners);
	void LoadWindowLeft         (const GSchemeLayer& borderData, bool corners);
	void LoadWindowRight        (const GSchemeLayer& borderData, bool corners);

	void LoadButtons            (const GSchemeText& buttonData);

	void LoadCheckBox	        (const GSchemeLayer& checkboxData);
	void LoadRadio		        (const GSchemeLayer& radioData);
	void LoadProgress	        (const GSchemeLayer& pbarbg, const GSchemeLayer& pbarblk);
	void LoadSunkEdge	        (const GSchemeLayer& sunkedgeData);

	void LoadMenuStripBackground(const GSchemeLayer& menubg);

	void LoadDropDown           (const GSchemeLayer& dropData );

	void LoadMenuItem			(const GSchemeLayer& menuItem);

	void LoadMenuStripItem		(const GSchemeLayer& menustripItem);

	void LoadToolStripBackground(const GSchemeLayer& toolbg);

	void LoadToolStripButton	(const GSchemeLayer& buttonData);

	void LoadScrollbarButtons	(const GSchemeLayer& buttonData);
	void LoadScrollbarBgH		(const GSchemeLayer& bgData);
	void LoadScrollbarBgV		(const GSchemeLayer& bgData);
	void LoadScrollbarDragH		(const GSchemeLayer& dragData);
	void LoadScrollbarDragV		(const GSchemeLayer& dragData);
	void LoadScrollbarDragHSmall(const GSchemeLayer& dragsData);
	void LoadScrollbarDragVSmall(const GSchemeLayer& dragsData);

	void LoadGeneric	        (const GSchemeLayer& data, int imagecount, VTexturePart* output);
	void LoadGeneric			(const GSchemeLayer& data, int imageCount, GScalableQuad* output);

	inline void LoadGeneric		(const GSchemeLayer& data, GScalableQuad* output)
	{
		LoadGeneric(data,1,output);
	}

	void SortQuads				(GScalableQuad* output, ui32* currentOrder, int imagecount);

	void LoadFontsAndColors();

	GSchemeFont* GetNumberedFont(int fontID, FontKinds fontKind );
	GFont* GetNumberedEngineFont(int fontID, FontKinds fontKind = FK_CUSTOMFONTS);

	bool UsePerPixel;

	/**
	 * Keeps pack structure.
	 */
	bool KeepPack;

	/**
	* Loads skin from a scheme file. Which is window blinds UIS file.
	* Note: this function will close the stream after usage.
	*/
	void LoadFromScheme(TStream* srcStream);
};


#endif


