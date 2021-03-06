#ifndef RDOCUMENT_H
#define RDOCUMENT_H

#include "tpath.h"

enum RDocumentTypes
{
	DT_TEXT,
	DT_XML,
	DT_GUI,
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
		KnownFile = false;
	}

	String Name;
	String FilePath;

	RDocumentTypes DocumentType;

	bool Changed;

	/// If knownFile then we know the filePath, and it should be legit, else we will ask for save folder
	bool KnownFile;

	inline virtual void UpdateName()
	{
		Name = TPath::GetFileName(FilePath);
	}

	/**
	 * Call this after you created an empty document?
	 */
	virtual void InitializeEmptyDocument()
	{

	}

	/**
	 * Loads document to cache.
	 */
	virtual void LoadDocument()
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	virtual void SaveDocument() 
	{ 
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void LoadDocument(const String& path)
	{
		FilePath = path;
		LoadDocument();
		KnownFile = true;
	}
};

#endif