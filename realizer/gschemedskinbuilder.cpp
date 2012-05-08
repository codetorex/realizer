#include "stdafx.h"
#include "gschemedskinbuilder.h"
#include "cengine.h"
#include "tconvert.h"


void GSchemedSkinBuilder::Begin( int w,int h , bool _keeppack )
{
	SkinBitmap = new TBitmap(w,h);
	Gfx.Initialize(SkinBitmap);
	Pack = new TPackedRectangle(w,h);
	Skin = new GSchemedSkin();
	KeepPack = _keeppack;
	Gfx.Clear(0);
}

void GSchemedSkinBuilder::Begin( TBitmap* bmp, TPackedRectangle* pck )
{
	SkinBitmap = bmp;
	Gfx.Initialize(SkinBitmap);
	Pack = pck;
	Skin = new GSchemedSkin();
}

GSchemedSkin* GSchemedSkinBuilder::Finish()
{
	Skin->SkinTexture = Engine.Textures.CreateTexture(SkinBitmap);

	if (KeepPack)
	{
		Skin->Pack = Pack;
	}
	
	return Skin;

	// delete this?
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

GSchemeClass* GSchemeFile::GetSchemeClass( const TString& className )
{
	TINIClass* inicls = GetClass(className);
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
	GSchemeClass* cls = GetSchemeClass(className);
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
	GSchemeClass* cls = GetSchemeClass(className);
	if (cls == 0)
	{
		throw Exception("Class not found");
	}
	lyr.LoadLayer(cls);
	lyr.LoadTextLayer(cls);
	return lyr;
}

#include "tgraphics.h"

void GSchemedSkinBuilder::LoadFromScheme( TStream* srcStream )
{
	Scheme = (GSchemeFile*) new TINIParser(srcStream);
	Scheme->LowerCaseKeys = true;
	Scheme->Parse(); // this guy closes and deletes the stream so don't need to be closed later

	// TODO: do lots of stuff here
	
	// Lets load window images
	if ( !Scheme->ContainsClass("windowframe.topperpixel") && UsePerPixel )
	{
		UsePerPixel = false;
	}

	// This optimization allows rendering "just" color things easily without changing texture.
	//ui32 white = 0xFFFFFFFF;
	TRectangleNode* whiteNode = Pack->Insert(16,16);
	Gfx.FillRectangle(TBrush(TColors::White),whiteNode->X,whiteNode->Y,16,16);
	VTexturePart* whitePart = new VTexturePart( SkinBitmap, whiteNode );
	Skin->WhitePart = *whitePart;

	LoadFontsAndColors();

	if (UsePerPixel)
	{
		LoadWindowTop(Scheme->GetTextLayer("windowframe.topperpixel"),true);
		LoadWindowBottom(Scheme->GetLayer("windowframe.bottomperpixel"),true);
		LoadWindowLeft(Scheme->GetLayer("windowframe.leftperpixel"),false);
		LoadWindowRight(Scheme->GetLayer("windowframe.rightperpixel"),false);
	}
	else
	{

	}

	Skin->WindowQuad[0].Center.Initialize(*whitePart);
	Skin->WindowQuad[0].CenterColor = Colors.ButtonFace;
	Skin->WindowQuad[1].Center.Initialize(*whitePart);
	Skin->WindowQuad[1].CenterColor = Colors.ButtonFace;
	Skin->ButtonFaceWindowBackgroundColor = Colors.ButtonFace;

	LoadButtons            (Scheme->GetTextLayer("buttons"));

	LoadCheckBox           (Scheme->GetLayer("button.checkbox"));
	LoadRadio              (Scheme->GetLayer("button.radio"));
	LoadProgress           (Scheme->GetLayer("progressxp.horzedge"),Scheme->GetLayer("progressxp.horzblock"));
	LoadSunkEdge           (Scheme->GetLayer("sunkedge"));
	LoadMenuStripBackground(Scheme->GetLayer("menubar.background"));
	LoadDropDown           (Scheme->GetLayer("menubackground"));
	LoadMenuItem           (Scheme->GetLayer("menuitem"));
	LoadMenuStripItem      (Scheme->GetLayer("menubar"));
	LoadToolStripBackground(Scheme->GetLayer("toolbarbackground"));
	LoadToolStripButton    (Scheme->GetLayer("toolbar.normal"));
	LoadToolStripButton    (Scheme->GetLayer("toolbar.normal"));

	LoadScrollbarButtons   (Scheme->GetLayer("scrollbar"));
	LoadScrollbarBgH       (Scheme->GetLayer("horzscroll"));
	LoadScrollbarBgV       (Scheme->GetLayer("vertscroll"));
	LoadScrollbarDragH     (Scheme->GetLayer("horzscrollthumb"));
	LoadScrollbarDragV     (Scheme->GetLayer("vertscrollthumb"));
	LoadScrollbarDragHSmall(Scheme->GetLayer("smallhscrollthumb"));
	LoadScrollbarDragVSmall(Scheme->GetLayer("smallvscrollthumb"));

	// TODO: load default font, code GSchemeFont loadFont, loadFont from SYSTEMFONT0 class
}


void GSchemedSkinBuilder::LoadSunkEdge( const GSchemeLayer& sunkedgeData )
{
	LoadGeneric(sunkedgeData,4,Skin->SunkEdge);
}

void GSchemedSkinBuilder::LoadProgress( const GSchemeLayer& pbarbg, const GSchemeLayer& pbarblk )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*pbarbg.ImagePath);
	VTexturePart* sub = InsertImage(image);

	TRegion tmpRegion;
	tmpRegion.SetFrom( sub );

	pbarbg.CopyTo(&Skin->ProgressBarBg);
	Skin->ProgressBarBg.Initialize(SkinBitmap,tmpRegion);

	delete image;

	image = Engine.Textures.LoadToBitmap(*pbarblk.ImagePath);
	sub = InsertImage(image);

	tmpRegion.SetFrom( sub );

	pbarblk.CopyTo(&Skin->ProgressBarBlock);
	Skin->ProgressBarBlock.Initialize(SkinBitmap,tmpRegion);

	delete image;
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

