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

	void NewProject();
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
	}

	GObject::NoArgEvent* AddNewItem;
	GObject::NoArgEvent* AddExistingItem;
	GObject::NoArgEvent* AddNewFolder;
	GObject::NoArgEvent* NewProject;
	GObject::NoArgEvent* Rename;
};

extern REditorEvents EditorEvents;

#endif