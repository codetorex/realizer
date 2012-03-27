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
	//Engine.Draw.DrawRectangle(ScreenRegion.X,ScreenRegion.Y,ScreenRegion.Width,ScreenRegion.Height, );
	Engine.Draw.DrawQuad(ScreenRegion.X,ScreenRegion.Y,ScreenRegion.Right,ScreenRegion.Bottom,0,0,0,0,TColor32(128,128,128,32).color);
	Engine.Draw.Flush();

	int drawX;
	int drawY = ScreenRegion.Bottom;

	if (Focused)
	{
		int cursorX = ScreenRegion.Left + (Buffer.Cursor.X * Font->SpaceWidth);
		int cursorY = ScreenRegion.Top + (Buffer.Cursor.Y * Font->Size);
		DrawBg(cursorX,cursorY,TColors::green);
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

		drawX = ScreenRegion.X;
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

void GConsole::DrawBg( float x,float y, ui32 color )
{
	Engine.Draw.DrawQuad(x,y,x+Font->SpaceWidth,y+Font->Size,0,0,1,1,color);
}

void GConsole::InitializeConsole( int _width, int _height, int _bufferheight /*= 0*/ )
{
	Buffer.InitializeBuffer(_width,_height,_bufferheight);
	SetWidth( _width * Font->SpaceWidth );
	SetHeight( _height * Font->Size );
}

void GConsole::KeyPress( ui32 keyID )
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