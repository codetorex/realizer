#ifndef RDOCUMENTVIEW_H
#define RDOCUMENTVIEW_H

#include "gobject.h"

class RDocument;

/**
 * Document viewing master control.
 */
class RDocumentView: public GObject
{
protected:
	RDocument* Document;

public:
	RDocumentView();

	TString Name;

	virtual void DocumentChanged() { };

	void SetDocument( RDocument* doc )
	{
		if (doc != Document)
		{
			Document = doc;
			DocumentChanged();
		}
	}

};

#endif