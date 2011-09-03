
#ifndef CRENDERWINDOW_H
#define CRENDERWINDOW_H

#include <raisetypes.h>
#include <tarray.h>
#include <tstring.h>

#include "gevent.h"
#include "cviewport.h"


class REngine;

class DisplayDevice: public Viewport
{
public:
	bool vFullScreen;

	virtual bool Create(int _width,int _height,const TString& _title,bool fullscr = false,int bits=24) = 0;
	virtual void Destroy() = 0;

	virtual void ChangeTitle(const TString& newtitle) = 0;
	virtual bool DoEvents() = 0;

	virtual void ShowMouseCursor(bool visible) = 0;

	virtual void SetCursorPosition(int left,int top) = 0; ///< Relative to viewport.

	virtual bool InitializeRenderer(int _width,int _height,const TString& _title,bool fullscr = false,int bits=24) = 0;
};


#endif