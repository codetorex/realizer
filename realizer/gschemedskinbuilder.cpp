#include "stdafx.h"
#include "gschemedskinbuilder.h"
#include "cengine.h"
#include "tconvert.h"


void GSchemedSkinBuilder::Begin( int w,int h , bool _keeppack , GSchemedSkin* lskin)
{
	SkinBitmap = new TBitmap(w,h);
	Gfx.Initialize(SkinBitmap);
	Pack = new TPackedRectangle(w,h);
	Skin = lskin;
	if (!Skin)
	{
		Skin = new GSchemedSkin();
	}
	KeepPack = _keeppack;
	Gfx.Clear(0);
}

void GSchemedSkinBuilder::Begin( TBitmap* bmp, TPackedRectangle* pck , GSchemedSkin* lskin)
{
	SkinBitmap = bmp;
	Gfx.Initialize(SkinBitmap);
	Pack = pck;
	Skin = lskin;
	if (!Skin)
	{
		Skin = new GSchemedSkin();
	}
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

int GSchemeClass::GetInt( const String& key, int defaultValue = 0 )
{
	THashKeyValue<String*>* kp = Variables.Get(key);
	if (kp == 0)
	{
		return defaultValue;
	}
	return Convert::ToInt32(*kp->Value);
}

String* GSchemeClass::GetValue(const String& key)
{
	String* rv = Variables.GetValue(key);
	if (rv == NULL)
	{
		return &String::Empty;
	}
	return rv;
}

String* GSchemeClass::GetMustValue( const String& key )
{
	String* rv = Variables.GetValue(key);
	if (rv == NULL)
	{
		throw Exception("Value not found");
	}
	return rv;
}

TColor32 GSchemeClass::GetColor( const String& key )
{
	String* val = GetValueOrNull(key);
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

GSchemeClass* GSchemeFile::GetSchemeClass( const String& className )
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
	GlyphImage = cls->GetValueOrNull("glyphimage");
	GlyphTransMode = cls->GetInt("glyphtransmode");
	FrameCount = cls->GetInt("framecount");
	PaintStyle = UseMargins;
	Tiling = (GSchemeTile)cls->GetInt("Tile");
}

void GSchemeLayer::CopyTo( GScalableQuad* qd ) const
{
	qd->Left = LeftMargin;
	qd->Top = TopMargin;
	qd->Right = RightMargin;
	qd->Bottom = BottomMargin;
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

GSchemeLayer GSchemeFile::GetLayer( const String& className )
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

GSchemeText GSchemeFile::GetTextLayer( const String& className )
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
	Gfx.FillRectangle2(TBrush(TColors::White),*whiteNode);
	VTexturePart* whitePart = new VTexturePart( *SkinBitmap, *whiteNode );
	Skin->WhitePart = *whitePart;


	TRectangleNode* transNode = Pack->Insert(16,16);
	Gfx.FillRectangle2(TBrush(0),*transNode);
	Skin->TransparentPart.Initialize(*SkinBitmap,*transNode);


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
	Skin->WindowQuad[0].CenterColor = Skin->Colors.ButtonFace;
	Skin->WindowQuad[1].Center.Initialize(*whitePart);
	Skin->WindowQuad[1].CenterColor = Skin->Colors.ButtonFace;
	Skin->ButtonFaceWindowBackgroundColor = Skin->Colors.ButtonFace;

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
	//LoadToolStripButton    (Scheme->GetLayer("toolbar.normal"));

	LoadScrollbarButtons   (Scheme->GetLayer("scrollbar"));
	LoadScrollbarBgH       (Scheme->GetLayer("horzscroll"));
	LoadScrollbarBgV       (Scheme->GetLayer("vertscroll"));
	LoadScrollbarDragH     (Scheme->GetLayer("horzscrollthumb"));
	LoadScrollbarDragV     (Scheme->GetLayer("vertscrollthumb"));
	LoadScrollbarDragHSmall(Scheme->GetLayer("smallhscrollthumb"));
	LoadScrollbarDragVSmall(Scheme->GetLayer("smallvscrollthumb"));

	LoadTabBorder          (Scheme->GetLayer("tabs.border"));
	LoadTabBackground      (Scheme->GetLayer("tabs.page"));
	LoadTabPageButton      (Scheme->GetLayer("tabs"));
	LoadTabPageLeftButton  (Scheme->GetLayer("tabs.left"));
	LoadTabPageRightButton (Scheme->GetLayer("tabs.right"));

	LoadToolWindowClose	   (Scheme->GetLayer("toolwindow.closebutton"));
	
	LoadPlusMinus		   (Scheme->GetLayer("treeview"));
	LoadDottedLines		   ();

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

	IRegion tmpRegion;
	tmpRegion.SetRegion( *sub );

	pbarbg.CopyTo(&Skin->ProgressBarBg);
	Skin->ProgressBarBg.Initialize(*SkinBitmap,tmpRegion);

	delete image;

	image = Engine.Textures.LoadToBitmap(*pbarblk.ImagePath);
	sub = InsertImage(image);

	tmpRegion.SetRegion( *sub );

	pbarblk.CopyTo(&Skin->ProgressBarBlock);
	Skin->ProgressBarBlock.Initialize(*SkinBitmap,tmpRegion);

	delete image;
}

void GSchemedSkinBuilder::LoadWindowTop( const GSchemeText& borderData, bool corners )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*borderData.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int halfHeight = sub->Height/2;

	if (corners)
	{
		Skin->WindowQuad[0].TopLeft.InitializeRelative(*SkinBitmap,*sub,0,0,borderData.LeftMargin,halfHeight);
		Skin->WindowQuad[0].Top.InitializeRelative(*SkinBitmap,*sub,borderData.LeftMargin,0,sub->Width-borderData.LeftMargin - borderData.RightMargin,halfHeight);
		Skin->WindowQuad[0].TopRight.InitializeRelative(*SkinBitmap,*sub, sub->Width - borderData.RightMargin,0,borderData.RightMargin,halfHeight);

		Skin->WindowQuad[1].TopLeft.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].TopLeft,0,halfHeight);
		Skin->WindowQuad[1].Top.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Top,0,halfHeight);
		Skin->WindowQuad[1].TopRight.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].TopRight,0,halfHeight);
	}
	else
	{
		Skin->WindowQuad[0].Top.InitializeRelative(*SkinBitmap,*sub,0,0,sub->Width,halfHeight);
		Skin->WindowQuad[1].Top.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Top,0,halfHeight);
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
		Skin->WindowQuad[0].BottomLeft.InitializeRelative(*SkinBitmap,*sub,0,0,borderData.LeftMargin,halfHeight);
		Skin->WindowQuad[0].Bottom.InitializeRelative(*SkinBitmap,*sub,borderData.LeftMargin,0,sub->Width-borderData.LeftMargin - borderData.RightMargin,halfHeight);
		Skin->WindowQuad[0].BottomRight.InitializeRelative(*SkinBitmap,*sub, sub->Width - borderData.RightMargin,0,borderData.RightMargin,halfHeight);

		Skin->WindowQuad[1].BottomLeft.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].BottomLeft,0,halfHeight);
		Skin->WindowQuad[1].Bottom.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Bottom,0,halfHeight);
		Skin->WindowQuad[1].BottomRight.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].BottomRight,0,halfHeight);
	}
	else
	{
		Skin->WindowQuad[0].Bottom.InitializeRelative(*SkinBitmap,*sub,0,0,sub->Width,halfHeight);
		Skin->WindowQuad[1].Bottom.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Bottom,0,halfHeight);
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
		Skin->WindowQuad[0].TopLeft.InitializeRelative(*SkinBitmap,*sub,0,0,halfWidth,borderData.TopMargin);
		Skin->WindowQuad[0].Left.InitializeRelative(*SkinBitmap,*sub,0,borderData.TopMargin,halfWidth,sub->Height-borderData.TopMargin-borderData.BottomMargin);
		Skin->WindowQuad[0].BottomLeft.InitializeRelative(*SkinBitmap,*sub,0,sub->Height-borderData.BottomMargin,halfWidth,borderData.BottomMargin);

		Skin->WindowQuad[1].TopLeft.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].TopLeft,halfWidth,0);
		Skin->WindowQuad[1].Left.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Left,halfWidth,0);
		Skin->WindowQuad[1].BottomLeft.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].BottomLeft,halfWidth,0);
	}
	else
	{
		Skin->WindowQuad[0].Left.InitializeRelative(*SkinBitmap,*sub,0,0,halfWidth,sub->Height);
		Skin->WindowQuad[1].Left.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Left,halfWidth,0);
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
		Skin->WindowQuad[0].TopRight.InitializeRelative(*SkinBitmap,*sub,0,0,halfWidth,borderData.TopMargin);
		Skin->WindowQuad[0].Right.InitializeRelative(*SkinBitmap,*sub,0,borderData.TopMargin,halfWidth,sub->Height-borderData.TopMargin-borderData.BottomMargin);
		Skin->WindowQuad[0].BottomRight.InitializeRelative(*SkinBitmap,*sub,0,sub->Height-borderData.BottomMargin,halfWidth,borderData.BottomMargin);

		Skin->WindowQuad[1].TopRight.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].TopRight,halfWidth,0);
		Skin->WindowQuad[1].Right.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Right,halfWidth,0);
		Skin->WindowQuad[1].BottomRight.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].BottomRight,halfWidth,0);
	}
	else
	{
		Skin->WindowQuad[0].Right.InitializeRelative(*SkinBitmap,*sub,0,0,halfWidth,sub->Height);
		Skin->WindowQuad[1].Right.InitializeOffset(*SkinBitmap,Skin->WindowQuad[0].Right,halfWidth,0);
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
	IRegion tmpRegion;
	tmpRegion.SetRegion( *sub );
	tmpRegion.ChangeWidth(partWidth);

	for (int i=0;i<imagecount;i++)
	{
		output[i].Initialize(*SkinBitmap, tmpRegion);
		tmpRegion.MoveXDiff(partWidth);
	}

	delete image;
	// TODO: should delete sub?
}

