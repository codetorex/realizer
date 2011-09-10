#include "stdafx.h"
#include "gtextboxbase.h"
#include "vgui.h"
#include "cinputenums.h"
#include "gfont.h"
#include "cengine.h"


GTextBoxBase::GTextBoxBase()
{
	ClassID = GTEXTBOXBASE_CLASSID;
	CaretPosition = 0;

	SelectionStart = 0;
	SelectionLength = 0;
	MaxLength = 0;

	AutoSize = true;
	TextAlign = CA_MiddleCenter;

	XOffset = 0;
	YOffset = 0;

	Selecting = false;
}

int GTextBoxBase::TextSimulation( int scanX,int scanY, bool dontrender )
{
	Font->BeginCustomRendering();

	

	int tx = ScreenRegion.X + XOffset;
	int ty = ScreenRegion.Y + YOffset;
	int theight = ty + Font->Height;
	int LastCharMidX = tx; // start x
	int LastCharY = ty; // start y
	int CurCharMidX = LastCharMidX; // current x

	int result = -1;

	bool LongerThanControl = false;
	

	TCharacterEnumerator schars(Text);
	while(schars.MoveNext())
	{
		GCharacter* charData = Font->GetCharacter(schars.Current);
		if ( !dontrender ) charData->DrawCharacter(tx,ty,ForeColor.color);

		
		if (result == -1) 
		{
			CurCharMidX = tx + (charData->XAdvance/2);
			if (scanX > LastCharMidX && scanX < CurCharMidX)
			{
				if (scanY > ty && scanY < theight)
				{
					result = schars.CharIndex;
				}
			}

			LastCharMidX = CurCharMidX;
			tx += charData->XAdvance;

			if (tx > ScreenRegion.Right)
			{
				LongerThanControl = true;
				break;
			}
		}
		
	}
	return result;
}

void GTextBoxBase::RenderText()
{
	Font->BeginCustomRendering();

	TColor32 lolColor(128,128,128,128);

	for (int i=0;i<Characters.Count;i++)
	{
		GCharacterBox& curChar = Characters.Item[i];
		if (i >= SelectionStart && i <= SelectionEnd)
		{
			Engine.Draw.Flush();
			Engine.Draw.SetTexture(0);
			//Engine.Draw.DrawRectangle(,,curChar.FontCharacter->XAdvance,Font->Height,lolColor.color);
			int x1 = curChar.ScreenX;
			int y1 = curChar.ScreenY;
			int x2 = x1 + curChar.FontCharacter->XAdvance;
			int y2 = y1 + Font->Height + 5;

			Engine.Draw.DrawQuad(x1,y1,x2,y2,0,0,1,1,lolColor.color);
			Font->BeginCustomRendering();
		}

		curChar.Render(ForeColor.color);
		if (CaretPosition == i)
		{
			GCharacter* line = Font->GetCharacter('|');
			line->DrawCharacter(curChar.ScreenX + curChar.FontCharacter->XAdvance-1,curChar.ScreenY,ForeColor.color);
		}
	}
}


void GTextBoxBase::UpdateText()
{
	int tx = ScreenRegion.X + XOffset;
	int ty = ScreenRegion.Y + YOffset;

	for (int i=0;i<Characters.Count;i++)
	{
		GCharacterBox& curChar = Characters.Item[i];
		curChar.FontCharacter = Font->GetCharacter(curChar.Character);
		curChar.ScreenY = ty;
		curChar.ScreenX = tx;
		curChar.ScreenMid = curChar.ScreenX + (curChar.FontCharacter->XAdvance / 2); 
		tx += curChar.FontCharacter->XAdvance;
	}
}

int GTextBoxBase::CollideText( int x,int y )
{
	if (Characters.Count == 0) return 0;
	
	for (int i=0;i<Characters.Count-1;i++)
	{
		GCharacterBox& curChar = Characters.Item[i];
		GCharacterBox& nextChar = Characters.Item[i+1];
		
		if ( x > (curChar.ScreenMid) && x < (nextChar.ScreenMid))
		{
			if (y > (curChar.ScreenY) && y < (curChar.ScreenY + Font->Height))
			{
				return i;
			}
		}
	}
	return -1;
}

void GTextBoxBase::Render()
{
	RenderText();
}

void GTextBoxBase::KeyPress( dword keyID )
{
	if (keyID == Keys::BackSpace)
	{
		Characters.RemoveLast();
		UpdateText();
		return;
	}

	GCharacterBox x;
	x.Character = keyID;
	Characters.Add(x);

	UpdateText();
	/*if (keyID == Keys::BackSpace)
	{
		Text.Truncate(Text.Length - 1);
		return;
	}
	Text.AppendUnicode(keyID);*/
}

void GTextBoxBase::MouseDown( int x,int y, int button )
{
	int scrX = x + ScreenRegion.X;
	int scrY = y + ScreenRegion.Y;

	int HitPosition = CollideText(scrX,scrY);
	if (HitPosition != -1)
	{
		SelectionStart = HitPosition;
		SelectionLength = 0;
		CaretPosition = HitPosition;
	}
	Selecting = true;
}

void GTextBoxBase::MouseUp( int x,int y,int button )
{
	MouseMove(x,y);
	Selecting = false;
}

void GTextBoxBase::MouseMove( int x,int y )
{
	if ( !Selecting ) return;

	int scrX = x + ScreenRegion.X;
	int scrY = y + ScreenRegion.Y;

	int HitPosition = CollideText(scrX,scrY);
	if (HitPosition != -1)
	{
		SelectionLength = HitPosition - SelectionStart;
		SelectionEnd = HitPosition;
		CaretPosition = HitPosition;
	}
}


void GTextBoxBase::Update()
{
	this->GObject::Update();
	/*if (AutoSize)
	{
		this->Height = Font->Height;
	}*/
}

