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
		RS_CODE_CPP,
		RS_CODE_H,
		RS_TERRAIN,
	};

	RNodeTypes Type;

	virtual void UpdateText() {}

	virtual void Serialize(TXMLWriter& xw) = 0;
};

/**
 * Folder like stuff.
 */
class RProjectFolder: public RProjectNode
{
public:
	RProjectFolder()
	{
		Type = RProjectNode::RS_FOLDER;
	}

	void Serialize(TXMLWriter& xw);
};

class RProjectSource: public RProjectNode
{
public:
	TString FilePath;

	void Serialize(TXMLWriter& xw);
};

class RProject: public RProjectNode
{
public:
	RProject()
	{
		Text = "Project";
		Type = RProjectNode::RS_PROJECT;
		
	}

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


#endif