#ifndef RDOCUMENT_H
#define RDOCUMENT_H

#include "tpath.h"

enum RDocumentTypes
{
	DT_TEXT,
};

/**
 * Base object that defines a document object that editor currently working on.
 */
class RDocument
{
public:
	RDocument()
	{
		Changed = false;
	}

	RDocument(const TString& filepath_)
	{
		FilePath = filepath_;
		LoadDocument();
	}

	TString Name;
	TString FilePath;

	RDocumentTypes DocumentType;

	bool Changed;

	inline virtual void UpdateName()
	{
		Name = TPath::GetFileName(FilePath);
	}

	/**
	 * Loads document to cache.
	 */
	virtual void LoadDocument() = 0;
};

#endif