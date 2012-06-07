#include "stdafx.h"
#include "reditorcommands.h"
#include "reditor.h"


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
		newFolder->Image = &Editor.ProjectViewImages->GetImage(1);
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

	Editor.Project->Image = &Editor.ProjectViewImages->GetImage(0);
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
