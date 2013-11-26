

#ifndef CWIN32RENDERWINDOW_H
#define CWIN32RENDERWINDOW_H
#ifdef WIN32

#include <tstring.h>
#include "cdisplaydevice.h"
#include "tlinkedlist.h"



class RDLL CWin32RenderWindow: public DisplayDevice
{
public:
	HDC			hDC;
	HWND		hWnd;
	HINSTANCE	hInstance;

	bool Create(int _width,int _height,const String& _title,bool fullscr = false,int bits=24);
	void Destroy();

	void ChangeTitle(const String& newtitle) {};
	bool DoEvents();

	void ShowMouseCursor(bool visible){};

	void SetCursorPosition(int left,int top){}; ///< Relative to viewport.


	int ProcessEvent(UINT uMsg,WPARAM wParam, LPARAM lParam);
	static int DummyWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);

	//virtual bool InitializeRenderer(int _width,int _height,ch16* _title,bool fullscr = false,int bits=24) = 0;
};

typedef CWin32RenderWindow CRendererWindow;



#endif
#endif