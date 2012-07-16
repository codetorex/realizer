#include "stdafx.h"
#include "rtextview.h"
#include "gfont.h"
#include "cengine.h"
#include "rtextdocument.h"

RTextViewStyle* RTextView::DefaultStyle = 0;

RTextView::RTextView()
{
	Document = 0;
	Name = "Text Editor";

	Style = DefaultStyle;
	LineNumberWidth = 4;

	ShowLineNumbers = true;

	VertBar.Dock = DCK_RIGHT;
}

void RTextView::Render()
{
	//Font->Render(Name,DrawRegion.X()+50.0f,DrawRegion.Y()+50.f,ForeColor);
	RTextDocument* td = GetTextDocument();

	IRectangle realContent;
	realContent.SetRectangle(DrawRegion.X()+Content.X,DrawRegion.Y() + Content.Y,Content.Width,Content.Height);

	Engine.Draw.Translate(realContent.X,realContent.Y);

	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(0,0,(float)Content.Width,(float)Content.Height, Style->Text.BackColor);

	if (ShowLineNumbers)
	{
		IRectangle lnRect;
		lnRect.SetVector(0,0);
		lnRect.SetSize(LineNumberPixelWidth,Height);

		Engine.Draw.NoTexture();
		Engine.Draw.FillRectangle(lnRect,Style->LineNumber.BackColor);

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

			cf->Render(sb,lineNumberX,textReg.Y(), Style->LineNumber.ForeColor);
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
			
			lineX += cf->RenderChar(ce.Current,lineX,textReg.Y(), Style->Text.ForeColor);
		}

		//Style->Font->Render(,textReg, CA_MiddleLeft, TColors::Black);
		textReg.MoveYDiff(LineHeight);
		Engine.Draw.Flush();
	}

	

	Engine.Draw.ResetTransform();

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

	if (!Style)
	{
		Style = DefaultStyle;
	}

	if (Style)
	{
		LineHeight = Style->Font->Height + 2; // 1 pixel top 1 pixel bottom
		LineCount = Height / LineHeight;
		LineNumberPixelWidth = Style->Font->GetCharacterWidth('0') * LineNumberWidth;
	}
}

void RTextView::OnKeyDown( ui32 keyID )
{
	Document->Changed = true;
}

void RTextView::DocumentChanged()
{
	Layout();
}

void RTextView::OnMouseWheel( int x,int y, int delta )
{
	VertBar.OnMouseWheel(10,y,delta * 3);
}

#include "txmlreader.h"

RTextViewStyleVS2010Loader RTextViewStyleVS2010Loader::Instance;

void RTextViewStyleVS2010Loader::GetColor( const TString& item, TColor32& fore, TColor32& back )
{
	TArrayEnumerator< TXMLNode* > ae(colorsNode->Nodes);
	while(ae.MoveNext())
	{
		TString* attrib = ae.Current->Attributes["Name"];

		if (attrib == 0)
		{
			continue;
		}

		if (*attrib == item)
		{
			TString* sfore = ae.Current->Attributes["Foreground"];
			TString* sback = ae.Current->Attributes["Background"];

			fore = TConvert::ToUInt32Hex(*sfore);
			back = TConvert::ToUInt32Hex(*sback);

			if (back.color == 0x02000000)
			{
				back.color = 0;
			}
			else
			{
				back.a = 255;
			}

			fore.a = 255;			
			return;
		}
	}
}

void RTextViewStyleVS2010Loader::LoadStyle( RTextViewStyle* style,TStream* srcStream )
{
	TXMLReader xr(srcStream);
	xr.Parse();

	TXMLNode* cnode = xr.RootNode.SelectSingleNode("Category");

	if (cnode == 0)
	{
		throw Exception("something went wrong");
	}

	TString* aname = cnode->Attributes["name"];

	if (*aname != "Environment_Group")
	{
		throw Exception("Unkown (yet) format");
	}

	cnode = cnode->SelectSingleNode("Category");
	if (cnode == 0)
	{
		throw Exception("something went wrong");
	}

	aname = cnode->Attributes["name"];
	if (*aname != "Environment_FontsAndColors")
	{
		throw Exception("Unkown (yet) format");
	}

	cnode = cnode->SelectSingleNode("FontsAndColors/Categories/Category/Items");

	if (cnode == 0)
	{
		throw Exception("Unkown (yet) format");
	}

	colorsNode = cnode;

	// load colors from here
	GetColor("Line Numbers",style->LineNumber);
	GetColor("Plain Text",style->Text);
	GetColor("Selected Text",style->SelectedText);
}

