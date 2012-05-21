#ifndef RDOCUMENTVIEW_H
#define RDOCUMENTVIEW_H

#include "gobject.h"

class RDocument;

/**
 * Document viewing master control.
 */
class RDocumentView: public GObject
{
public:
	RDocumentView();

	TString Name;

	RDocument* Document;

};

#endif