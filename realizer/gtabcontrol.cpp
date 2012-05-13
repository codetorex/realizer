#include "stdafx.h"
#include "gtabcontrol.h"
#include "vgui.h"
#include "glayout.h"
#include "gfont.h"

GTabPage::GTabPage()
{
	ClassID = GTABPAGE_CLASSID;
}

GTabControl::GTabControl()
{
	ClassID = GTABCONTROL_CLASSID;
	CurrentPage = 0;
	Alignment = GTB_TOP;
}

void GTabControl::Render()
{
	Skin->RenderTabControl(this);

	if (CurrentPage)
	{
		CurrentPage->Render();
	}


	byte tmp[512];
	TStringBuilder k(tmp,512);
	k.Append("Current page: ");
	k.Append(sfx((ui32)CurrentPage));

	Font->Render(k,50,50,TColors::Wheat);

	k.UnbindByteArray();
}

void GTabControl::Update()
{
	this->GObject::Update();
	
	if (CurrentPage)
	{
		CurrentPage->Update();
	}
}

void GTabControl::Layout()
{
	if (TabPageButtons.Parent == 0)
	{
		AddChild(&TabPageButtons);
	}

	ObjectRegion.SetRectangle(0,0,Width,Height); // initialize region

	// 5 soldan sagdan 4 yukardan assagidan

	TabPageButtons.Clear();

	for (int i=0;i<TabPages.Count;i++)
	{
		GTabPage* curPage = TabPages.Item[i];
		GButtonBase* pageButton = &(curPage->BaseButton);
		TabPageButtons.AddChild(pageButton);
		TCharacterEnumerator sb(pageButton->Text);
		pageButton->SetSize(0,0, Font->GetStringWidth(sb)+ 10,Font->Height + 8 );

	}

	TabPageButtons.SetSize(0,0,Width,Height);
	//TabPageButtons.ObjectRegion.SetRectangle(0,0,Width,Height);

	// TODO: support other aligments by layouting tabpagebuttons
	if (Alignment == GTB_TOP)
	{
		GLayoutHorizontalOverflow::Instance.Layout(&TabPageButtons,false);
		TabPageButtons.Dock = DCK_TOP;
	}

	PageArea = GLayout::Instance.Layout(this,false);

	for (int i=0;i< TabPages.Count;i++)
	{
		GTabPage* curPage = TabPages.Item[i];
		curPage->SetSize(PageArea.X,PageArea.Y,PageArea.Width,PageArea.Height);
		this->OwnObject(curPage);
		curPage->ObjectRegion.RectangleShrink(3); // 3 px padding TODO: make this changable
	}
}

GTabPage* GTabControl::AddPage( const TString& pageName )
{
	GTabPage* page = new GTabPage();
	page->Text = pageName;
	page->BaseButton.Text = pageName;
	TabPageButtons.AddChild(&page->BaseButton);
	TabPages.Add(page);
	Layout();
	if (CurrentPage == 0)
	{
		SelectPage(page);
	}
	return page;
}

void GTabControl::RemovePage( GTabPage* page )
{
	int pageIndex = TabPages.IndexOf(page);
	TabPages.RemoveAt(pageIndex);
	TabPageButtons.Remove(&page->BaseButton);
	if (CurrentPage == page)
	{
		SelectPage(pageIndex);
	}
	else
	{
		Layout();
	}
}

void GTabControl::SelectPage( int index )
{
	if (TabPages.Count == 0)
	{
		CurrentPage = 0;
		return;
	}
	index = MathDriver::Clamp<int>(0,TabPages.Count-1,index);
	SelectPage(TabPages.Item[index]);
}

void GTabControl::SelectPage( GTabPage* page )
{
	CurrentPage = page;
}

GObject* GTabControl::FindObject()
{
	GObject* result = 0;
	if (CurrentPage)
	{
		 result = CurrentPage->FindObject();
	}

	if (result != 0)
	{
		return result;
	}

	return this->GObject::FindObject();
}