void GSchemedSkinBuilder::LoadCheckBox( const GSchemeLayer& checkboxData )
{
	VTexturePart checkbox[12];
	LoadGeneric(checkboxData,12,checkbox);

	for (int i= 0;i<3;i++)
	{
		ui32 order[] = {GBG_NORMAL,GBG_OVER, GBG_DOWN,GBG_DISABLED};
		Skin->CheckBoxGfx[i].Load(checkbox + (i * 4),order,4);
	}
}


void GSchemedSkinBuilder::LoadRadio( const GSchemeLayer& radioData )
{
	VTexturePart radio[8];
	LoadGeneric(radioData,8,radio);

	for (int i= 0;i<2;i++)
	{
		ui32 order[] = {GBG_NORMAL,GBG_OVER, GBG_DOWN,GBG_DISABLED};
		Skin->RadioGfx[i].Load(radio + (i * 4),order,4);
	}
}

void GSchemedSkinBuilder::LoadGeneric( const GSchemeLayer& data, int imagecount, VTexturePart* output )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*data.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int partWidth = sub->Width / imagecount;
	TRegion tmpRegion;
	tmpRegion.SetFrom( sub );
	tmpRegion.SetWidth(partWidth);

	for (int i=0;i<imagecount;i++)
	{
		output[i].Initialize(SkinBitmap, tmpRegion);
		tmpRegion.SetLeftRelative(partWidth);
	}

	delete image;
}

void GSchemedSkinBuilder::LoadGeneric( const GSchemeLayer& data, int imageCount, GScalableQuad* output )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*data.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int partWidth = sub->Width / imageCount; // there is 5 different pictures
	TRegion tmpRegion;
	tmpRegion.SetFrom( sub );
	tmpRegion.SetWidth( partWidth );
	for (int i=0;i<imageCount;i++)
	{
		data.CopyTo(&output[i]);
		output[i].Initialize(SkinBitmap, tmpRegion);
		tmpRegion.SetLeftRelative(partWidth); // increment x of region
	}

	delete image;
}

