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
	
	// TODO: cache string pixel width in somewhere
	WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,WindowTitleAlign,WindowTitleColor[windowState] , WindowQuad->TopLeft.Width ,-1 );
	//WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft,WindowTitleColor[windowState] );
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{
	window->TitleBar->SetSize(0,0,window->Width,WindowQuad->Top.Height);
}

void GSchemedSkin::RenderButton( GButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ButtonQuad[button->GraphicState].Render(button);

	button->Font->Render(button->Text,button->ScreenRegion,button->TextAlign,button->ForeColor,0,-1);
}

void GSchemedSkin::RenderLabel( GLabel* label )
{
	Engine.Draw.SetTexture(SkinTexture);
	label->Font->Render(label->Text,label->ScreenRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GSchemedSkin::RenderCheckBox( GCheckBox* checkbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	checkbox->Height = CheckBoxQuad[0].Height; // TODO: fix this and use checkalign
	CheckBoxQuad[checkbox->GraphicState].Draw((float)checkbox->ScreenRegion.X,(float)checkbox->ScreenRegion.Y,0xFFFFFFFF);
	checkbox->Font->Render(checkbox->Text,checkbox->ScreenRegion,checkbox->TextAlign,checkbox->ForeColor,CheckBoxQuad[0].Width+5,-1);
}

void GSchemedSkin::RenderRadioButton( GRadioButton* radiobutton )
{
	Engine.Draw.SetTexture(SkinTexture);
	radiobutton->Height = RadioQuad[0].Height; // TODO: fix this and use checkalign
	RadioQuad[radiobutton->GraphicState].Draw((float)radiobutton->ScreenRegion.X,(float)radiobutton->ScreenRegion.Y,0xFFFFFFFF);
	radiobutton->Font->Render(radiobutton->Text,radiobutton->ScreenRegion,radiobutton->TextAlign,radiobutton->ForeColor,RadioQuad[0].Width+5,-1);
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