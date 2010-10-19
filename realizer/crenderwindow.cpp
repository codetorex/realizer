
#include "stdafx.h"
#include "crenderwindow.h"
#include "gevent.h"


void RenderWindow::SendEvent( EventArgs* evnt )
{
	// TODO: implement and do with iterator?
	for (int i=0;i<EventListeners.Count;i++)
	{
		EventListeners[i]->OnEvent(evnt);
	}
}

