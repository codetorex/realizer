#ifndef RSTARTPAGE_H
#define RSTARTPAGE_H

#include "rdocumentview.h"
#include "rstartpagebutton.h"

/**
 * It just start page looks like in visual studio 2010.
 */
class RStartPage: public RDocumentView
{
public:
	RStartPage();

	RStartPageButton NewProjectButton;
	RStartPageButton OpenProjectButton;

	void NewProjectButton_Click();


	void Render();

	void Layout();
};

#endif