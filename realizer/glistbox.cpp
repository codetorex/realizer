#include "stdafx.h"
#include "glistbox.h"
#include "vgui.h"
#include "gfont.h"
#include "glayout.h"

GListBox::GListBox()
{
	ClassID = GLISTBOX_CLASSID;
	ForeColor = TColors::Black;
	SelectedIndex = 0;
	VScrollBar.SetRectangle(0,0,100,100);
	VScrollBar.Dock = DCK_RIGHT;
}

void GListBox::Layout()
{
	if (ItemCount < 1)
	{
		AddChild(&VScrollBar);
	}

	OwnObject(&VScrollBar);
	Skin->LayoutSunkEdge(this);

	ItemHeight = Font->Height + 4;
	int screenSpace = Content.Height / ItemHeight;
	VScrollBar.LargeChange = screenSpace;

	if (Items.Count > screenSpace)
	{		
		VScrollBar.MaxValue = Items.Count;
		VScrollBar.Visible = true;
		GLayout::Instance.Layout(this,false);
		VScrollBar.Layout();
	}
	else
	{
		VScrollBar.Value = 0;
		VScrollBar.Visible = false;
	}
}

void GListBox::Render()
{
	Skin->RenderSunkEdge(this);
	
	if (Items.Count == 0)
	{
		return;
	}

	int itemStart = VScrollBar.Value;
	int itemEnd = itemStart + VScrollBar.LargeChange;
	int drawX = DrawRegion.X() + Content.X + 2; // 2px FROM me
	int drawY = DrawRegion.Y() + Content.Y;
	
	

	if (itemEnd >= Items.Count)
	{
		itemEnd = Items.Count;
	}

	for (int i=itemStart;i< itemEnd;i++)
	{
		// TODO: we can call virtual OnItemRender() here for customized item rendering
	
		if (i == SelectedIndex)
		{
			int HilightWidth = VScrollBar.Visible ? VScrollBar.X : Content.Width;
			Skin->RenderHilight(drawX-2,drawY,HilightWidth,ItemHeight);
		}

		Font->Render(*Items.Items[i],drawX,drawY, TColors::Black);
		drawY += ItemHeight;
		// TODO: move font rendering to TGraphics and move these rendering stuff to graphics
	}
	
	if (VScrollBar.Visible)
	{
		VScrollBar.Render();
	}
	//this->GObject::Render(); // render kids (scrollbar)
}

void GListBox::AddItem( String& value )
{
	String* str = new String(value);
	Items.Add( str );
	if (Items.Count > VScrollBar.LargeChange)
	{
		Layout();
	}
	else
	{
		VScrollBar.Value = 0;
	}
}

void GListBox::OnMouseDown( int x,int y, int button )
{
	int pickedItem = (y - Content.Y) / ItemHeight;
	int realIndex = pickedItem + VScrollBar.Value;
	SelectedIndex = Math.Clamp<int>(0,Items.Count-1,realIndex);
}

void GListBox::OnMouseWheel( int x,int y, int delta )
{
	if (VScrollBar.Visible)
	{
		VScrollBar.OnMouseWheel(VScrollBar.Width / 2,y,delta);
	}
}