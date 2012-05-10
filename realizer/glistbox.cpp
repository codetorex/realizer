#include "stdafx.h"
#include "glistbox.h"
#include "vgui.h"
#include "gfont.h"


GListBox::GListBox()
{
	ClassID = GLISTBOX_CLASSID;
}

void GListBox::Layout()
{
	if (ItemCount < 1)
	{
		VScrollBar.SetSize(0,0,100,100);
		VScrollBar.Dock = DCK_RIGHT;
		AddChild(&VScrollBar);

		VScrollBar.SmallChange = Font->Height + 4; // 2 px top and 2px bottom margin
		VScrollBar.LargeChange = ObjectRegion.Height;

		VScrollBar.MaxValue = Items.Count * VScrollBar.SmallChange;
		VScrollBar.MinValue = 0;
	}

	OwnObject(&VScrollBar);
	Skin->LayoutSunkEdge(this);
	LayoutChilds();
}

void GListBox::Render()
{
	Skin->RenderSunkEdge(this);
	
	
	this->GObject::Render(); // render kids (scrollbar)
}
