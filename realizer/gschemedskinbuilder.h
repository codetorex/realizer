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

	/**
	* Throws exception if key not found.
	*/
	str8* GetMustValue(const str8& key);
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

public:

	void Begin(int w,int h);
	GSchemedSkin* Finish();

	VTexturePart* InsertImage(TBitmap* bmp);

	void LoadWindowTop   (const GSchemeText& borderData);
	void LoadWindowBottom(const GSchemeLayer& borderData);
	void LoadWindowLeft  (const GSchemeLayer& borderData);
	void LoadWindowRight (const GSchemeLayer& borderData);


	void LoadFromScheme(const str8& filePath, bool usePerPixel = true);
};


#endif


