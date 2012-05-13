#include "stdafx.h"
#include "gschemedskin.h"
#include "vtexture.h"
#include "tbitmap.h"
#include "cengine.h"
#include "gfont.h"

#include "gcomponents.h"

#include "tconvert.h"

void GSchemedSkin::RenderWindow( GWindow* window )
{
	int windowState = window->IsActive() ? 0 : 1;

	Engine.Draw.SetTexture(SkinTexture);
	WindowQuad[windowState].Render(window);
	
	TColor32 renColor = window->IsActive() ? Colors.TitleText : Colors.InactiveTitleText;

	// TODO: cache string pixel width in somewhere
	WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,WindowTitleAlign, renColor , WindowQuad->TopLeft.Width ,-1 );
	//WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft,WindowTitleColor[windowState] );
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{
	int windowState = window->IsActive() ? 0 : 1;
	GScalableQuadParted& q = WindowQuad[windowState];

	window->TitleBar->SetSize(0,-q.Top.Height,window->Width,q.Top.Height);

	q.SetObjectRegion(window);
}

void GSchemedSkin::RenderButton( GButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ButtonGfx.Render(button);
	//ButtonQuad[button->ButtonGraphic].Render(button);

	button->Font->Render(button->Text,button->ScreenRegion,button->TextAlign,button->ForeColor,0,-1);
}

