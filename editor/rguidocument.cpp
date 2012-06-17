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

GGUIItem* GGUIItem::FindObject( GObject* obj )
{
	if (obj == Object)
	{
		return this;
	}

	TArrayEnumerator< GTreeNode* > ae(Nodes);
	while(ae.MoveNext())
	{
		GGUIItem* gitem = (GGUIItem*)ae.Current;
		GGUIItem* found = gitem->FindObject(obj);
		if (found)
		{
			return found;
		}
	}

	return 0;
}