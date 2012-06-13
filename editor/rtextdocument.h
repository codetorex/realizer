#ifndef RTEXTDOCUMENT_H
#define RTEXTDOCUMENT_H

#include "rdocument.h"
#include "tarray.h"

class RTextLine
{
public:
	TString Line;

	operator TString&  ()
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

	TArray<RTextLine*> Lines;

	void LoadDocument();
};


#endif