void GSchemedSkin::RenderLabel( GLabel* label )
{
	Engine.Draw.SetTexture(SkinTexture); // TODO: wtf is this shit?
	label->Font->Render(label->Text,label->ScreenRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GSchemedSkin::RenderCheckBox( GCheckBox* checkbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	//checkbox->Height = CheckBoxQuad[0].Height; // TODO: fix this and use checkalign
	CheckBoxGfx[checkbox->CheckState].Render(checkbox);
	checkbox->Font->Render(checkbox->Text,checkbox->ScreenRegion,checkbox->TextAlign,checkbox->ForeColor,CheckBoxGfx[0].Width+5,-1);
}

void GSchemedSkin::RenderRadioButton( GRadioButton* radiobutton )
{
	Engine.Draw.SetTexture(SkinTexture);
	//radiobutton->Height = RadioQuad[0].Height; // TODO: fix this and use checkalign
	//RadioQuad[radiobutton->GraphicState].Draw((float)radiobutton->ScreenRegion.X,(float)radiobutton->ScreenRegion.Y,0xFFFFFFFF);
	RadioGfx[radiobutton->Checked ? 1 : 0].Render(radiobutton);
	radiobutton->Font->Render(radiobutton->Text,radiobutton->ScreenRegion,radiobutton->TextAlign,radiobutton->ForeColor,RadioGfx[0].Width+5,-1);
}

void GSchemedSkin::RenderProgressBar( GProgressBar* progressbar )
{
	Engine.Draw.SetTexture(SkinTexture);
	ProgressBarBg.Render(progressbar);

	if (progressbar->Value > progressbar->Minimum)
	{
		if (progressbar->Value != progressbar->Maximum)
		{
			TRegion progressRegion;
			progressRegion.SetFrom(&progressbar->ScreenRegion);
			progressRegion.SetWidth(progressbar->GetPercentWidth());
			ProgressBarBlock.Render(&progressRegion);
		}
		else
		{
			ProgressBarBlock.Render(progressbar);
		}
	}

	//progressbar->Value = (progressbar->Value + 1) % progressbar->Maximum;
	
	if (progressbar->ShowPercent)
	{
		TString* pbstr = &progressbar->Text;
		pbstr->Clear();
		TConvert::AppendToString(progressbar->GetPercentage(),*pbstr);
		pbstr->AppendASCIIFast(' ');
		pbstr->AppendASCIIFast('%');

		progressbar->Font->Render(progressbar->Text,progressbar->ScreenRegion,progressbar->PercentAlign,progressbar->ForeColor,0,-1);
	}
}

void GSchemedSkin::RenderTextBox( GTextBox* textbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	SunkEdge[0].Render(textbox);
	//textbox->Font->Render(textbox->Text,textbox->ScreenRegion,CA_TopLeft,textbox->ForeColor,SunkEdge[0].LeftMargin,-1);
}

void GSchemedSkin::RenderSunkEdge( GObject* object )
{
	Engine.Draw.SetTexture(SkinTexture);
	SunkEdge[0].Render(object);
}


void GSchemedSkin::LayoutSunkEdge( GObject* object )
{
	SunkEdge[0].SetObjectRegion(object);
}

void GSchemedSkin::RenderMenuStrip( GMenuStrip* menustrip )
{
	int mod = 1;
	if (menustrip->Enabled)
		mod = 0;
	
	Engine.Draw.SetTexture(SkinTexture);
	MenuBarBg[mod].Render(menustrip);
}

void GSchemedSkin::RenderDropDown( GDropDown* dropdown )
{
	Engine.Draw.SetTexture(SkinTexture);
	DropDownBg.Render(dropdown);
}

void GSchemedSkin::LayoutDropDown( GDropDown* dropdown )
{
	DropDownBg.SetObjectRegion(dropdown);
}

void GSchemedSkin::RenderMenuItem( GMenuItem* menuItem )
{
	Engine.Draw.SetTexture(SkinTexture);
	MenuItemBg[menuItem->GraphicState].Render(menuItem);
}

void GSchemedSkin::RenderMenuStripItem( GMenuItem* menuItem )
{
	Engine.Draw.SetTexture(SkinTexture);
	MenuBarButton[menuItem->GraphicState].Render(menuItem);
}

void GSchemedSkin::LayoutMenuStripItem( GMenuItem* menuItem )
{
	MenuBarButton[0].SetObjectRegion(menuItem);
}

void GSchemedSkin::RenderToolStrip( GToolStrip* toolbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	ToolBarBg.Render(toolbox);
}

void GSchemedSkin::LayoutToolButton( GToolStripButton* button )
{
	ToolButton[0].SetObjectRegion(button);
}

void GSchemedSkin::RenderToolButton( GToolStripButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ToolButton[button->GraphicState].Render(button);
}

void GSchemedSkin::RenderScrollBarButton( GScrollBarButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ScrollbarButtonGfx[button->Direction].Render(button);
}

void GSchemedSkin::LayoutScrollBarButton( GScrollBarButton* button )
{
	button->SetWidth(ScrollbarButtonGfx[0].Width);
	button->SetHeight(ScrollbarButtonGfx[1].Height);
}

void GSchemedSkin::RenderScrollBar( GScrollBar* scrollbar )
{
	Engine.Draw.SetTexture(SkinTexture);
	if (scrollbar->Orientation == SBO_VERTICAL)
	{
		ScrollbarBgVGfx.Render(scrollbar);
		if (scrollbar->DragBar->Height <= ScrollbarDragSmallVGfx.Height)
		{
			ScrollbarDragSmallVGfx.Render(scrollbar->DragBar);
		}
		else
		{
			ScrollbarDragVGfx.Render(scrollbar->DragBar); // check if we going to render smallest one
		}
	}
	else
	{
		ScrollbarBgHGfx.Render(scrollbar);
		if (scrollbar->DragBar->Width <= ScrollbarDragSmallHGfx.Width)
		{
			ScrollbarDragSmallHGfx.Render(scrollbar->DragBar);
		}
		else
		{
			ScrollbarDragHGfx.Render(scrollbar->DragBar);
		}
	}

	RenderScrollBarButton(scrollbar->UpButton);

	RenderScrollBarButton(scrollbar->DownButton);
}

void GSchemedSkin::LayoutScrollBar( GScrollBar* scrollbar )
{
	LayoutScrollBarButton(scrollbar->UpButton);
	LayoutScrollBarButton(scrollbar->DownButton);

	GScrollbarLayout st(*scrollbar);

	st.Calculate();
	st.SetDragSize();

	if (scrollbar->Orientation == SBO_VERTICAL)
	{
		scrollbar->SetWidth(ScrollbarButtonGfx[0].Width);
		scrollbar->DragBar->SetWidth(scrollbar->Width);	

		scrollbar->UpButton->SetLeftTop(0,0);
		scrollbar->DownButton->SetLeftTop(0,scrollbar->Height- scrollbar->DownButton->Height);
		scrollbar->DragBar->SetLeftTop(0,scrollbar->UpButton->Bottom);

		if (scrollbar->DragBar->Height < ScrollbarDragSmallVGfx.Height)
		{
			scrollbar->DragBar->SetHeight(ScrollbarDragSmallVGfx.Height);
			st.Calculate(false);
		}
	}
	else
	{
		scrollbar->SetHeight(ScrollbarButtonGfx[0].Height);
		scrollbar->DragBar->SetHeight(scrollbar->Height);	

		scrollbar->UpButton->SetLeftTop(0,0);
		scrollbar->DownButton->SetLeftTop(scrollbar->Width- scrollbar->DownButton->Width,0);
		scrollbar->DragBar->SetLeftTop(scrollbar->UpButton->Right,0);

		if (scrollbar->DragBar->Width < ScrollbarDragSmallHGfx.Width)
		{
			scrollbar->DragBar->SetWidth(ScrollbarDragSmallHGfx.Width);
			st.Calculate(false);
		}
	}

	scrollbar->ObjectRegion.SetRectangle(0,0,scrollbar->Width,scrollbar->Height);

	st.CalculateDragPosition();
	st.SetDragPos();
}

void GSchemedSkin::RenderHilight( int x,int y, int w,int h )
{
	Engine.Draw.SetTexture(SkinTexture);
	WhitePart.DrawScaled(x,y,w,h,Colors.Hilight);
}

void GSchemedSkin::RenderTabControl( GTabControl* tabc )
{
	Engine.Draw.SetTexture(SkinTexture);

	// lets render tab pages
	TLinkedListEnumerator<GObject*> btn(&tabc->TabPageButtons);
	while(btn.MoveNext())
	{
		if (btn.Current == btn.mList->FirstItem)
		{
			TabPageLeft.Render((GButtonBase*)btn.Current);
		}
		else if (btn.Current == btn.mList->LastItem)
		{
			TabPageRight.Render((GButtonBase*)btn.Current);
		}
		else
		{
			TabPage.Render((GButtonBase*)btn.Current);
		}

		tabc->Font->Render(btn.Current->Text,btn.Current->ScreenRegion,CA_MiddleCenter,tabc->ForeColor);
		Engine.Draw.SetTexture(SkinTexture);
	}


	TRegion borderRegion;
	borderRegion.SetSize(tabc->ScreenRegion.X+tabc->PageArea.X,tabc->ScreenRegion.Y + tabc->PageArea.Y,tabc->PageArea.Width,tabc->PageArea.Height);
	TabBorder.Render(&borderRegion);
}