#ifndef GTEXTBOXBASE_H
#define GTEXTBOXBASE_H

#include "gobject.h"

class GCharacter;

class GCharacterBox
{
public:
	GCharacterBox()
	{

	};

	GCharacterBox(ch32 chr)
	{
		Character = chr;
	}

	GCharacterBox(ch32 chr, GFont* fnt, dword clr)
	{
		Character = chr;
		SetFont(fnt);
		Color = clr;
	}

	/// Every character can be different font with different size... Maybe even a smiley?
	GCharacter* FontCharacter;
	ch32 Character;

	/// Every character can have different color
	dword Color;

	int X;
	int Y;
	int MidPoint;

	void Render()
	{
		FontCharacter->DrawCharacter(X,Y,Color);
	}

	inline void SetFont( GFont* font )
	{
		FontCharacter = font->GetCharacter(Character);
	}
};

class GEditableLine
{
public:
	TArray< GCharacterBox > Characters;
	int SelectionStart;
	int SelectionEnd;
	int TotalLength;

	GFont* DefaultFont;
	TColor32 DefaultColor;

	void UpdateCharacter(int startPos, int endPos)
	{
		int rx = 0,ry = 0;

		if (startPos > 0)
		{
			rx = Characters.Item[startPos].X;
			ry = Characters.Item[startPos].Y;
		}

		if (endPos > Characters.Count)
		{
			endPos = Characters.Count;
		}

		for (int i=startPos;i<endPos;i++)
		{
			GCharacterBox& curChar = Characters.Item[i];
			curChar.X = rx;
			curChar.Y = ry;
			curChar.MidPoint = curChar.FontCharacter->XAdvance / 2;

			rx += curChar.FontCharacter->XAdvance;
		}

		TotalLength = rx;
	}

	void ChangeFont(GFont* font)
	{
		for (int i=0;i<Characters.Count;i++)
		{
			Characters.Item[i].SetFont(font);
		}
	}
	
	void UpdateCharacter(int startPos)
	{
		UpdateCharacter(startPos,Characters.Count);
	}

	void InsertCharacter(ch32 chr, int pos)
	{
		GCharacterBox cbox(chr,DefaultFont,DefaultColor);
		Characters.Insert(cbox,pos);
		UpdateCharacter(pos);
	}

	void RemoveCharacter(int startPos, int endPos)
	{
		Characters.RemoveBetween(startPos,endPos-startPos);
		UpdateCharacter(startPos);
	}

	inline void SelectAll()
	{
		SelectionStart = 0;
		SelectionEnd = Characters.Count;
	}

	void Render(int x,int y);
};

class GEditableMultiLine
{
public:
	// not implemented yet.
};

class GTextBoxBase: public GObject
{
public:
	bool Selecting;
	int CaretPosition;
	int XOffset;
	int YOffset;

	int TextSimulation(int scanX,int scanY, bool dontrender);

	void RenderText();
	void UpdateText();
	int CollideText(int x,int y);

	

public:
	GTextBoxBase();

	/*void GotFocus();
	void LostFocus();*/

	void MouseDown(int x,int y, int button);
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	void KeyPress(dword keyID);

	int SelectionStart;
	int SelectionLength;
	int SelectionEnd;
	int MaxLength;

	bool AutoSize;

	ContentAlignment TextAlign;

	TString get_Text();

	TString get_SelectedText();

	void Render();
	void Update();
};

#endif