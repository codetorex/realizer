#ifndef GCONSOLE_H
#define GCONSOLE_H

#include "gobject.h"
#include "tconsoledriver.h"
#include "tlog.h"
#include "tinputenum.h"


class GConsoleState
{
public:
	TColor32 BackColor;
	TColor32 ForeColor;
	ui32 Attributes;
};


class GConsoleCell
{
public:
	TColor32 BackColor;
	TColor32 ForeColor;
	ui32	 Attributes;
	ch32	 Character;

	enum GConsoleAttribute
	{
		CA_BOLD = 1,
		CA_UNDERSCORE = 2,
		CA_BLINK = 4,
	};

	void Set(GConsoleState* currentState, ui32 chr)
	{
		BackColor = currentState->BackColor;
		ForeColor = currentState->ForeColor;
		Attributes = currentState->Attributes;
		Character = chr;
	}
};

class GConsoleBuffer: public TConsoleDriver
{
public:
	ui32 BufferHeight;

	GConsoleCell* Cells;
	ui32 Head; // Head of where the buffer is located
	GConsoleState CurrentState;
	GConsoleState SavedState;

	GConsoleCell* CurrentCell;


	GConsoleBuffer()
	{

	}

	void DumpBuffer()
	{
		TString b( Size.Width *4 );
		for (int y=0;y<BufferHeight;y++)
		{
			GConsoleCell* currentLine = &Cells[ y * Size.Width ];

			b.Clear();

			for (int x=0;x<Size.Width;x++)
			{
				b.AppendUnicodeFast(currentLine[x].Character);
			}

			Log.Output(LG_INF,b);

		}
	}

	void SaveState()
	{
		SavedState = CurrentState;
	}

	void LoadState()
	{
		CurrentState = SavedState;
	}

	void GetPosition()
	{

	}

	void GetSize()
	{
		
	}

	inline void UpdateCurrentCellAddr()
	{
		int bufferY = (Head - (Size.Height-1)) + Cursor.Y;


		int bufferIndex = (Size.Width * bufferY) + Cursor.X;
		CurrentCell = &Cells[ bufferIndex ];
	}

	void SetPosition(TPosition& pos)
	{
		if (pos.X >= Size.Width)
		{
			pos.X = Size.Width - 1;
		}

		if (pos.Y >= Size.Height)
		{
			pos.Y = Size.Height - 1;
		}

		Cursor.X = pos.X;
		Cursor.Y = pos.Y;

		UpdateCurrentCellAddr();
	}

	inline void SetBackColor(byte color )
	{
		CurrentState.BackColor = XTermColors.GetColor(color);
	}

	inline void SetForeColor( byte color )
	{
		CurrentState.ForeColor = XTermColors.GetColor(color);
	}

	void InitializeBuffer( int _width, int _height, int _bufferheight = 0)
	{
		if (_bufferheight < _height)
		{
			_bufferheight = _height;
		}
		Cells = new GConsoleCell[_width * _bufferheight];

		SetColor(CC_BLACK,CC_GRAY);
		CurrentState.Attributes = 0;

		Size.Width = _width;
		Size.Height = _height;
		BufferHeight = _bufferheight;

		Head = Size.Height-1;

		Cursor.SetPosition(0,0);
		CurrentCell = Cells;

		set_NewLine("\n");

		Clear();
	}

	inline void SetBold(bool bold)
	{
		if (bold)
		{
			CurrentState.Attributes |= GConsoleCell::CA_BOLD;
		}
		else
		{
			CurrentState.Attributes &= ~GConsoleCell::CA_BOLD;
		}
	}

	inline void Clear()
	{
		int amount = BufferHeight;
		while(amount--)
		{
			ClearLine(amount);
		}
	}

	inline void ClearLine(ui32 LineIndex)
	{
		int bufferIndex = (Size.Width * LineIndex);
		int amount = Size.Width;
		GConsoleCell* Cell = &Cells[bufferIndex];
		while (amount--)
		{
			Cell->Set(&CurrentState, ' ' );
			Cell++;
		}
	}

	inline void ScrollUp()
	{
		Head++;
		if (Head >= BufferHeight)
		{
			Head = 0;
		}
		ClearLine(Head);
	}

	inline void WrapNewLine()
	{
		Cursor.X = 0;
		if (Cursor.Y == Size.Height-1)
		{
			ScrollUp();
		}
		else
		{
			Cursor.Y++;
		}

		UpdateCurrentCellAddr();
	}

	inline void WriteChar(ch32 chr)
	{
		if (chr == '\n')
		{
			CurrentCell->Set(&CurrentState,' ');
			WrapNewLine();
			return;
		}

		if (chr == '\t')
		{
			RepeatChar(' ', 4);
			return;
		}

		CurrentCell->Set(&CurrentState,chr);
		Cursor.X++;
		if (Cursor.X >= Size.Width)
		{
			WrapNewLine();
		}
		else
		{
			CurrentCell++;
		}
	}


	void BeginGetString(delegate1<void,const TString&>* callback)
	{

	}
};

/**
 * Provides generic input for console.
 */
class TConsoleInput
{
public:
	TConsoleDriver* cinput;

	TArray<ch32> Chars;
	TPosition CursorStart;
	int Cursor;

	void BeginInput()
	{
		Chars.Allocate(128); // todo: make this get from buffer
		cinput->GetPosition(CursorStart);
		Cursor = 0;
		cinput->GetSize();
	}

	void Update()
	{
		/*int ncursor = CursorStart.X + Cursor;
		int newx = ncursor % cinput->Size.Width;
		int yadd = ncursor / cinput->Size.Width;

		TPosition newpos(newx,CursorStart.Y + yadd);*/

		//cinput->SetPosition(CursorStart);

		cinput->Write(Chars);
	}

	void Input(ch32 chr)
	{
		if (chr == Keys::Left)
		{
			if (Cursor > 0)
			{
				Cursor--;
			}
		}

		if (chr == Keys::Right)
		{

		}

		if (chr == Keys::BackSpace)
		{

		}

		if (chr == Keys::Delete)
		{

		}
		

	}


};

class GConsole: public GObject
{
public:

	GConsoleBuffer Buffer;

	void InitializeConsole(int _width, int _height, int _bufferheight = 0);

	void KeyPress(ui32 keyID);

	void DrawBg(float x,float y, ui32 color);

	void Render();
};


#endif