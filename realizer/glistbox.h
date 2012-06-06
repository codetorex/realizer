#ifndef GLISTBOX_H
#define GLISTBOX_H

#include "gscrollbar.h"

class GListBox: public GObject
{
private:
	GScrollBar VScrollBar; // vertical scrollbar
	int ItemHeight;

public:
	GListBox();

	void OnMouseDown(int x,int y, int button);

	TArray< TString* > Items;

	void AddItem( TString& value );

	void OnMouseWheel(int x,int y, int delta);

	int SelectedIndex;

	// TODO: implement multi select stuff

	void Render();
	void Layout();
};

#endif