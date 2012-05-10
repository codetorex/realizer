#ifndef GLISTBOX_H
#define GLISTBOX_H

#include "gobject.h"
#include "gscrollbar.h"

class GListBox: public GObject
{
public:
	GListBox();

	void MouseDown(int x,int y, int button);
	void MouseUp(int x,int y,int button);

	GScrollBar VScrollBar; // vertical scrollbar

	TArray< TString* > Items;

	void AddItem( const TString& value );

	void Render();
	void Layout();
};

#endif