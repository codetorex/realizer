#include "stdafx.h"
#include "gschemedskinbuilder.h"
#include "cengine.h"
#include "tconvert.h"


void GSchemedSkinBuilder::Begin( int w,int h )
{
	SkinBitmap = new TBitmap(w,h,TBitmapFormats::fARGB);
	Pack = new TPackedRectangle(w,h);
	Skin = new GSchemedSkin();
}

GSchemedSkin* GSchemedSkinBuilder::Finish()
{
	Skin->SkinTexture = Engine.Textures.CreateTexture(SkinBitmap);
	
	TStream* fs = Engine.FileSystem.Open("outputSKIN.bmp",fm_Write);
	SkinBitmap->WriteBMP(fs);

	return Skin;
}

int GSchemeClass::GetInt( const TString& key, int defaultValue = 0 )
{
	THashKeyValue<TString*>* kp = Variables.Get(key);
	if (kp == 0)
	{
		return defaultValue;
	}
	return Convert::ToInt32(*kp->Value);
}

TString* GSchemeClass::GetValue(const TString& key)
{
	TString* rv = Variables.GetValue(key);
	if (rv == NULL)
	{
		return &TString::Empty;
	}
	return rv;
}

TString* GSchemeClass::GetMustValue( const TString& key )
{
	TString* rv = Variables.GetValue(key);
	if (rv == NULL)
	{
		throw Exception("Value not found");
	}
	return rv;
}

TColor32 GSchemeClass::GetColor( const TString& key )
{
	TString* val = GetValueOrNull(key);
	if (val == NULL)
	{
		return TColor32(255,255,255);
	}

	int nStart = 0;
	int nLength = 0;
	int cRed = Convert::ToInt32Ambiguous(*val,nStart,&nLength); //val->ParseInt(nStart,&nLength);
	if (nLength == 0)
	{
		return TColor32(255,255,255);
	}
	nStart += nLength+1;
	int cGreen = Convert::ToInt32Ambiguous(*val,nStart,&nLength);
	if (nLength == 0)
	{
		return TColor32(255,255,255);
	}
	nStart += nLength+1;
	int cBlue = Convert::ToInt32Ambiguous(*val,nStart );
	if (nLength == 0)
	{
		return TColor32(255,255,255);
	}

	return TColor32(cRed,cGreen,cBlue);
}

GSchemeClass* GSchemeFile::GetClass( const TString& className )
{
	TINIClass* inicls = Classes.GetValue(className);
	return (GSchemeClass*)inicls;
}

void GSchemeLayer::LoadLayer( GSchemeClass* cls )
{
	ImagePath = cls->GetMustValue("image");
	LeftMargin = cls->GetInt("leftwidth");
	RightMargin = cls->GetInt("rightwidth");
	TopMargin = cls->GetInt("topheight");
	BottomMargin = cls->GetInt("bottomheight");
	PaintStyle = UseMargins;
	Tiling = (GSchemeTile)cls->GetInt("Tile");
}

void GSchemeLayer::CopyTo( GScalableQuad* qd ) const
{
	qd->LeftMargin = LeftMargin;
	qd->TopMargin = TopMargin;
	qd->RightMargin = RightMargin;
	qd->BottomMargin = BottomMargin;
}

void GSchemeText::LoadTextLayer( GSchemeClass* cls )
{
	XAlign = (GSchemeTextXAlign)cls->GetInt("texthorzalignnormal");
	XAlignPressed = (GSchemeTextXAlign)cls->GetInt("texthorzalignpressed");
	YAlign = (GSchemeTextYAlign)cls->GetInt("textvertalignnormal");
	YAlignPressed = (GSchemeTextYAlign)cls->GetInt("textvertalignpressed");

	TextAlpha = cls->GetInt("textalpha",255);
	TextAlphaInactive = cls->GetInt("textalphainactive",150);

	NormalColor = cls->GetInt("normalcolour",-1);
	PressedColor = cls->GetInt("pressedcolour",-1);
	DisabledColor = cls->GetInt("disabledcolour",-1);
	FocusColor = cls->GetInt("focuscolour",-1);
	DefaultColor = cls->GetInt("defaultcolour",-1);

	NormalFont = cls->GetInt("normalfont",-1);
	PressedFont = cls->GetInt("pressedfont",-1);
	DisabledFont = cls->GetInt("disabledfont",-1);
	FocusFont = cls->GetInt("focusfont",-1);
	DefaultFont = cls->GetInt("defaultfont",-1);

	UseOSFont = cls->GetInt("useosfont",0);

	ContentLeft = cls->GetInt("contentleft",0);
	ContentRight = cls->GetInt("contentright",0);
	ContentTop = cls->GetInt("contenttop",0);
	ContentBottom = cls->GetInt("contentbottom",0);
}

