
#include "stdafx.h"
#include "crenderwindow.h"
#include "gevent.h"


void RenderWindow::SendEvent( EventArgs* evnt )
{
	EventListener* curlistener =  (EventListener*)EventListeners.FirstItem;
	while(curlistener)
	{
		curlistener->OnEvent(evnt);
		curlistener = (EventListener*)curlistener->NextItem;
	}
}

