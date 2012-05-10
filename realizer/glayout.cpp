#include "stdafx.h"
#include "glayout.h"
#include "gobject.h"

GLayout GLayout::Instance;

//GLayoutVertical GLayoutVertical::Instance;


GLayoutHorizontalOverflow GLayoutHorizontalOverflow::Instance;

/*
enum DockType
{
	DCK_NODOCK,
	DCK_LEFT,
	DCK_TOP,
	DCK_RIGHT,
	DCK_BOTTOM,
	DCK_FILL,
};
*/

TRectangle GLayout::Layout( GObject* container, bool measureOnly )
{
	TRectangle remainingArea(0,0,container->ObjectRegion.Width,container->ObjectRegion.Height);

	TLinkedListEnumerator< GObject* > c(container);
	while(c.MoveNext())
	{
		GObject* cur = c.Current;
		cur->Layout();
		if (cur->Dock == DCK_TOP)
		{
			if (!measureOnly)
			{
				cur->SetSize(remainingArea.X,remainingArea.Y,remainingArea.Width,cur->Height);
				cur->Layout();
			}
			
			remainingArea.Y += cur->Height;
			remainingArea.Height -= cur->Height;
		}
		else if ( cur->Dock == DCK_LEFT)
		{
			if (!measureOnly)
			{
				cur->SetSize(remainingArea.X,remainingArea.Y,cur->Width,remainingArea.Height);
				cur->Layout();
			}
			
			remainingArea.X += cur->Width;
			remainingArea.Width -= cur->Width;
		}
		else if ( cur->Dock == DCK_RIGHT)
		{
			if (!measureOnly)
			{
				cur->SetSize(remainingArea.Width - cur->Width,remainingArea.Y,cur->Width,remainingArea.Height);
				cur->Layout();
			}
			
			remainingArea.Width -= cur->Width;
		}
		else if (cur->Dock == DCK_BOTTOM)
		{
			if (!measureOnly)
			{
				cur->SetSize(remainingArea.X,remainingArea.Height - cur->Height,remainingArea.Width,cur->Height);
				cur->Layout();
			}
			
			remainingArea.Height -= cur->Height;
		}
		else if (cur->Dock == DCK_FILL)
		{
			if (!measureOnly)
			{
				cur->SetSize(remainingArea.X,remainingArea.Y,remainingArea.Width,remainingArea.Height);
				cur->Layout();
			}

			// fill should be done last so it wont use all the space up
		}
	}

	return remainingArea;
}

TRectangle GLayoutHorizontalOverflow::Layout( GObject* container, bool measureOnly )
{
	int rowHeight;
	TRectangle remainingArea(0,0,container->ObjectRegion.Width,container->ObjectRegion.Height);

	int heightMargins = container->Height - remainingArea.Height;

	int thisRow = 0;
	int finalObjectHeight = 0;

	TLinkedListEnumerator< GObject* > c(container);
	while(c.MoveNext())
	{
		GObject* cur = c.Current;

		if (cur->Width > remainingArea.Width && cur->Width < container->ObjectRegion.Width)
		{
			finalObjectHeight += thisRow;

			remainingArea.X = 0;
			// CREATE NEW LINE CUZ OVERFLOW
			remainingArea.Y += thisRow;
			//remainingArea.Height += rowHeight;
			remainingArea.Width = container->ObjectRegion.Width;

			thisRow = 0;

			/*if (remainingArea.Height < rowHeight)
			{
				// todo: fix this formula
				int req = rowHeight - remainingArea.Height;
				container->SetHeight(container->Height + req);
				remainingArea.Height += req;
			}*/
		}

		if (thisRow < cur->Height)
		{
			thisRow = cur->Height; // find min object size in this row.
		}

		cur->SetSize(remainingArea.X,remainingArea.Y,cur->Width,cur->Height);
		remainingArea.X += cur->Width;
		remainingArea.Width -= cur->Width;
	}

	if (thisRow > 0)
	{
		finalObjectHeight += thisRow;
	}

	container->SetHeight(finalObjectHeight + heightMargins);

	return remainingArea;
}

/*TRectangle GLayoutVertical::Layout( GObject* container, bool measureOnly )
{
	TRectangle remainingArea(container->ObjectRegion);
}*/