void GSchemedSkinBuilder::LoadGeneric( const GSchemeLayer& data, int imageCount, GScalableQuad* output )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(*data.ImagePath);
	VTexturePart* sub = InsertImage(image);

	int partWidth = sub->Width / imageCount; // there is 5 different pictures
	IRegion tmpRegion;
	tmpRegion.SetRegion( *sub );
	tmpRegion.ChangeWidth( partWidth );
	for (int i=0;i<imageCount;i++)
	{
		data.CopyTo(&output[i]);
		output[i].Initialize(*SkinBitmap, tmpRegion);
		tmpRegion.MoveXDiff(partWidth); // increment x of region
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
	GScalableQuad butquads[6];
	LoadGeneric(buttonData,6,butquads);
	ui32 order[] = {GBG_NORMAL,GBG_OVER, GBG_DOWN, GBG_DISABLED,GBG_FOCUSED};
	Skin->ToolButtonGfx.Load(butquads,order,5);

	//LoadGeneric(buttonData,6,Skin->ToolButton);
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

VTexturePart* GSchemedSkinBuilder::LoadInsertImage( const String& path )
{
	TBitmap* image = Engine.Textures.LoadToBitmap(path);
	VTexturePart* tp = InsertImage(image);
	delete image;
	return tp;
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

	Gfx.DrawImage2(*bmp,*(IPosition*)node);
	VTexturePart* tpart = new VTexturePart( *SkinBitmap, *node );
	return tpart;
}

GSchemeFont* GSchemedSkinBuilder::GetNumberedFont( int fontID, FontKinds fontKind )
{
	
	Array<GSchemeFont*>& fontArray = fontKind == FK_CUSTOMFONTS ? NumberedFonts : NumberedSystemFonts;
	for (ui32 i=0;i<fontArray.Count;i++)
	{
		GSchemeFont* curFont = fontArray.Items[i];

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
				LoadColors((GSchemeClass*)curClass,Skin->Colors);
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
	Skin->DefaultFontColor = Skin->Colors.ButtonText;
}

void GSchemedSkinBuilder::LoadScrollbarButtons( const GSchemeLayer& buttonData )
{
	VTexturePart sbut[23];
	LoadGeneric(buttonData,23,sbut);

	if (buttonData.GlyphImage)
	{
		TBitmap* glyphs = Engine.Textures.LoadToBitmap(*buttonData.GlyphImage);
		TBlendMode* curMode = Gfx.GetBlending();
		Gfx.SetBlending(&TBlendModes::Normal); // we are drawing things over other things
		Gfx.DrawImage2(*glyphs,sbut[0].X,sbut[0].Y);
		Gfx.SetBlending(curMode); // copying is enough for building stuff like this
		delete glyphs;
	}

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
	VTexturePart sbg[3];
	LoadGeneric(dragsData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragSmallHGfx.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadScrollbarDragVSmall( const GSchemeLayer& dragsData )
{
	VTexturePart sbg[3];
	LoadGeneric(dragsData,3,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ScrollbarDragSmallVGfx.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadToolWindowClose( const GSchemeLayer& butData )
{
	VTexturePart sbg[6];
	LoadGeneric(butData,6,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_OVER};
	Skin->ToolWindowClose.Load(sbg,order,3);
}

void GSchemedSkinBuilder::LoadColors( GSchemeClass* cls, GSchemeColors& colors )
{
	colors.Scrollbar             = cls->GetColor("scrollbar");
	colors.ActiveTitle           = cls->GetColor("activetitle");
	colors.InactiveTitle         = cls->GetColor("inactivetitle");
	colors.Menu                  = cls->GetColor("menu");
	colors.Window                = cls->GetColor("window");
	colors.MenuText              = cls->GetColor("menutext");
	colors.WindowText            = cls->GetColor("windowtext");
	colors.TitleText             = cls->GetColor("titletext");
	colors.ActiveBorder          = cls->GetColor("activeborder");
	colors.InactiveBorder        = cls->GetColor("inactiveborder");
	colors.AppWorkSpace          = cls->GetColor("appworkspace");
	colors.Hilight               = cls->GetColor("hilight");
	colors.HilightText           = cls->GetColor("hilighttext");
	colors.ButtonFace            = cls->GetColor("buttonface");
	colors.ButtonShadow          = cls->GetColor("buttonshadow");
	colors.GrayText              = cls->GetColor("graytext");
	colors.ButtonText            = cls->GetColor("buttontext");
	colors.InactiveTitleText     = cls->GetColor("inactivetitletext");
	colors.ButtonHilight         = cls->GetColor("buttonhilight");
	colors.ButtonDkShadow        = cls->GetColor("buttondkshadow");
	colors.ButtonLight           = cls->GetColor("buttonlight");
	colors.InfoText              = cls->GetColor("infotext");
	colors.InfoWindow            = cls->GetColor("infowindow");
	colors.ButtonAlternateFace   = cls->GetColor("buttonalternateface");
	colors.HotTrackingColor      = cls->GetColor("hottrackingcolor");
	colors.GradientActiveTitle   = cls->GetColor("gradientactivetitle");
	colors.GradientInactiveTitle = cls->GetColor("gradientinactivetitle");
	colors.MenuHilight           = cls->GetColor("menuhilight");
	colors.MenuBar               = cls->GetColor("menubar");
	colors.Background            = cls->GetColor("background");
	colors.WindowFrame           = cls->GetColor("windowframe");
}

void GSchemedSkinBuilder::LoadTabBorder( const GSchemeLayer& tabData )
{
	LoadGeneric(tabData,&Skin->TabBorder);
}

void GSchemedSkinBuilder::LoadTabBackground( const GSchemeLayer& tabData )
{
	LoadGeneric(tabData,&Skin->TabBackground);
}

void GSchemedSkinBuilder::LoadTabPageButton( const GSchemeLayer& tabData )
{
	GScalableQuad sbg[5];
	LoadGeneric(tabData,5,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
	Skin->TabPage.Load(sbg,order,4);
}

void GSchemedSkinBuilder::LoadTabPageLeftButton( const GSchemeLayer& tabData )
{
	GScalableQuad sbg[5];
	LoadGeneric(tabData,5,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
	Skin->TabPageLeft.Load(sbg,order,4);
}

void GSchemedSkinBuilder::LoadTabPageRightButton( const GSchemeLayer& tabData )
{
	GScalableQuad sbg[5];
	LoadGeneric(tabData,5,sbg);
	ui32 order[] = {GBG_NORMAL,GBG_DOWN,GBG_DISABLED,GBG_OVER};
	Skin->TabPageRight.Load(sbg,order,4);
}

void GSchemedSkinBuilder::LoadDottedLines()
{
	//TColor32 dclr = Skin->Colors.ButtonShadow;

	TColor32 dclr = TColor32::Black;

	TBitmap* dot = new TBitmap(48,20);
	TBitmapGraphics g(dot);
	g.Clear(0);


	IRectangle dr(0,0,16,20); // |
	g.DrawDottedVerticalLine(dclr,dr.CenterX(),0,20);

	dr.TranslateVector(16,0); // |-
	g.DrawDottedVerticalLine(dclr,dr.CenterX(),0,20);
	g.DrawDottedHorizontalLine(dclr,dr.CenterX()+1,dr.CenterY(),(dr.Width/2)-1);

	dr.TranslateVector(16,0); // |_
	g.DrawDottedVerticalLine(dclr,dr.CenterX(),0,dr.Height/2);
	g.DrawDottedHorizontalLine(dclr,dr.CenterX()+1,dr.CenterY(),(dr.Width/2)-1);

	
	VTexturePart* sub = InsertImage(dot);

	int partWidth = sub->Width / 3;
	IRegion tmpRegion;
	tmpRegion.SetRegion( *sub );
	tmpRegion.ChangeWidth(partWidth);

	for (int i=0;i<3;i++)
	{
		Skin->DotLine[i].Initialize(*SkinBitmap, tmpRegion);
		tmpRegion.MoveXDiff(partWidth);
	}

	delete dot;
}

void GSchemedSkinBuilder::LoadPlusMinus( const GSchemeLayer& buttonData )
{
	if (buttonData.FrameCount != 2 && buttonData.FrameCount != 4)
	{
		throw Exception("Treeview frame count should 2 or 4");
	}


	VTexturePart* sbut = new VTexturePart [ buttonData.FrameCount ];
	LoadGeneric(buttonData,buttonData.FrameCount,sbut);
	
	/**
	 * Embed glyph
	 */
	if (buttonData.GlyphImage)
	{
		TBitmap* glyphs = Engine.Textures.LoadToBitmap(*buttonData.GlyphImage);
		TBlendMode* curMode = Gfx.GetBlending();
		Gfx.SetBlending(&TBlendModes::Normal); // we are drawing things over other things
		Gfx.DrawImage2(*glyphs,sbut[0].X,sbut[0].Y);
		Gfx.SetBlending(curMode); // copying is enough for building stuff like this
		delete glyphs;
	}

	if (buttonData.FrameCount == 2)
	{
		Skin->TreeViewPlusMinus[0].Initialize( sbut[0] );
		Skin->TreeViewPlusMinus[1].Initialize( sbut[1] );

		Skin->TreeViewPlusMinus[2].Initialize( sbut[0] );
		Skin->TreeViewPlusMinus[3].Initialize( sbut[1] );
	}
	else
	{
		Skin->TreeViewPlusMinus[0].Initialize( sbut[0] );
		Skin->TreeViewPlusMinus[1].Initialize( sbut[1] );
		Skin->TreeViewPlusMinus[2].Initialize( sbut[2] );
		Skin->TreeViewPlusMinus[3].Initialize( sbut[3] );
	}

	delete [] sbut;
}

