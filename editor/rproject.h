#ifndef RPROJECT_H
#define RPROJECT_H

#include "gtreeview.h"

#include "tserialization.h"

class TXMLWriter;

class RProjectNode: public GTreeNode
{
public:
	enum RNodeTypes
	{
		RS_PROJECT,
		RS_FOLDER,
		RS_LIVEFOLDER,
		RS_SOURCE,
	};

	RNodeTypes Type;

	virtual void UpdateText() {}

	virtual void Serialize(TXMLWriter& xw) = 0;

	void SerializeNodes(TXMLWriter& xw);
};

class RProject: public RProjectNode
{
public:
	RProject();

	String FilePath;
	String ProjectName;

	void setProjectName(const String& newProjectName)
	{
		ProjectName = newProjectName;
		UpdateText();
	}
	
	void Load(const String& projectPath);

	void SaveAs(const String& projectPath);

	/**
	 * If loaded from a file you can call this
	 */
	void Save();

	/**
	 * Imports project files from other sources like VS2010
	 */
	void ImportProject(const String& projectPath);

	/**
	 * Synchronises this project files from other sources like VS2010 CodeBlocks etc.
	 */
	void SynchronizeProject(const String& projectPath);

	void UpdateText();

	void Serialize(TXMLWriter& xw);
};

#include "rprojectfolder.h"
#include "rprojectsource.h"


#endif