GSchemeLayer GSchemeFile::GetLayer( const TString& className )
{
	GSchemeLayer lyr;
	GSchemeClass* cls = GetClass(className);
	if (cls == 0)
	{
		throw Exception("Class not found");
	}
	lyr.LoadLayer(cls);
	return lyr;
}

GSchemeText GSchemeFile::GetTextLayer( const TString& className )
{
	GSchemeText lyr;
	GSchemeClass* cls = GetClass(className);
	if (cls == 0)
	{
		throw Exception("Class not found");
	}
	lyr.LoadLayer(cls);
	lyr.LoadTextLayer(cls);
	return lyr;
}

void GSchemedSkinBuilder::LoadFromScheme( TStream* srcStream, bool usePerPixel )
{
	Scheme = (GSchemeFile*) new TINIParser(srcStream);
	Scheme->LowerCaseKeys = true;
	Scheme->Parse(); // this guy closes and deletes the stream so don't need to be closed later

	// TODO: do lots of stuff here
	
	// Lets load window images
	if ( !Scheme->ContainsClass("windowframe.topperpixel") && usePerPixel )
	{
		usePerPixel = false;
	}

	// This optimization allows rendering "just" color things easily without changing texture.
	dword white = 0xFFFFFFFF;
	TRectangleNode* whiteNode = Pack->Insert(16,16);
	SkinBitmap->DrawRectangle(whiteNode->X,whiteNode->Y,16,16,(byte*)&white);
	VTexturePart* whitePart = new VTexturePart( SkinBitmap, whiteNode );
	Skin->WhitePart = *whitePart;

	LoadFontsAndColors();

	if (usePerPixel)
	{
		LoadWindowTop(Scheme->GetTextLayer("windowframe.topperpixel"),true);
		LoadWindowBottom(Scheme->GetLayer("windowframe.bottomperpixel"),true);
		LoadWindowLeft(Scheme->GetLayer("windowframe.leftperpixel"),false);
		LoadWindowRight(Scheme->GetLayer("windowframe.rightperpixel"),false);
	}
	else
	{

	}

	Skin->WindowQuad[0].Center.Initialize(whitePart);
	Skin->WindowQuad[0].CenterColor = Colors.ButtonFace;
	Skin->WindowQuad[1].Center.Initialize(whitePart);
	Skin->WindowQuad[1].CenterColor = Colors.ButtonFace;
	Skin->ButtonFaceWindowBackgroundColor = Colors.ButtonFace;

	LoadButtons(Scheme->GetTextLayer("buttons"));

	// TODO: load default font, code GSchemeFont loadFont, loadFont from SYSTEMFONT0 class
	// TODO: lower case stuff of classes, implement it to TINIParser
}

void GSchemeFont::LoadFont( GSchemeClass* cls )
{
	FntName = *cls->GetValue("fontname");
	FntHeight = cls->GetInt("fontheight",9);
	FntWeight = cls->GetInt("fontweight",400);

	DrawingStyle = cls->GetInt("drawingstyle",1);

	ShadowR = cls->GetInt("shadowr",0);
	ShadowG = cls->GetInt("shadowg",0);
	ShadowB = cls->GetInt("shadowb",0);

	ShadowOffset = cls->GetInt("shadowoffset",0);
	ShadowOffsetY = cls->GetInt("shadowoffsety",0);
}

void GSchemeFont::LoadFontToEngine()
{
	LoadedFont = Engine.GUI.Fonts.GetFont(FntName,FntHeight,(FontWeight)FntWeight);
	if (LoadedFont == 0)
	{
		throw Exception("Scheme font cant loaded");
	}
}

