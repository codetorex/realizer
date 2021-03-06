#ifndef GDROPDOWNTEXT_H
#define GDROPDOWNTEXT_H

#include "gdropdown.h"
#include "gtextbox.h"

/**
 * Good for prompting text on the fly
 */
class GDropDownText: public GDropDown
{
public:
	GDropDownText();

	GTextBox EditBox;

	void Layout();

	void Show(int x, int y, int width);

	event<NoArgEvent> Edited;


	void SelectAll();

	void SetText(const String& value);

	/**
	 * Clears edit box
	 */
	void Clear();

	void EditBox_KeyPress(void* sender, KeyEventArgs& e);
};


#endif