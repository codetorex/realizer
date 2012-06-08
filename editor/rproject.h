#ifndef RPROJECT_H
#define RPROJECT_H

#include "gtreeview.h"

#include "tserializable.h"

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

	TString FilePath;
	TString ProjectName;

	void setProjectName(const TString& newProjectName)
	{
		ProjectName = newProjectName;
		UpdateText();
	}
	
	void Load(const TString& projectPath);

	void SaveAs(const TString& projectPath);

	/**
	 * If loaded from a file you can call this
	 */
	void Save();

	/**
	 * Imports project files from other sources like VS2010
	 */
	void ImportProject(const TString& projectPath);

	/**
	 * Synchronises this project files from other sources like VS2010 CodeBlocks etc.
	 */
	void SynchronizeProject(const TString& projectPath);

	void UpdateText();

	void Serialize(TXMLWriter& xw);
};

#include "rprojectfolder.h"
#include "rprojectsource.h"


#endif