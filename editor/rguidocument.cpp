#include "stdafx.h"
#include "rguidocument.h"
#include "reditor.h"

void RGUIDocument::InitializeRoot()
{
	GGUIItem* newItem = new GGUIItem();
	newItem->Text = "Canvas";
	newItem->Image = Resources.UICanvas;
	newItem->Object = 0;
	RootNode = newItem;
}

void RGUIDocument::InitializeEmptyDocument()
{
	Name = "untitled-gui";
	InitializeRoot();
}