#include "stdafx.h"
#include "gtextboxbase.h"
#include "vgui.h"
#include "tinputenum.h"
#include "gfont.h"
#include "cengine.h"


/**
 * Lame function to test out stuff.
 * TODO: integrate it to GFONT with attaching 4 white pixel to it.
 */
void DrawRectangle(int x1,int y1,int x2,int y2, ui32 color)
{
	VTexture* curTexture = Engine.Draw.CurrentTexture;

	Engine.Draw.Flush();
	Engine.Draw.SetTexture(0);
	//Engine.Draw.DrawRectangle(,,curChar.FontCharacter->XAdvance,Font->Height,lolColor.color);

	Engine.Draw.DrawQuad((float)x1,(float)y1,(float)x2,(float)y2,0,0,1,1,color);
	Engine.Draw.Flush();
	Engine.Draw.SetTexture(curTexture);
}

void DrawRectangleSize(int x,int y,int w,int h, ui32 color)
{
	DrawRectangle(x,y,x+w,y+h,color);
}

void GEditableLine::Render( int x,int y )
{
	Engine.Draw.Translate((float)x,(float)y);
	InternalRender();
	Engine.Draw.ResetTransform();
}

void GEditableLine::RenderWithSelection( int x,int y, int caret )
{
	Engine.Draw.Translate((float)x,(float)y);

	if (Characters.Count > 0)
	{
		if ( SelectionStart != SelectionEnd )
		{			
			if (SelectionEnd < SelectionStart) // exchange if broken
			{
				int tmp = SelectionEnd;
				SelectionEnd = SelectionStart;
				SelectionStart = tmp;
			}

			GCharacterBox& selStart = Characters.Item[SelectionStart];
			int startX = selStart.X;
			int endX;


			if (SelectionEnd >= Characters.Count)
			{
				GCharacterBox& selEnd = Characters.Item[Characters.Count-1];
				endX = selEnd.CharEndPx();
			}
			else
			{
				GCharacterBox& selEnd = Characters.Item[SelectionEnd];
				endX = selEnd.X;
			}

			// TODO: improve this shit
			//DrawRectangle(startX,0,endX,15, TColors::Grey);
			Skin->RenderHilight(startX,0,endX,15);
		}

		InternalRender();
	}

	if (caret != -1)
	{
		int drawPos = 1;
		if (caret == Characters.Count)
		{
			if (Characters.Count != 0)
			{
				GCharacterBox& lastChr = Characters.Item[Characters.Count-1];
				drawPos = lastChr.X + lastChr.FontCharacter->XAdvance;
			}
		}
		else if (caret != 0)
		{
			GCharacterBox& caretChr = Characters.Item[caret];
			drawPos = caretChr.X;
		}
		

		DrawRectangleSize(drawPos,0,1,15,DefaultColor.color);
	}

	Engine.Draw.ResetTransform();
}

void GEditableLine::InternalRender()
{
	DefaultFont->BeginCustomRendering();
	for (int i=0;i<Characters.Count;i++)
	{
		Characters.Item[i].Render();
	}
}

void GEditableLine::SetText( const TString& value )
{
	Clear();

	int i = 0;
	TCharacterEnumerator ce(value);
	while(ce.MoveNext())
	{
		InsertCharacter(ce.Current,i++);
	}
}

GTextBoxBase::GTextBoxBase()
{
	ClassID = GTEXTBOXBASE_CLASSID;
	CaretPosition = 0;

	SelectionStart = 0;
	SelectionLength = 0;
	MaxLength = 0;

	AutoSize = true;
	TextAlign = CA_MiddleCenter;

	Selecting = false;
	CaretPosition = 0;

	CaretEffect.set_RealTime(true);
	CaretEffect.Delay = 400;
}

void GTextBoxBase::Render()
{
	// TODO: make caret parameter bound to a GTimer like but simpler object
	int ShowCaret = CaretPosition;
	if (!CaretEffect.Value || !Focused)
	{
		ShowCaret = -1;
	}

	const IRectangle& dRect = DrawRegion;
	Line.Skin = Skin;
	Line.RenderWithSelection(dRect.X,dRect.Y, ShowCaret);
}

