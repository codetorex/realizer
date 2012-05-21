#ifndef RSTARTPAGE_H
#define RSTARTPAGE_H

#include "rdocumentview.h"

/**
 * It just start page looks like in visual studio 2010.
 */
class RStartPage: public RDocumentView
{
public:
	RStartPage();

	void Render();
};

#endif