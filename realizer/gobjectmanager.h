#ifndef GOBJECTMANAGER_H
#define GOBJECTMANAGER_H

#include "tstring.h"
#include "genums.h"
#include "tarray.h"

class GObject;

enum GDefinitionExtensionTypes
{
	DET_TOOLBOXICON,
};

class GDefinitionExtension
{
public:
	ui32 ExtensionType;
	
	virtual TType* GetMemberInfo() 
	{ 
		return 0; 
	};
};

class GDefinitionExtensionToolboxIcon: public GDefinitionExtension
{
public:
	GDefinitionExtensionToolboxIcon()
	{
		ExtensionType = DET_TOOLBOXICON;
		TTypeBuilder mib(&MemberInfo);
		mib.AddMember("ToolboxIcon",mib.GetOffset(&ToolboxIcon),MT_POINTER);
	}

	GImage* ToolboxIcon;

	static TType MemberInfo;
};

/**
 * GUI Object Type Definition and Factory
 * TODO: we can create a base TType class and derive these stuff from it.
 */
class GObjectType
{
public:
	/// This factory creates objects having this class ID
	GUIClassID ClassID;

	/// This factory creates objects named as this
	TString ObjectName;

	/// Small description about
	TString Description;

	TArray< GDefinitionExtension* > Extensions;

	virtual GObject* CreateObject() = 0;
};

class GObjectManager
{
public:

	GObjectManager();

	/// Singleton
	static GObjectManager Instance;

	TArray< GObjectType* > Factories;

	inline GObjectType* GetFactoryFromClassID(GUIClassID classID)
	{
		TArrayEnumerator< GObjectType* > ae (Factories);
		if (ae.Current->ClassID == classID)
		{
			return ae.Current;
		}

		return 0;
	}

	/// TODO: We can use some indexing array stuff to fast lookup
	inline GObject* CreateObject(const TString& objectName)
	{
		TArrayEnumerator< GObjectType* > ae (Factories);
		if (ae.Current->ObjectName == objectName)
		{
			return ae.Current->CreateObject();
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

	inline TString GetObjectName(GUIClassID classID)
	{
		GObjectType* fac = GetFactoryFromClassID(classID);
		if (fac)
		{
			return fac->ObjectName;
		}

		return 
	}

	inline void RegisterFactory(GObjectType* factory)
	{
		Factories.Add(factory);
	}
};

#endif