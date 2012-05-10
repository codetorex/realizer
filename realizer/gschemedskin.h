#ifndef GSCHEMEDSKIN_H
#define GSCHEMEDSKIN_H

#include <tcolor.h>
#include <tstring.h>

#include "gskin.h"
#include "gquad.h"
#include "tpackedrectangle.h"
#include "genums.h"
#include "gbuttonbase.h"

class VTexture;
class GFont;
class TBitmap;


// Skin class that can use window blinds skins.

class GScemedSkinButtonPart: public TRange
{
public:
	union
	{
		struct  
		{
			VTexturePart Normal;
			VTexturePart Over;
			VTexturePart Down;
			VTexturePart Disabled;
			VTexturePart Focused;
		};

		struct
		{
			VTexturePart Parts[5];
		};
	};

	void Load(VTexturePart* inputParts, ui32* inputOrder, int count)
	{
		int loaded[] = {0,0,0,0,0};

		for (int i=0;i<count;i++)
		{
			for (int j=0;j<count;j++)
			{
				if (inputOrder[j] == i)
				{
					Parts[i].Initialize(inputParts[j]);
					loaded[i] = 1;
					break;
				}
			}
		}

		for (int i=0;i<5;i++)
		{
			if (loaded[i] != 1)
			{
				Parts[i].Initialize(inputParts[0]);
			}
		}

		Width = Normal.Width;
		Height = Normal.Height;
	}

	inline void Render(GButtonBase* button)
	{
		Parts[button->ButtonGraphic].Draw(button->ScreenRegion.X,button->ScreenRegion.Y,TColors::White);
	}
};

class GSchemedSkinButtonQuad
{
public:

	union
	{
		struct
		{
			GScalableQuad Normal;
			GScalableQuad Over;
			GScalableQuad Down;
			GScalableQuad Disabled;
			GScalableQuad Focused;
		};

		struct
		{
			GScalableQuad Quads[5];
		};
	};

	void Load(GScalableQuad* inputQuads, ui32* inputOrder, int count)
	{
		int loaded[] = {0,0,0,0,0};

		for (int i=0;i<count;i++)
		{
			for (int j=0;j<count;j++)
			{
				if (inputOrder[j] == i)
				{
					Quads[i].Initialize(inputQuads[j]);
					loaded[i] = 1;
					break;
				}
			}
		}

		for (int i=0;i<5;i++)
		{
			if (loaded[i] != 1)
			{
				Quads[i].Initialize(Quads[0]);
			}
		}
	}

	inline void Render(GButtonBase* button)
	{
		Quads[button->ButtonGraphic].Render(button);
	}
};

class GSchemedSkin: public GSkin
{
public:
	VTexture* SkinTexture;
	TPackedRectangle* Pack;

	VTexturePart WhitePart;

	GScalableQuadParted WindowQuad[2]; // 0= Active, 1= Inactive
	TColor32 ButtonFaceWindowBackgroundColor;
	GFont* WindowTitleFont; // normal font, pressed font
	ContentAlignment WindowTitleAlign;
	TColor32 WindowTitleColor[2];

	GSchemedSkinButtonQuad ButtonGfx;

	GScalableQuad ButtonQuad[5]; // 0 = Normal, 1 = Pressed, 2 = Disabled, 3 = Mouse Over, 4 = Focus & Default
	GFont* ButtonFont;

	GScemedSkinButtonPart CheckBoxGfx[3];

	//VTexturePart CheckBoxQuad[12];
	GScemedSkinButtonPart RadioGfx[2];
	//VTexturePart RadioQuad[8];

	GScalableQuad ProgressBarBg;
	GScalableQuad ProgressBarBlock;

	GScalableQuad SunkEdge[4];

	GScalableQuad MenuBarBg[2];
	
	GScalableQuad ToolBarBg;

	GScalableQuad DropDownBg;

	GScalableQuad MenuItemBg[5];

	GScalableQuad MenuBarButton[5];

	GScalableQuad ToolButton[6];


	GScemedSkinButtonPart ScrollbarButtonGfx[6];
	VTexturePart SizingCorner;

	GSchemedSkinButtonQuad ScrollbarBgHGfx;
	GSchemedSkinButtonQuad ScrollbarBgVGfx;

	GSchemedSkinButtonQuad ScrollbarDragHGfx;
	GSchemedSkinButtonQuad ScrollbarDragVGfx;

	GSchemedSkinButtonQuad ScrollbarDragSmallHGfx;
	GSchemedSkinButtonQuad ScrollbarDragSmallVGfx;

	void SaveSkin(const TString& path);
	void LoadSkin(const TString& path);

public:

	// Implementation
	void RenderWindow(GWindow* window);
	void LayoutWindow(GWindow* window);

	void RenderButton(GButton* button);

	void RenderLabel(GLabel* label);

	void RenderCheckBox(GCheckBox* checkbox);

	void RenderRadioButton(GRadioButton* radiobutton);

	void RenderProgressBar(GProgressBar* progressbar);

	void RenderTextBox(GTextBox* textbox);

	void RenderSunkEdge(GObject* object);
	
	void LayoutSunkEdge(GObject* object);

	void RenderMenuStrip(GMenuStrip* menustrip);

	void RenderDropDown(GDropDown* dropdown);

	void LayoutDropDown(GDropDown* dropdown);

	void RenderMenuItem(GMenuItem* menuItem);

	void RenderMenuStripItem(GMenuItem* menuItem);

	void LayoutMenuStripItem(GMenuItem* menuItem);

	void RenderToolStrip(GToolStrip* toolbox);

	void RenderToolButton(GToolStripButton* button);

	void LayoutToolButton(GToolStripButton* button);

	void RenderScrollBar(GScrollBar* scrollbar);

	void LayoutScrollBar(GScrollBar* scrollbar);

private:
	void RenderScrollBarButton( GScrollBarButton* button );
	void LayoutScrollBarButton( GScrollBarButton* button );

};





#endif