#ifndef RTEXTDOCUMENT_H
#define RTEXTDOCUMENT_H

#include "rdocument.h"
#include "tarray.h"

class RTextLine
{
public:
	String Line;

	operator String&  ()
	{
		return Line;
	}
};

class RTextDocument: public RDocument
{
public:
	RTextDocument()
	{
		DocumentType = DT_TEXT;
		Name = "Untitled";
		KnownFile = false;
	}

	Array<RTextLine*> Lines;

	void LoadDocument();
};


#endif