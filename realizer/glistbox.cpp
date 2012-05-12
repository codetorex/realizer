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
}

void GListBox::Layout()
{
	if (ItemCount < 1)
	{
		VScrollBar.SetSize(0,0,100,100);
		VScrollBar.Dock = DCK_RIGHT;
		AddChild(&VScrollBar);
		VScrollBar.SmallChange = 1;
	}

	OwnObject(&VScrollBar);
	Skin->LayoutSunkEdge(this);

	ItemHeight = Font->Height + 4;
	int screenSpace = ObjectRegion.Height / ItemHeight;
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
	int drawX = ScreenRegion.X + ObjectRegion.X + 2; // 2px FROM me
	int drawY = ScreenRegion.Y + ObjectRegion.Y;
	
	

	if (itemEnd >= Items.Count)
	{
		itemEnd = Items.Count;
	}

	for (int i=itemStart;i< itemEnd;i++)
	{
		// TODO: we can call virtual OnItemRender() here for customized item rendering
	
		if (i == SelectedIndex)
		{
			int HilightWidth = VScrollBar.Visible ? VScrollBar.Left : ObjectRegion.Width;
			Skin->RenderHilight(drawX-2,drawY,HilightWidth,ItemHeight);
		}

		Font->Render(*Items.Item[i],drawX,drawY, TColors::Black);
		drawY += ItemHeight;
		// TODO: move font rendering to TGraphics and move these rendering stuff to graphics
	}
	
	if (VScrollBar.Visible)
	{
		VScrollBar.Render();
	}
	//this->GObject::Render(); // render kids (scrollbar)
}

void GListBox::AddItem( TString& value )
{
	TString* str = new TString(value);
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

void GListBox::MouseDown( int x,int y, int button )
{
	int pickedItem = (y - ObjectRegion.Y) / ItemHeight;
	int realIndex = pickedItem + VScrollBar.Value;
	SelectedIndex = MathDriver::Clamp<int>(0,Items.Count-1,realIndex);
}