void GSchemedSkinBuilder::LoadFontsAndColors()
{
	THashMapEnumerator< TINIClass* > classenum(&Scheme->Classes);
	while(classenum.MoveNext())
	{
		TINIClass* curClass = classenum.Current->Value;

		if (curClass->Name.StartsWith("colour"))
		{
			if (curClass->Name.GetLast() == 's') // Loading colours class is another story, so just do it and get back here.
			{
				Colors.LoadColorsClass((GSchemeClass*)curClass);
				continue;
			}

			int colorId = Convert::ToInt32Ambiguous(curClass->Name, 5 );
			GSchemeColor* newColor = new GSchemeColor();
			newColor->ColorID = colorId;
			newColor->LoadColor((GSchemeClass*)curClass);

			NumberedColors.Add(newColor);
			continue;
		}

		if (curClass->Name.StartsWith("systemfont"))
		{
			if (curClass->Name.GetLast() == 's') // Loading colours class is another story, so just do it and get back here.
			{
				continue;
			}
			
			GSchemeFont* newFont = new GSchemeFont();
			newFont->LoadFont((GSchemeClass*)curClass);
			newFont->FontID = Convert::ToInt32Ambiguous(curClass->Name, 9 );
			NumberedSystemFonts.Add(newFont);
			continue;
		}

		if (curClass->Name.StartsWith("font"))
		{
			if (curClass->Name.Data[4] == 's')
			{
				continue;
			}
			GSchemeFont* newFont = new GSchemeFont();
			newFont->LoadFont((GSchemeClass*)curClass);
			newFont->FontID = Convert::ToInt32Ambiguous(curClass->Name, 3 );
			NumberedFonts.Add(newFont);
			continue;
		}
	}

	Skin->SystemFont = GetNumberedEngineFont(0,FK_SYSTEMFONTS);
	Skin->DefaultFontColor = Colors.ButtonText;
	Skin->WindowTitleColor[0] = Colors.TitleText;
	Skin->WindowTitleColor[1] = Colors.InactiveTitleText;
}

void GSchemedSkinBuilder::LoadWindowTop( const GSchemeText& borderData, bool corners )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*borderData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int halfHeight = sub->Height/2;

	if (corners)
	{
		Skin->WindowQuad[0].TopLeft.InitializeRelative(SkinBitmap,sub,0,0,borderData.LeftMargin,halfHeight);
		Skin->WindowQuad[0].Top.InitializeRelative(SkinBitmap,sub,borderData.LeftMargin,0,sub->Width-borderData.LeftMargin - borderData.RightMargin,halfHeight);
		Skin->WindowQuad[0].TopRight.InitializeRelative(SkinBitmap,sub, sub->Width - borderData.RightMargin,0,borderData.RightMargin,halfHeight);

		Skin->WindowQuad[1].TopLeft.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].TopLeft,0,halfHeight);
		Skin->WindowQuad[1].Top.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Top,0,halfHeight);
		Skin->WindowQuad[1].TopRight.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].TopRight,0,halfHeight);
	}
	else
	{
		Skin->WindowQuad[0].Top.InitializeRelative(SkinBitmap,sub,0,0,sub->Width,halfHeight);
		Skin->WindowQuad[1].Top.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Top,0,halfHeight);
	}

	Skin->WindowTitleFont = GetNumberedEngineFont(borderData.NormalFont);
	Skin->WindowTitleAlign = borderData.GetContentAlign(CA_MiddleLeft);
	//Skin->WindowTopFont[0] = GetNumberedEngineFont(borderData.NormalFont);
	//Skin->WindowTopFont[1] = GetNumberedEngineFont(borderData.PressedFont);

	delete image;
}

void GSchemedSkinBuilder::LoadWindowBottom( const GSchemeLayer& borderData , bool corners)
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*borderData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int halfHeight = sub->Height/2;
	if (corners)
	{
		Skin->WindowQuad[0].BottomLeft.InitializeRelative(SkinBitmap,sub,0,0,borderData.LeftMargin,halfHeight);
		Skin->WindowQuad[0].Bottom.InitializeRelative(SkinBitmap,sub,borderData.LeftMargin,0,sub->Width-borderData.LeftMargin - borderData.RightMargin,halfHeight);
		Skin->WindowQuad[0].BottomRight.InitializeRelative(SkinBitmap,sub, sub->Width - borderData.RightMargin,0,borderData.RightMargin,halfHeight);

		Skin->WindowQuad[1].BottomLeft.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].BottomLeft,0,halfHeight);
		Skin->WindowQuad[1].Bottom.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Bottom,0,halfHeight);
		Skin->WindowQuad[1].BottomRight.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].BottomRight,0,halfHeight);
	}
	else
	{
		Skin->WindowQuad[0].Bottom.InitializeRelative(SkinBitmap,sub,0,0,sub->Width,halfHeight);
		Skin->WindowQuad[1].Bottom.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Bottom,0,halfHeight);
	}

	delete image;
}

