#ifndef RTEXTVIEW_H
#define RTEXTVIEW_H

#include "rdocumentview.h"

/**
 * Main text editor/viewer implementation.
 */
class RTextView: public RDocumentView
{
public:
	RTextView();

	void Render();

};


#endif