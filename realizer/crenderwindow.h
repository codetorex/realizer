
#ifndef CRENDERWINDOW_H
#define CRENDERWINDOW_H

//class Event;
class RDLL RenderWindow
{
public:
	bool vFullScreen;

	//TList3 EventListeners;

	virtual bool CreateRenderWindow(int _width,int _height,char* _title,bool fullscr = false,int bits=24) = 0;
	virtual void DestroyRenderWindow() = 0;

	virtual void ChangeTitle(char* newtitle) = 0;
	virtual void DoEvents() = 0;

	virtual void ShowMouseCursor(bool visible) = 0;

	virtual void SetCursorPosition(int left,int top) = 0; ///< Relative to viewport.

	//virtual void SendEvent(Event* evnt);
};


#endif