void GSchemedSkinBuilder::LoadMenuStripBackground( const GSchemeLayer& menubg )
{
	LoadGeneric(menubg,2, Skin->MenuBarBg);
}


void GSchemedSkinBuilder::LoadToolStripBackground( const GSchemeLayer& toolbg )
{
	LoadGeneric(toolbg,&Skin->ToolBarBg);
}

void GSchemedSkinBuilder::LoadDropDown( const GSchemeLayer& dropData )
{
	LoadGeneric(dropData,&Skin->DropDownBg);
}


void GSchemedSkinBuilder::LoadMenuItem( const GSchemeLayer& menuItem )
{
	LoadGeneric(menuItem,5,Skin->MenuItemBg);
}

void GSchemedSkinBuilder::LoadMenuStripItem( const GSchemeLayer& menustripItem )
{
	LoadGeneric(menustripItem,5,Skin->MenuBarButton);
}

void GSchemedSkinBuilder::LoadToolStripButton( const GSchemeLayer& buttonData )
{
	LoadGeneric(buttonData,6,Skin->ToolButton);
}

void GSchemedSkinBuilder::LoadButtons( const GSchemeText& buttonData )
{
	GScalableQuad butquads[5];
	LoadGeneric(buttonData,5,butquads);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN, GBG_DISABLED, GBG_OVER,GBG_FOCUSED};
	Skin->ButtonGfx.Load(butquads,order,5);

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
}

VTexturePart* GSchemedSkinBuilder::InsertImage( TBitmap* bmp )
{
	TRectangleNode* node = Pack->Insert(bmp);
	if (node == 0)
	{
		throw Exception("Texture is not big enough, implement multi texture method");
	}

	/*
	 *NO need to convert since they are converted in graphics
	 *
	 *if (bmp->BufferFormat != SkinBitmap->BufferFormat)
	{
		bmp->Convert(SkinBitmap->BufferFormat);
	}*/

	Gfx.DrawImage(*bmp,*(TPosition*)node);
	VTexturePart* tpart = new VTexturePart( SkinBitmap, node );
	return tpart;
}

GSchemeFont* GSchemedSkinBuilder::GetNumberedFont( int fontID, FontKinds fontKind )
{
	
	TArray<GSchemeFont*>& fontArray = fontKind == FK_CUSTOMFONTS ? NumberedFonts : NumberedSystemFonts;
	for (ui32 i=0;i<fontArray.Count;i++)
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

void GSchemedSkinBuilder::LoadScrollbarButtons( const GSchemeLayer& buttonData )
{
	VTexturePart sbut[23];
	LoadGeneric(buttonData,23,sbut);

	for (int i= 0;i<6;i++)
	{
		ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
		Skin->ScrollbarButtonGfx[i].Load(sbut + (i * 4),order,4);
	}

	Skin->SizingCorner.Initialize(sbut[22]);
}

void GSchemedSkinBuilder::LoadScrollbarBgH( const GSchemeLayer& bgData )
{
	GScalableQuad sbg[4];
	LoadGeneric(bgData,4,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
	Skin->ScrollbarBgHGfx.Load(sbg,order,4);
}

void GSchemedSkinBuilder::LoadScrollbarBgV( const GSchemeLayer& bgData )
{
	GScalableQuad sbg[4];
	LoadGeneric(bgData,4,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
	Skin->ScrollbarBgVGfx.Load(sbg,order,4);
}

void GSchemedSkinBuilder::LoadScrollbarDragH( const GSchemeLayer& dragData )
{
	GScalableQuad sbg[3];
	LoadGeneric(dragData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragHGfx.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadScrollbarDragV( const GSchemeLayer& dragData )
{
	GScalableQuad sbg[3];
	LoadGeneric(dragData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragVGfx.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadScrollbarDragHSmall( const GSchemeLayer& dragsData )
{
	GScalableQuad sbg[3];
	LoadGeneric(dragsData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragSmallHGfx.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadScrollbarDragVSmall( const GSchemeLayer& dragsData )
{
	GScalableQuad sbg[3];
	LoadGeneric(dragsData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragSmallVGfx.Load(sbg,order,3);
}
