#ifndef GOBJECTMANAGER_H
#define GOBJECTMANAGER_H

#include "tstring.h"
#include "genums.h"
#include "tarray.h"


class GObject;
class TType;

class GTypeExtension
{
public:
	String Name;
	void* ExtensionPtr;
};

/**
 * GUI Object Type Definition and Factory
 * TODO: we can create a base TType class and derive these stuff from it.
 * TODO: define constructor stuff in TType like in .net reflection and derive this object from it 
 */
class GObjectType
{
public:
	/// This factory creates objects having this class ID
	GUIClassID ClassID;

	/// This factory creates objects named as this
	String ObjectName;

	/// Small description about
	String Description;

	Array< GTypeExtension* > Extensions;

	inline void* GetExtensionPtr(const String& name)
	{
		GTypeExtension* ext = GetExtension(name);
		if (ext)
		{
			return ext->ExtensionPtr;
		}
		return 0;
	}

	inline GTypeExtension* GetExtension(const String& name)
	{
		TArrayEnumerator< GTypeExtension* > ae(Extensions);
		while(ae.MoveNext())
		{
			if (ae.Current->Name == name)
			{
				return ae.Current;
			}
		}

		return 0;
	}

	inline GTypeExtension* RegisterExtension( const String& name, void* extensionPtr)
	{
		GTypeExtension* newExt = new GTypeExtension();
		newExt->Name = name;
		newExt->ExtensionPtr = extensionPtr;
		Extensions.Add(newExt);
		return newExt;
	}

	virtual GObject* CreateObject() = 0;

	virtual TType* GetType();
};

class GObjectManager
{
public:

	GObjectManager();

	/// Singleton
	static GObjectManager Instance;

	Array< GObjectType* > Factories;

	inline GObjectType* GetFactoryFromClassID(GUIClassID classID)
	{
		TArrayEnumerator< GObjectType* > ae (Factories);
		while(ae.MoveNext())
		{
			if (ae.Current->ClassID == classID)
			{
				return ae.Current;
			}
		}

		return 0;
	}

	/// TODO: We can use some indexing array stuff to fast lookup
	inline GObject* CreateObject(const String& objectName)
	{
		TArrayEnumerator< GObjectType* > ae (Factories);
		while(ae.MoveNext())
		{
			if (ae.Current->ObjectName == objectName)
			{
				return ae.Current->CreateObject();
			}
		}

		return 0;
	}

	inline GObject* CreateObject(GUIClassID classID)
	{
		GObjectType* fac =GetFactoryFromClassID(classID);
		if (fac)
		{
			return fac->CreateObject();
		}

		return 0;
	}

	inline String GetObjectName(GUIClassID classID)
	{
		GObjectType* fac = GetFactoryFromClassID(classID);
		if (fac)
		{
			return fac->ObjectName;
		}

		throw Exception("Object factory not found");
		return String::Empty;
	}

	inline void RegisterFactory(GObjectType* factory)
	{
		Factories.Add(factory);
	}
};

#endif