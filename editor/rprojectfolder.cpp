#include "stdafx.h"
#include "rprojectfolder.h"
#include "txmlwriter.h"
#include "reditor.h"

RProjectFolder::RProjectFolder()
{
	Type = RProjectNode::RS_FOLDER;
	Image = Resources.PVFolder;
}

void RProjectFolder::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("folder");
	xw.WriteAttributeString("name", Text);

	SerializeNodes(xw);

	xw.WriteEndElement();
}

RProjectLiveFolder::RProjectLiveFolder()
{
	Type = RProjectNode::RS_LIVEFOLDER;
	Image = Resources.PVLiveFolder;
}

void RProjectLiveFolder::Serialize( TXMLWriter& xw )
{
	xw.WriteStartElement("livefolder");
	xw.WriteAttributeString("name", Text);
	xw.WriteAttributeString("path", FolderPath);
	xw.WriteEndElement();
}

void RProjectLiveFolder::UpdateLiveFolder()
{
	Nodes.DeletePointers();
	TDirectoryInfo baseDir(FolderPath);
	ParseFolder(&baseDir, this);
}

void RProjectLiveFolder::ParseFolder( IDirectory* folder, RProjectNode* target )
{
	IFolderEnumerator* folders = folder->EnumerateDirectories();
	while(folders->MoveNext())
	{
		RProjectLiveFolder* newFolder = new RProjectLiveFolder();
		newFolder->FolderPath = folders->Current->GetFullName();
		newFolder->UpdateText();
		target->AddNode(newFolder);

		ParseFolder(folders->Current, newFolder);
	}

	IFileEnumerator* files = folder->EnumerateFiles();
	while(files->MoveNext())
	{
		RProjectSource* newSource = new RProjectSource(files->Current->GetFullName());
		newSource->UpdateType();
		newSource->UpdateText();
		target->AddNode(newSource);
	}
}

void RProjectLiveFolder::UpdateText()
{
	Text = TPath::GetDirectoryName(FolderPath);
}