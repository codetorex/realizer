#include "stdafx.h"
#include "rtextview.h"
#include "gfont.h"
#include "cengine.h"
#include "rtextdocument.h"

RTextView::RTextView()
{
	Document = 0;
	Name = "Text Editor";

	Style = new RTextViewStyle();
	Style->Font = Engine.GUI.Fonts.GetFont("Dina",11);
	LineNumberWidth = 4;

	ShowLineNumbers = true;

	LineNumberBackColor = TColors::White;
	LineNumberForeColor = TColor32(43,145,175);

	/*LineNumberBackColor = 0xFF343129;
	LineNumberForeColor = 0xFF494E3F;*/
}

void RTextView::Render()
{
	//Font->Render(Name,DrawRegion.X()+50.0f,DrawRegion.Y()+50.f,ForeColor);
	RTextDocument* td = (RTextDocument*)Document;

	IRectangle realContent;
	realContent.SetRectangle(DrawRegion.X()+Content.X,DrawRegion.Y() + Content.Y,Content.Width,Content.Height);

	Engine.Draw.PreTranslate((float)realContent.X,(float)realContent.Y,0.0f);

	if (ShowLineNumbers)
	{
		IRectangle lnRect;
		lnRect.SetVector(0,0);
		lnRect.SetSize(LineNumberPixelWidth,Height);

		Engine.Draw.NoTexture();
		Engine.Draw.FillRectangle(lnRect,LineNumberBackColor);

		IRegion textReg;
		textReg.SetRegion(0,0,LineNumberPixelWidth-1,LineHeight);

		int lineEnd = td->Lines.Count;
		if (lineEnd == 0)
		{
			lineEnd = 40;
		}

		byte tmp[512];
		TStringBuilder sb(tmp,512);

		for (int i=0;i<lineEnd;i++)
		{
			sb.Clear();
			sb.Append(i);
			Style->Font->Render(sb,textReg, CA_MiddleRight,LineNumberForeColor);
			textReg.MoveYDiff(LineHeight);
		}

		sb.UnbindByteArray();
	}

	Engine.Draw.ResetTranslation();
}

void RTextView::Layout()
{
	GObject::Layout();

	LineHeight = Style->Font->Height + 2; // 1 pixel top 1 pixel bottom
	LineCount = Height / LineHeight;
	LineNumberPixelWidth = Style->Font->GetCharacterWidth('0') * LineNumberWidth;
}

void RTextView::KeyDown( ui32 keyID )
{
	Document->Changed = true;
}