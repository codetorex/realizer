

#ifndef CWIN32RENDERWINDOW_H
#define CWIN32RENDERWINDOW_H
#ifdef WIN32

#include "crenderwindow.h"
#include "tlinkedlist.h"


class RDLL CWin32RenderWindow: public RenderWindow
{
public:
	HDC			hDC;
	HWND		hWnd;
	HINSTANCE	hInstance;

	bool CreateRenderWindow(int _width,int _height,char* _title,bool fullscr = false,int bits=24);
	void DestroyRenderWindow();

	void ChangeTitle(char* newtitle) {};
	void DoEvents(){};

	void ShowMouseCursor(bool visible){};

	void SetCursorPosition(int left,int top){}; ///< Relative to viewport.


	int ProcessEvent(UINT uMsg,WPARAM wParam, LPARAM lParam);
	static int DummyWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam);
};





#endif
#endif