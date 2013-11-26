#ifndef RPROJECTSOURCE_H
#define RPROJECTSOURCE_H

#include "rproject.h"

class RProjectSource;

class RProjectSourceType
{
public:
	/// Holds associated extensions
	Array< String* > Extensions;

	String Name;
	int Index;

	GImage* Image;

	void AddExtension(const String& ext)
	{
		String* nExt = new String(ext);
		Extensions.Add(nExt);
	}

	/**
	 * Opens file in editor with according editor
	 */
	virtual void Open(RProjectSource* src) { };
};

class RProjectSourceTypeImage: public RProjectSourceType
{
public:
	RProjectSourceTypeImage();

	void Open(RProjectSource* src);
};

class RProjectSourceTypeManager
{
public:
	RProjectSourceType Unknown;

	Array< RProjectSourceType* > Types;

	void InitializeDefaultTypes();

	void AddType(RProjectSourceType* typ);

	/**
	 * Retrieves source type from file extension
	 */
	RProjectSourceType* GetType(const String& ext);

	static RProjectSourceTypeManager Instance;
};


class RProjectSource: public RProjectNode
{
public:
	RProjectSource(const String& path);

	String FilePath;
	RProjectSourceType* SourceType;

	/**
	 * Finds type from manager and sets SourceType accordingly
	 */
	void UpdateType();

	void UpdateText();

	void Serialize(TXMLWriter& xw);
};

#endif