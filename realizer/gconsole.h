#ifndef GCONSOLE_H
#define GCONSOLE_H

#include "gobject.h"
#include "tstream.h"
#include "tbitmap.h"


/**
 * Useful for programming terminal emulators.
 * Don't know use a circular buffer or linked list.
 * Memory wise it doesn't matters.
 * Lets make it in circular buffer.
 */
class TConsoleBuffer: private TBitmap
{
public:
	/*T* Buffer;

	ui32 Width;
	ui32 Height;*/

	vec2i Cursor;

	ui32 Head;
	ui32 Tail;

	byte* WritePos;

	void Initialize(ui32 _width,ui32 _height)
	{
		/*Buffer = new T [_width * _height];
		Width = _width;
		Height = _height;
		Cursor.set(0,0);
		WritePos = Cursor;

		Head = 0;
		Tail = 0;*/
	}

	void ScrollBuffer(ui32 linecount)
	{
		/*Head += linecount;
		Tail += linecount;
		if (Head > Height)
		{
			Head = Head % Height;
		}

		if (Tail > Height)
		{
			Tail = Tail % Height;
		}*/

		//WritePos = Tail * Height
	}
};

class GConsoleStream: public TStream
{
public:



};


class GConsole: public GObject
{
public:
	byte	Alpha;

	GConsole();

	void InitializeBuffer(int width,int height);

	void Render();
};


#endif