void GSchemedSkinBuilder::LoadWindowLeft( const GSchemeLayer& borderData, bool corners )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*borderData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int halfWidth = sub->Width/2;
	if (corners)
	{
		Skin->WindowQuad[0].TopLeft.InitializeRelative(SkinBitmap,sub,0,0,halfWidth,borderData.TopMargin);
		Skin->WindowQuad[0].Left.InitializeRelative(SkinBitmap,sub,0,borderData.TopMargin,halfWidth,sub->Height-borderData.TopMargin-borderData.BottomMargin);
		Skin->WindowQuad[0].BottomLeft.InitializeRelative(SkinBitmap,sub,0,sub->Height-borderData.BottomMargin,halfWidth,borderData.BottomMargin);

		Skin->WindowQuad[1].TopLeft.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].TopLeft,halfWidth,0);
		Skin->WindowQuad[1].Left.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Left,halfWidth,0);
		Skin->WindowQuad[1].BottomLeft.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].BottomLeft,halfWidth,0);
	}
	else
	{
		Skin->WindowQuad[0].Left.InitializeRelative(SkinBitmap,sub,0,0,halfWidth,sub->Height);
		Skin->WindowQuad[1].Left.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Left,halfWidth,0);
	}

	delete image;
}

void GSchemedSkinBuilder::LoadWindowRight( const GSchemeLayer& borderData , bool corners)
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*borderData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int halfWidth = sub->Width/2;
	if (corners)
	{
		Skin->WindowQuad[0].TopRight.InitializeRelative(SkinBitmap,sub,0,0,halfWidth,borderData.TopMargin);
		Skin->WindowQuad[0].Right.InitializeRelative(SkinBitmap,sub,0,borderData.TopMargin,halfWidth,sub->Height-borderData.TopMargin-borderData.BottomMargin);
		Skin->WindowQuad[0].BottomRight.InitializeRelative(SkinBitmap,sub,0,sub->Height-borderData.BottomMargin,halfWidth,borderData.BottomMargin);

		Skin->WindowQuad[1].TopRight.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].TopRight,halfWidth,0);
		Skin->WindowQuad[1].Right.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Right,halfWidth,0);
		Skin->WindowQuad[1].BottomRight.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].BottomRight,halfWidth,0);
	}
	else
	{
		Skin->WindowQuad[0].Right.InitializeRelative(SkinBitmap,sub,0,0,halfWidth,sub->Height);
		Skin->WindowQuad[1].Right.InitializeOffset(SkinBitmap,&Skin->WindowQuad[0].Right,halfWidth,0);
	}

	delete image;
}

void GSchemedSkinBuilder::LoadButtons( const GSchemeText& buttonData )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*buttonData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int partWidth = sub->Width / 5; // there is 5 different pictures
	TRegion tmpRegion;
	tmpRegion.SetFrom( sub );
	tmpRegion.SetWidth( partWidth );
	for (int i=0;i<5;i++)
	{
		buttonData.CopyTo(&Skin->ButtonQuad[i]);
		Skin->ButtonQuad[i].Initialize(SkinBitmap, tmpRegion);
		tmpRegion.SetLeftRelative(partWidth); // increment x of region
	}

	if (buttonData.UseOSFont == 1)
	{
		Skin->ButtonFont = Skin->SystemFont;
	}
	else
	{
		Skin->ButtonFont = GetNumberedEngineFont(buttonData.NormalFont);
	}

	/*if (buttonData.UseOSFont == 1)
	{
		GFont* systemFont = GetNumberedEngineFont(0,FK_SYSTEMFONTS);
		for (int i=0;i<5;i++)
		{
			Skin->ButtonFont[i] = systemFont;
		}
	}
	else
	{
		Skin->ButtonFont[0] = GetNumberedEngineFont(buttonData.NormalFont);
		Skin->ButtonFont[1] = GetNumberedEngineFont(buttonData.PressedFont);
		Skin->ButtonFont[2] = GetNumberedEngineFont(buttonData.DisabledFont);
		Skin->ButtonFont[3] = GetNumberedEngineFont(buttonData.FocusFont);
		Skin->ButtonFont[4] = GetNumberedEngineFont(buttonData.DefaultFont);
	}*/

	delete image;
}

