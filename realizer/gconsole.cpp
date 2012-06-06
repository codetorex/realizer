#include "stdafx.h"
#include "gconsole.h"
#include "gfont.h"
#include "cengine.h"


/*void GConsole::InitializeBuffer( int width,int height, int bufferheight )
{
	//Buffer.InitializeTrueColorBuffer(width,bufferheight,width,height);
	SetHeight( Font->Height * height );
	SetWidth( Font->SpaceWidth * width );
}*/

void GConsole::Render()
{
	Engine.Draw.NoTexture();
	Engine.Draw.FillRectangle(DrawRegion,TColor32(128,128,128,32));
	Engine.Draw.Flush();

	int drawX;
	int drawY = DrawRegion.Bottom();

	if (Focused)
	{
		int cursorX = DrawRegion.Left() + (Buffer.Cursor.X * Font->SpaceWidth);
		int cursorY = DrawRegion.Top() + (Buffer.Cursor.Y * Font->Size);
		DrawBg(cursorX,cursorY,TColors::Green);
	}

	Engine.Draw.SetTexture(Font->FontTexture);

	for (int cy = 0;cy < Buffer.Size.Height;cy++)
	{
		int curLine = Buffer.Head - cy;
		if (curLine < 0)
		{
			curLine = Buffer.BufferHeight + curLine;
		}
		GConsoleCell* currentLine = &Buffer.Cells[ curLine * Buffer.Size.Width ];

		drawX = DrawRegion.X();
		drawY -= Font->Size;

		for( int cx = 0;cx < Buffer.Size.Width; cx++)
		{
			GConsoleCell* currentChar = &currentLine[cx];
			//DrawBg(drawX,drawY, currentChar->BackColor);
			
			if (currentChar->Character == ' ')
			{
				drawX += Font->SpaceWidth;
				continue;
			}

			GCharacter* charData = Font->GetCharacter(currentChar->Character);
			charData->DrawCharacter(drawX,drawY,currentChar->ForeColor);
			drawX += charData->XAdvance;
		}
		Engine.Draw.Flush();
	}
}

void GConsole::DrawBg( float x,float y, const TColor32& color )
{
	Engine.Draw.DrawQuad(x,y,x+Font->SpaceWidth,y+Font->Size,0,0,1,1,color);
}

void GConsole::InitializeConsole( int _width, int _height, int _bufferheight /*= 0*/ )
{
	Buffer.InitializeBuffer(_width,_height,_bufferheight);
	ChangeWidth( _width * Font->SpaceWidth );
	ChangeHeight( _height * Font->Size );
}

void GConsole::OnKeyPress( ui32 keyID )
{
	if (keyID == Keys::Enter)
	{
		Buffer.WriteChar('\n');
		return;
	}

	if (keyID == Keys::BackSpace)
	{

	}

	// ACTUALLY THIS SHOULD BE SENT TO APPLICATION
	Buffer.WriteChar(keyID);
}