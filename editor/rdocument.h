#ifndef RDOCUMENT_H
#define RDOCUMENT_H

#include "tpath.h"

/**
 * Base object that defines a document object that editor currently working on.
 */
class RDocument
{
public:
	TString Name;
	TString FilePath;

	inline virtual void UpdateName()
	{
		Name = TPath::GetFileName(FilePath);
	}


};

#endif