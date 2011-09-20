#ifndef GTEXTBOXBASE_H
#define GTEXTBOXBASE_H

#include "gobject.h"
#include "gfont.h"
#include "gtimer.h"

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

	GCharacterBox(ch32 chr, GFont* fnt, ui32 clr)
	{
		Character = chr;
		SetFont(fnt);
		Color = clr;
	}

	/// Every character can be different font with different size... Maybe even a smiley?
	GCharacter* FontCharacter;
	ch32 Character;

	/// Every character can have different color
	ui32 Color;

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

	inline int CharEndPx()
	{
		return X + FontCharacter->XAdvance;
	}
};

class GEditableLine
{
public:
	GEditableLine()
	{
		SelectionStart = 0;
		SelectionEnd = 0;
		DefaultFont = 0;
		DefaultColor.color = 0xFF;
	}

	TArray< GCharacterBox > Characters;
	int SelectionStart;
	int SelectionEnd;
	int TotalPixelLength;

	GFont* DefaultFont;
	TColor32 DefaultColor;

	void UpdateCharacter(int startPos, int endPos)
	{
		if (Characters.Count == 0) return;

		int rx = 0,ry = 0;
		if (startPos > 0)
		{
			rx = Characters.Item[startPos-1].X + Characters.Item[startPos-1].FontCharacter->XAdvance;
			ry = Characters.Item[startPos-1].Y;
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
			curChar.MidPoint = rx + curChar.FontCharacter->XAdvance / 2;

			rx += curChar.FontCharacter->XAdvance;
		}

		TotalPixelLength = rx;
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
		if (endPos < startPos)
		{
			int tmp = endPos;
			endPos = startPos;
			startPos = tmp;
		}

		Characters.RemoveBetween(startPos,endPos-startPos);
		UpdateCharacter(startPos);
	}

	void RemoveSelection()
	{
		RemoveCharacter(SelectionStart,SelectionEnd);
	}

	void ReplaceSelection(ch32 chr)
	{
		RemoveSelection();
		InsertCharacter(chr,SelectionStart);
	}

	inline void SelectAll()
	{
		SelectionStart = 0;
		SelectionEnd = Characters.Count;
	}

	/**
	 * Collides with characters to detect caret position.
	 * Returns insert position ( character position ) for a caret.
	 */
	int Collide(int x)
	{
		if (Characters.Count == 0)
		{
			return 0;
		}

		if (x < Characters.Item[0].MidPoint)
		{
			return 0;
		}

		for (int i=0;i<Characters.Count;i++)
		{
			GCharacterBox& curChar = Characters.Item[i];

			if ( x < curChar.MidPoint )
			{
				return i;
			}
		}

		return Characters.Count;
	}

	void InternalRender();
	void Render(int x,int y);

	// TODO: caret parameter is lame find a way to connect these classes
	void RenderWithSelection(int x,int y,  int caret);
};

class GEditableMultiLine
{
public:
	// not implemented yet.
};


// TODO: GeditableLine ile bunu birlestir
class GTextBoxBase: public GObject
{
private:
	GEditableLine Line;
	GTimeEffectBool CaretEffect;
	int CaretPosition;
	bool Selecting;

public:
	GTextBoxBase();

	void MouseDown(int x,int y, int button);
	void MouseUp(int x,int y,int button);
	void MouseMove(int x,int y);

	void KeyDown(ui32 keyID);

	void KeyPress(ui32 keyID);

	int SelectionStart;
	int SelectionLength;
	int SelectionEnd;
	int MaxLength;

	bool AutoSize;

	ContentAlignment TextAlign;
	TColor32 SelectionBackGroundColor;

	TString get_Text();

	TString get_SelectedText();

	void Render();
	void Update();
};

#endif