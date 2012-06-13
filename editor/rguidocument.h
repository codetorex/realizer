#ifndef RGUIDOCUMENT_H
#define RGUIDOCUMENT_H

#include "rtextdocument.h"

#include "gtreeview.h"


class GGUIItem: public GTreeNode
{
public:
	/// The object associated with the node
	GObject* Object;
};

/**
 * GUI definition is basically an XML file. Similar to other xml based gui definitions.
 */
class RGUIDocument: public RTextDocument
{
public:
	GGUIItem* RootNode;

	void InitializeRoot();

	void InitializeEmptyDocument();
};


#endif