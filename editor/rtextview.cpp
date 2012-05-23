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
	Style->Font = Engine.GUI.Fonts.GetFont("Dina",12);
	LineNumberWidth = 4;

	ShowLineNumbers = true;

	LineNumberBackColor = TColors::White;
	LineNumberForeColor = TColor32(43,145,175);

	VertBar.Dock = DCK_RIGHT;

	/*LineNumberBackColor = 0xFF343129;
	LineNumberForeColor = 0xFF494E3F;*/
}

void RTextView::Render()
{
	//Font->Render(Name,DrawRegion.X()+50.0f,DrawRegion.Y()+50.f,ForeColor);
	RTextDocument* td = GetTextDocument();

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

		/*IRegion numReg;
		numReg.SetRegion(0,0,LineNumberPixelWidth-1,LineHeight);


		TStringBuilderStack<512> sb;
		for (int i=0;i<LineCount;i++)
		{
			sb.Clear();
			sb.Append(i);
			Style->Font->Render(sb,numReg, CA_MiddleRight,LineNumberForeColor);
			numReg.MoveYDiff(LineHeight);
		}*/
	}

	IRegion textReg;
	textReg.SetRegion(0,0,Width-LineNumberPixelWidth,LineHeight);
	
	TStringBuilderStack<32> sb;

	int startLine = VertBar.Value;
	int endLine = VertBar.Value + LineCount;

	if (endLine > td->Lines.Count)
	{
		endLine = td->Lines.Count;
	}

	
	GFont* cf = Style->Font;

	int zeroWidth = cf->GetCharacterWidth('0');

	for (int i=startLine;i<endLine;i++)
	{
		int lineX = textReg.X();
		if (ShowLineNumbers)
		{
			sb.Clear();
			sb.Append(i+1);

			TCharacterEnumerator ce(sb);
			int slen = cf->GetStringWidth(ce);

			int lineNumberX = (LineNumberPixelWidth - slen) -zeroWidth;

			cf->Render(sb,lineNumberX,textReg.Y(), LineNumberForeColor);
			lineX += LineNumberPixelWidth;
		}

		TCharacterEnumerator ce(td->Lines.Item[i]->Line);
		
		while(ce.MoveNext())
		{
			if (ce.Current == '\t')
			{
				lineX += 4 * cf->SpaceWidth;
				continue;
			}
			
			lineX += cf->RenderChar(ce.Current,lineX,textReg.Y(),TColors::Black);
		}

		//Style->Font->Render(,textReg, CA_MiddleLeft, TColors::Black);
		textReg.MoveYDiff(LineHeight);
		Engine.Draw.Flush();
	}

	

	Engine.Draw.ResetTranslation();

	GObject::Render();
}

void RTextView::Layout()
{
	if (ItemCount < 1)
	{
		VertBar.SetRectangle(0,0,100,100);
		VertBar.Dock = DCK_RIGHT;
		AddChild(&VertBar);
		VertBar.SmallChange = 1;
	}
	OwnObject(&VertBar);

	RTextDocument* td = GetTextDocument();

	if (td)
	{
		VertBar.MaxValue = (td->Lines.Count + LineCount) - 1;
		
		TStringBuilderStack<32> sb;
		sb.Append(td->Lines.Count);
		LineNumberWidth = sb.Length+3;
	}
	else
	{
		VertBar.MaxValue = LineCount*2 +1;
		LineNumberWidth = 4;
	}

	VertBar.LargeChange = LineCount;
	VertBar.Value = 0;

	GObject::Layout();
	VertBar.Layout();

	LineHeight = Style->Font->Height + 2; // 1 pixel top 1 pixel bottom
	LineCount = Height / LineHeight;
	LineNumberPixelWidth = Style->Font->GetCharacterWidth('0') * LineNumberWidth;
}

void RTextView::KeyDown( ui32 keyID )
{
	Document->Changed = true;
}

void RTextView::DocumentChanged()
{
	Layout();
}

void RTextView::MouseWheel( int x,int y, int delta )
{
	VertBar.MouseWheel(10,y,delta * 3);
}