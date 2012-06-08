#ifndef REDITORCOMMANDS_H
#define REDITORCOMMANDS_H

#include "gobject.h"

class REditorCommands
{
public:
	void AddNewItem();
	void AddExistingItem();
	void AddNewFolder();
	void Rename();
	void ImportFolder();

	void NewProject();
	void SaveProject();
};

extern REditorCommands EditorCommands;

class REditorEvents
{
public:
	REditorEvents()
	{
		AddNewItem      = GetHandler(&EditorCommands, &REditorCommands::AddNewItem);
		AddExistingItem = GetHandler(&EditorCommands, &REditorCommands::AddExistingItem);
		AddNewFolder    = GetHandler(&EditorCommands, &REditorCommands::AddNewFolder);
		NewProject      = GetHandler(&EditorCommands, &REditorCommands::NewProject);
		Rename          = GetHandler(&EditorCommands, &REditorCommands::Rename);
		SaveProject     = GetHandler(&EditorCommands, &REditorCommands::SaveProject);
		ImportFolder    = GetHandler(&EditorCommands, &REditorCommands::ImportFolder);
	}

	GObject::NoArgEvent* AddNewItem;
	GObject::NoArgEvent* AddExistingItem;
	GObject::NoArgEvent* AddNewFolder;
	GObject::NoArgEvent* Rename;
	GObject::NoArgEvent* ImportFolder;
	

	GObject::NoArgEvent* SaveProject;
	GObject::NoArgEvent* NewProject;
};

extern REditorEvents EditorEvents;

#endif