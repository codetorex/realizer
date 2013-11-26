#ifndef RPROJECTFOLDER_H
#define RPROJECTFOLDER_H

#include "rproject.h"

/**
 * Folder like stuff.
 */
class RProjectFolder: public RProjectNode
{
public:
	RProjectFolder();

	void Serialize(TXMLWriter& xw);
};

class RProjectLiveFolder: public RProjectNode
{
public:
	RProjectLiveFolder();

	String FolderPath;

	void Serialize(TXMLWriter& xw);

	/**
	 * Recursively scans folder and creates necessary nodes
	 */
	void ParseFolder(IDirectory* folder, RProjectNode* target);

	/**
	 * Scraps if any child node, then recreates given folder structure
	 */
	void UpdateLiveFolder();

	void UpdateText();
};

#endif