void GTextBoxBase::OnKeyDown( ui32 keyID )
{
	if (keyID == Keys::Left)
	{
		CaretPosition--;
		if (CaretPosition < 0)
			CaretPosition = 0;
		SetSelection(CaretPosition,0); // reset selection
	}
	else if (keyID == Keys::Right)
	{
		CaretPosition++;
		if (CaretPosition > Line.Characters.Count)
			CaretPosition = Line.Characters.Count;
		SetSelection(CaretPosition,0); // reset selection
	}
	else if (keyID == Keys::Delete)
	{
		if (CaretPosition <= 0)
		{
			CaretPosition = 0;
			return;
		}

		if (CaretPosition >= Line.Characters.Count)
			return;
		
		Line.RemoveCharacter(CaretPosition,CaretPosition+1);
		if (CaretPosition > Line.Characters.Count)
			CaretPosition = Line.Characters.Count;
	}

	CaretEffect.ResetValue(true);
}

void GTextBoxBase::OnKeyPress( ui32 keyID )
{
	KeyEventArgs args;
	args.KeyChar = keyID;
	KeyPress.call(this, args);
	if (args.Handled)
	{
		return;
	}

	if (keyID == Keys::BackSpace)
	{
		if (SelectionLength != 0)
		{
			Line.RemoveSelection();
			CaretPosition = Line.SelectionStart;
			Line.SelectionEnd = Line.SelectionStart;
			SelectionLength = 0;
			return;
		}

		if (CaretPosition <= 0)
		{
			CaretPosition = 0;
			return;
		}
		Line.RemoveCharacter(CaretPosition-1,CaretPosition);
		CaretPosition--;
		return;
	}
	if (keyID < ' ')
	{
		return;
	}

	if (SelectionLength != 0)
	{
		Line.ReplaceSelection(keyID);
		Line.SelectionStart++;
		CaretPosition = Line.SelectionStart;
		Line.SelectionEnd = Line.SelectionStart;
		SelectionLength = 0;
	}
	else
	{
		Line.InsertCharacter(keyID,CaretPosition);
		CaretPosition++;
	}
	
	CaretEffect.ResetValue(true);
}

void GTextBoxBase::OnMouseDown( int x,int y, int button )
{
	int hitpos = Line.Collide(x);
	SelectionStart = hitpos;
	SelectionLength = 0;
	SelectionEnd = SelectionStart;
	Selecting = true;

	// TODO: find a way to connect these members
	Line.SelectionStart = SelectionStart;
	Line.SelectionEnd = SelectionEnd;
}

void GTextBoxBase::OnMouseUp( int x,int y,int button )
{
	OnMouseMove(x,y);
	Selecting = false;
}

void GTextBoxBase::OnMouseMove( int x,int y )
{
	if ( !Selecting ) return;
	int hitpos = Line.Collide(x);

	SelectionEnd = hitpos;
	SelectionLength = SelectionEnd - SelectionStart;
	CaretPosition = SelectionEnd;
	
	Line.SelectionStart = SelectionStart;
	Line.SelectionEnd = SelectionEnd;
}


void GTextBoxBase::Update()
{
	this->GObject::Update();
	Line.DefaultFont = Font;
	Line.DefaultColor.color = ForeColor.color;

	CaretEffect.UpdateTimeEffect();
	/*if (AutoSize)
	{
		this->Height = Font->Height;
	}*/
}

TString GTextBoxBase::get_Text()
{
	TStringBuilderStack<1024> sb;
	for (ui32 i=0;i<Line.Characters.Count;i++)
	{
		GCharacterBox& cb = Line.Characters.Item[i];
		sb.AppendUnicode(cb.Character);
	}
	return sb.ToString();
}

void GTextBoxBase::Clear()
{
	Line.Clear();
	CaretPosition = 0;
}

void GTextBoxBase::set_Text( const TString& value )
{
	Line.SetText(value);
	CaretPosition = value.Length;
}

void GTextBoxBase::SelectAll()
{
	Line.SelectAll();
	SelectionStart = Line.SelectionStart;
	SelectionEnd = Line.SelectionEnd;
	SelectionLength = SelectionEnd - SelectionStart;
}

void GTextBoxBase::SetSelection( int start, int length )
{
	SelectionStart = start;
	SelectionEnd = start + length;
	if (SelectionEnd > Line.Characters.Count)
	{
		SelectionEnd = Line.Characters.Count;
	}

	Line.SelectionStart = SelectionStart;
	Line.SelectionEnd = SelectionEnd;
	SelectionLength = SelectionEnd - SelectionStart;
}