
#include "stdafx.h"
#include "cwindows.h"
#include "gevent.h"
#include "cengine.h"

#ifdef WIN32

bool CWin32RenderWindow::Create( int _width,int _height,ch16* _title,bool fullscr,int bits )
{
	SetViewportSize(_width,_height);

	WNDCLASSEX wc;    // Window class object.
	RECT		WindowRect;			
	WindowRect.left=(long)0;		
	WindowRect.right=(long)_width;	
	WindowRect.top=(long)0;			
	WindowRect.bottom=(long)_height;

	vFullScreen = fullscr;
	vColorDepth = bits;

	// This is the Window class.
	hInstance = GetModuleHandle(NULL);

	wc.cbSize = sizeof(WNDCLASSEX);           // size of the WNDCLASSEX structure.
	wc.style = CS_HREDRAW | CS_VREDRAW;       // style of the window.
	wc.lpfnWndProc = (WNDPROC)CWin32RenderWindow::DummyWndProc;                 // Address to the windows procedure.
	wc.cbClsExtra = 0;                        // Extra class information.
	wc.cbWndExtra = 0;                        // Extra window information.
	wc.hInstance = hInstance;                 // Handle of application Instance.
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);// Window Icon.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load mouse cursor.
	wc.hbrBackground = NULL;                  // Background color.
	wc.lpszMenuName = NULL;                   // Menu.
	wc.lpszClassName = L"Realizer";            // Name of the window class.
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);// Minimized window icon.

	if(!RegisterClassEx(&wc)) return false;

	// Create the window.
	hWnd = CreateWindowEx(NULL,                                       // The extended style.
		L"Realizer",                                 // Window class.
		_title,     // Window name.
		WS_OVERLAPPEDWINDOW | WS_VISIBLE |         // Window style.
		WS_SYSMENU |WS_CLIPCHILDREN |              // Window style.
		WS_CLIPSIBLINGS,                           // Window style.
		0, 0,                                  // X, Y coords.
		WindowRect.right-WindowRect.left, 
		WindowRect.bottom-WindowRect.top,                                  // Window size.
		NULL,                                      // Handle to parent window.
		NULL,                                      // Menu.
		hInstance,                                 // Handle to app instance.
		(LPVOID)this);                                     // Pointer to window.

	if (!hWnd) return false;

	if (!(hDC=GetDC(hWnd)))							
	{
		Destroy();						
		return false;								
	}

	ShowWindow(hWnd, SW_SHOW);    // Show the window.
	UpdateWindow(hWnd);           // Update its display.

	return true;
}

void CWin32RenderWindow::Destroy()
{
	if (hDC && !ReleaseDC(hWnd,hDC))				
	{
		// release dc failed
	}

	if (hWnd && !DestroyWindow(hWnd))				
	{
		//MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;									
	}

	if (!UnregisterClass(L"Realizer",hInstance))		
	{
		//MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							
	}
}

int CWin32RenderWindow::DummyWndProc( HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam )
{
	if (uMsg == WM_CREATE)
	{
		long This = reinterpret_cast<long>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, This);
		return 0;
	}

	CWin32RenderWindow* window = (CWin32RenderWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (window)
	{
		if (window->ProcessEvent(uMsg,wParam,lParam))
			return 0;
	}

	if (uMsg == WM_CLOSE)
		return 0;

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int CWin32RenderWindow::ProcessEvent( UINT uMsg,WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)								
	{
	case WM_ACTIVATE:						
		if (!HIWORD(wParam))				
		{
			vActive=TRUE;				
		}
		else
		{
			vActive=FALSE;					
		}
		break;				

	case WM_SYSCOMMAND:					
		switch (wParam)				
		{
		case SC_SCREENSAVE:				
		case SC_MONITORPOWER:		
			return 1;
		}
		break;

	/*case WM_CLOSE :
		nEvent.Type = EventArgs::Closed;
		SendEvent(&nEvent);
		break;*/


	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		Engine.Inputs.Keyboard.KeyDown(wParam);
		break;						

	case WM_SYSKEYUP:
	case WM_KEYUP:
		Engine.Inputs.Keyboard.KeyUp(wParam);
		break;							

	case WM_CHAR:
		Engine.Inputs.Keyboard.KeyUnicode(wParam);
		break;							

	case WM_LBUTTONDOWN:
		Engine.Inputs.Mouse.MouseDown(LOWORD(lParam),HIWORD(lParam),MouseButtons::Left);
		break;

	case WM_LBUTTONDBLCLK:
		// intresting tool
		break;

	case WM_LBUTTONUP:
		Engine.Inputs.Mouse.MouseUp(LOWORD(lParam),HIWORD(lParam),MouseButtons::Left);
		break;

	case WM_RBUTTONDOWN:
		Engine.Inputs.Mouse.MouseDown(LOWORD(lParam),HIWORD(lParam),MouseButtons::Right);
		break;

	case WM_RBUTTONUP:
		Engine.Inputs.Mouse.MouseUp(LOWORD(lParam),HIWORD(lParam),MouseButtons::Right);
		break;

	case WM_MBUTTONDOWN:
		Engine.Inputs.Mouse.MouseDown(LOWORD(lParam),HIWORD(lParam),MouseButtons::Middle);
		break;

	case WM_MBUTTONUP:
		Engine.Inputs.Mouse.MouseUp(LOWORD(lParam),HIWORD(lParam),MouseButtons::Middle);
		break;

	case WM_XBUTTONDOWN:
		Engine.Inputs.Mouse.MouseDown(LOWORD(lParam),HIWORD(lParam),HIWORD(wParam) == XBUTTON1 ? MouseButtons::XButton1 : MouseButtons::XButton2);
		break;

	case WM_XBUTTONUP:
		Engine.Inputs.Mouse.MouseUp(LOWORD(lParam),HIWORD(lParam),HIWORD(wParam) == XBUTTON1 ? MouseButtons::XButton1 : MouseButtons::XButton2);
		break;

	case WM_MOUSEMOVE:
		Engine.Inputs.Mouse.MouseMove(LOWORD(lParam),HIWORD(lParam));
		break;


	case WM_MOUSEWHEEL:
		Engine.Inputs.Mouse.MouseWheel(LOWORD(lParam),HIWORD(lParam),(int)wParam / 120);
		break;

	case WM_SIZE:							
		//			Engine.ResizeScene(LOWORD(lParam),HIWORD(lParam));  
		break;

	default:
		return 0;
		break;
	}
	return 1;
}

bool CWin32RenderWindow::DoEvents()
{
	MSG msg;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		if(msg.message==WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

#endif
