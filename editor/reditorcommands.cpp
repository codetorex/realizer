#include "stdafx.h"
#include "reditorcommands.h"
#include "reditor.h"

#include "twintools.h"

void REditorCommands::AddNewItem()
{

}

void REditorCommands::AddExistingItem()
{

}

void REditorCommands::AddNewFolder()
{
	RProjectNode* pn = (RProjectNode*)Editor.ProjectView.SelectedNode;
	if (!pn) return;
	if (pn->Type == RProjectNode::RS_PROJECT || pn->Type == RProjectNode::RS_FOLDER)
	{
		RProjectFolder* newFolder = new RProjectFolder();
		newFolder->Text = "New Folder";
		pn->AddNode(newFolder);
		newFolder->BeginEdit();
	}
}

void REditorCommands::Rename()
{
	if (Editor.ProjectView.SelectedNode)
	{
		Editor.ProjectView.SelectedNode->BeginEdit();
	}
}

void REditorCommands::NewProject()
{
	Editor.Project = new RProject();
	Editor.Project->setProjectName("untitled");

	Editor.Project->TreeView = &Editor.ProjectView;
	Editor.ProjectView.RootNode = Editor.Project;

	// TODO: open a window and ask for project name
}

void REditorCommands::SaveProject()
{
	if (Editor.Project)
	{
		Editor.Project->SaveAs("c://TestProject.xml");
	}
}

void REditorCommands::ImportFolder()
{
	RProjectNode* pn = (RProjectNode*)Editor.ProjectView.SelectedNode;
	if (!pn)
	{
		TWinTools::ShowMessage("No node selected for import folder");
	}

	if (pn->Type != RProjectNode::RS_FOLDER && pn->Type != RProjectNode::RS_PROJECT)
	{
		TWinTools::ShowMessage("Selected project node is not appropriate for importing live folder");
	}

	TString result;
	bool picked = TWinTools::BrowseFolder(Application.StartupPath,result);
	if (!picked)
	{
		return;
	}

	RProjectLiveFolder* newLiveFolder = new RProjectLiveFolder();
	newLiveFolder->Text = TPath::GetDirectoryName(result);
	newLiveFolder->FolderPath = result;
	pn->AddNode(newLiveFolder);
	newLiveFolder->EnsureVisible();
	newLiveFolder->UpdateLiveFolder();
}