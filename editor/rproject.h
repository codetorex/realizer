#ifndef RPROJECT_H
#define RPROJECT_H

#include "gtreeview.h"

class RProjectNode: public GTreeNode
{
public:
	enum RNodeTypes
	{
		RS_CODE_CPP,
		RS_CODE_H,
		RS_TERRAIN,
	};

	RNodeTypes Type;

	virtual void UpdateText() {}

};

/**
 * Folder like stuff.
 */
class RProjectFilter: public RProjectNode
{
public:

};

class RProjectSource: public RProjectNode
{
public:
	
};

class RProject: public RProjectNode
{
public:

	RProject()
	{
		Text = "Project";
	}

	TString FilePath;
	TString ProjectName;
	
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
};


#endif