VTexturePart* GSchemedSkinBuilder::InsertImage( TBitmap* bmp )
{
	TRectangleNode* node = Pack->Insert(bmp);
	if (node == 0)
	{
		throw Exception("Texture is not big enough, implement multi texture method");
	}

	SkinBitmap->Copy(bmp,node);
	VTexturePart* tpart = new VTexturePart( SkinBitmap, node );
	return tpart;
}

GSchemeFont* GSchemedSkinBuilder::GetNumberedFont( int fontID, FontKinds fontKind )
{
	
	TArray<GSchemeFont*>& fontArray = fontKind == FK_CUSTOMFONTS ? NumberedFonts : NumberedSystemFonts;
	for (int i=0;i<fontArray.Count;i++)
	{
		GSchemeFont* curFont = fontArray.Item[i];

		if (curFont->FontID == fontID)
		{
			return curFont;
		}
	}
	return 0;
}

GFont* GSchemedSkinBuilder::GetNumberedEngineFont( int fontID , FontKinds fontKind )
{
	GSchemeFont* curFont = GetNumberedFont(fontID,fontKind);
	if (curFont)
	{
		if (curFont->LoadedFont == 0)
		{
			curFont->LoadFontToEngine();
		}
		return curFont->LoadedFont;
	}

	throw Exception("Font not found");
}


void GSchemeColors::LoadColorsClass( GSchemeClass* cls )
{
	Scrollbar             = cls->GetColor("scrollbar");
	ActiveTitle           = cls->GetColor("activetitle");
	InactiveTitle         = cls->GetColor("inactivetitle");
	Menu                  = cls->GetColor("menu");
	Window                = cls->GetColor("window");
	MenuText              = cls->GetColor("menutext");
	WindowText            = cls->GetColor("windowtext");
	TitleText             = cls->GetColor("titletext");
	ActiveBorder          = cls->GetColor("activeborder");
	InactiveBorder        = cls->GetColor("inactiveborder");
	AppWorkSpace          = cls->GetColor("appworkspace");
	Hilight               = cls->GetColor("hilight");
	HilightText           = cls->GetColor("hilighttext");
	ButtonFace            = cls->GetColor("buttonface");
	ButtonShadow          = cls->GetColor("buttonshadow");
	GrayText              = cls->GetColor("graytext");
	ButtonText            = cls->GetColor("buttontext");
	InactiveTitleText     = cls->GetColor("inactivetitletext");
	ButtonHilight         = cls->GetColor("buttonhilight");
	ButtonDkShadow        = cls->GetColor("buttondkshadow");
	ButtonLight           = cls->GetColor("buttonlight");
	InfoText              = cls->GetColor("infotext");
	InfoWindow            = cls->GetColor("infowindow");
	ButtonAlternateFace   = cls->GetColor("buttonalternateface");
	HotTrackingColor      = cls->GetColor("hottrackingcolor");
	GradientActiveTitle   = cls->GetColor("gradientactivetitle");
	GradientInactiveTitle = cls->GetColor("gradientinactivetitle");
	MenuHilight           = cls->GetColor("menuhilight");
	MenuBar               = cls->GetColor("menubar");
	Background            = cls->GetColor("background");
	WindowFrame           = cls->GetColor("windowframe");
}

void GSchemeColor::LoadColor( GSchemeClass* cls )
{
	if ( cls->Variables.ContainsKey("colour") )
	{
		mColor = cls->GetColor("colour");
	}
	else
	{
		mColor.r = cls->GetInt("r",0);
		mColor.g = cls->GetInt("g",0);
		mColor.b = cls->GetInt("b",0);
		mColor.a = 255;
	}
}


