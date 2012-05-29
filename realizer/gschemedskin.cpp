#include "stdafx.h"
#include "gschemedskin.h"
#include "vtexture.h"
#include "tbitmap.h"
#include "cengine.h"
#include "gfont.h"

#include "gcomponents.h"

#include "tconvert.h"

void GSchemedSkin::RenderWindow( GWindow* window )
{
	int windowState = window->IsActive() ? 0 : 1;

	Engine.Draw.SetTexture(SkinTexture);
	WindowQuad[windowState].Render(window);
	
	TColor32 renColor = window->IsActive() ? Colors.TitleText : Colors.InactiveTitleText;

	// TODO: cache string pixel width in somewhere
	WindowTitleFont->Render(window->Text,window->TitleBar->DrawRegion,WindowTitleAlign, renColor , WindowQuad->TopLeft.Width ,-1 );
	//WindowTitleFont->Render(window->Text,window->TitleBar->ScreenRegion,CA_MiddleLeft,WindowTitleColor[windowState] );
}

void GSchemedSkin::LayoutWindow( GWindow* window )
{
	int windowState = window->IsActive() ? 0 : 1;
	GScalableQuadParted& q = WindowQuad[windowState];

	window->TitleBar->SetRectangle(0,-q.Top.Height,window->Width,q.Top.Height);

	q.SetObjectContent(window);
}

void GSchemedSkin::RenderButton( GButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ButtonGfx.Render(button);
	//ButtonQuad[button->ButtonGraphic].Render(button);

	button->Font->Render(button->Text,button->DrawRegion,button->TextAlign,button->ForeColor,0,-1);
}

void GSchemedSkin::RenderLabel( GLabel* label )
{
	Engine.Draw.SetTexture(SkinTexture); // TODO: wtf is this shit?
	label->Font->Render(label->Text,label->DrawRegion,label->TextAlign,label->ForeColor,0,-1);
}

void GSchemedSkin::RenderCheckBox( GCheckBox* checkbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	//checkbox->Height = CheckBoxQuad[0].Height; // TODO: fix this and use checkalign
	CheckBoxGfx[checkbox->CheckState].Render(checkbox);
	checkbox->Font->Render(checkbox->Text,checkbox->DrawRegion,checkbox->TextAlign,checkbox->ForeColor,CheckBoxGfx[0].Width+5,-1);
}

void GSchemedSkin::RenderRadioButton( GRadioButton* radiobutton )
{
	Engine.Draw.SetTexture(SkinTexture);
	//radiobutton->Height = RadioQuad[0].Height; // TODO: fix this and use checkalign
	//RadioQuad[radiobutton->GraphicState].Draw((float)radiobutton->ScreenRegion.X,(float)radiobutton->ScreenRegion.Y,0xFFFFFFFF);
	RadioGfx[radiobutton->Checked ? 1 : 0].Render(radiobutton);
	radiobutton->Font->Render(radiobutton->Text,radiobutton->DrawRegion,radiobutton->TextAlign,radiobutton->ForeColor,RadioGfx[0].Width+5,-1);
}

void GSchemedSkin::RenderProgressBar( GProgressBar* progressbar )
{
	Engine.Draw.SetTexture(SkinTexture);
	ProgressBarBg.Render(progressbar);

	if (progressbar->Value > progressbar->Minimum)
	{
		if (progressbar->Value != progressbar->Maximum)
		{
			IRegion progressRegion;
			progressRegion.SetRegion(progressbar->DrawRegion);
			progressRegion.ChangeWidth(progressbar->GetPercentWidth());
			ProgressBarBlock.Render(progressRegion);
		}
		else
		{
			ProgressBarBlock.Render(progressbar);
		}
	}

	//progressbar->Value = (progressbar->Value + 1) % progressbar->Maximum;
	
	if (progressbar->ShowPercent)
	{
		TString* pbstr = &progressbar->Text;
		pbstr->Clear();
		TConvert::AppendToString(progressbar->GetPercentage(),*pbstr);
		pbstr->AppendASCIIFast(' ');
		pbstr->AppendASCIIFast('%');

		progressbar->Font->Render(progressbar->Text,progressbar->DrawRegion,progressbar->PercentAlign,progressbar->ForeColor,0,-1);
	}
}

void GSchemedSkin::RenderTextBox( GTextBox* textbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	SunkEdge[0].Render(textbox);
	//textbox->Font->Render(textbox->Text,textbox->ScreenRegion,CA_TopLeft,textbox->ForeColor,SunkEdge[0].LeftMargin,-1);
}

void GSchemedSkin::RenderSunkEdge( GObject* object )
{
	Engine.Draw.SetTexture(SkinTexture);
	SunkEdge[0].Render(object);
}


void GSchemedSkin::LayoutSunkEdge( GObject* object )
{
	SunkEdge[0].SetObjectRegion(object);
}

void GSchemedSkin::RenderMenuStrip( GMenuStrip* menustrip )
{
	int mod = 1;
	if (menustrip->Enabled)
		mod = 0;
	
	Engine.Draw.SetTexture(SkinTexture);
	MenuBarBg[mod].Render(menustrip);
}

void GSchemedSkin::RenderDropDown( GDropDown* dropdown )
{
	Engine.Draw.SetTexture(SkinTexture);
	DropDownBg.Render(dropdown);
}

void GSchemedSkin::LayoutDropDown( GDropDown* dropdown )
{
	DropDownBg.SetObjectRegion(dropdown);
}

void GSchemedSkin::RenderMenuItem( GMenuItem* menuItem )
{
	Engine.Draw.SetTexture(SkinTexture);
	MenuItemBg[menuItem->GraphicState].Render(menuItem);
}

void GSchemedSkin::RenderMenuStripItem( GMenuItem* menuItem )
{
	Engine.Draw.SetTexture(SkinTexture);
	MenuBarButton[menuItem->GraphicState].Render(menuItem);
}

void GSchemedSkin::LayoutMenuStripItem( GMenuItem* menuItem )
{
	MenuBarButton[0].SetObjectRegion(menuItem);
}

void GSchemedSkin::RenderToolStrip( GToolStrip* toolbox )
{
	Engine.Draw.SetTexture(SkinTexture);
	ToolBarBg.Render(toolbox);
}

void GSchemedSkin::LayoutToolButton( GToolStripButton* button )
{
	ToolButton[0].SetObjectRegion(button);
}

void GSchemedSkin::RenderToolButton( GToolStripButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ToolButton[button->GraphicState].Render(button);
}

void GSchemedSkin::RenderScrollBarButton( GScrollBarButton* button )
{
	Engine.Draw.SetTexture(SkinTexture);
	ScrollbarButtonGfx[button->Direction].Render(button);
}

void GSchemedSkin::LayoutScrollBarButton( GScrollBarButton* button )
{
	button->ChangeWidth(ScrollbarButtonGfx[0].Width);
	button->ChangeHeight(ScrollbarButtonGfx[1].Height);
	button->UpdateContent();
}

void GSchemedSkin::RenderScrollBar( GScrollBar* scrollbar )
{
	Engine.Draw.SetTexture(SkinTexture);
	if (scrollbar->Orientation == GO_VERTICAL)
	{
		ScrollbarBgVGfx.Render(scrollbar);
		if (scrollbar->DragBar->Height <= ScrollbarDragSmallVGfx.Height)
		{
			ScrollbarDragSmallVGfx.Render(scrollbar->DragBar);
		}
		else
		{
			ScrollbarDragVGfx.Render(scrollbar->DragBar); // check if we going to render smallest one
		}
	}
	else
	{
		ScrollbarBgHGfx.Render(scrollbar);
		if (scrollbar->DragBar->Width <= ScrollbarDragSmallHGfx.Width)
		{
			ScrollbarDragSmallHGfx.Render(scrollbar->DragBar);
		}
		else
		{
			ScrollbarDragHGfx.Render(scrollbar->DragBar);
		}
	}

	RenderScrollBarButton(scrollbar->UpButton);

	RenderScrollBarButton(scrollbar->DownButton);
}

void GSchemedSkin::LayoutScrollBar( GScrollBar* scrollbar )
{
	LayoutScrollBarButton(scrollbar->UpButton);
	LayoutScrollBarButton(scrollbar->DownButton);

	GScrollbarLayout st(*scrollbar);

	st.Calculate();
	st.SetDragSize();

	if (scrollbar->Orientation == GO_VERTICAL)
	{
		scrollbar->ChangeWidth(ScrollbarButtonGfx[0].Width);
		scrollbar->DragBar->ChangeWidth(scrollbar->Width);	

		scrollbar->UpButton->Move(0,0);
		scrollbar->DownButton->Move(0,scrollbar->Height- scrollbar->DownButton->Height);
		scrollbar->DragBar->Move(0,scrollbar->UpButton->Bottom());

		if (scrollbar->DragBar->Height < ScrollbarDragSmallVGfx.Height)
		{
			scrollbar->DragBar->ChangeHeight(ScrollbarDragSmallVGfx.Height);
			st.Calculate(false);
		}
	}
	else
	{
		scrollbar->ChangeHeight(ScrollbarButtonGfx[0].Height);
		scrollbar->DragBar->ChangeHeight(scrollbar->Height);	

		scrollbar->UpButton->Move(0,0);
		scrollbar->DownButton->Move(scrollbar->Width- scrollbar->DownButton->Width,0);
		scrollbar->DragBar->Move(scrollbar->UpButton->Right(),0);

		if (scrollbar->DragBar->Width < ScrollbarDragSmallHGfx.Width)
		{
			scrollbar->DragBar->ChangeWidth(ScrollbarDragSmallHGfx.Width);
			st.Calculate(false);
		}
	}

	scrollbar->Content.SetRectangle(0,0,scrollbar->Width,scrollbar->Height);

	st.CalculateDragPosition();
	st.SetDragPos();
}

void GSchemedSkin::RenderHilight( int x,int y, int w,int h )
{
	Engine.Draw.SetTexture(SkinTexture);
	WhitePart.DrawScaled(x,y,w,h,Colors.Hilight);
}

void GSchemedSkin::RenderTabControl( GTabControl* tabc )
{
	Engine.Draw.SetTexture(SkinTexture);

	IRegion borderRegion;
	borderRegion.SetRegion(tabc->DrawRegion.X()+tabc->PageArea.X,tabc->DrawRegion.Y() + tabc->PageArea.Y,tabc->PageArea.Width,tabc->PageArea.Height);
	TabBorder.Render(borderRegion);

	// lets render tab pages
	/*TLinkedListEnumerator<GObject*> btn(&tabc->TabPageButtons);
	while(btn.MoveNext())
	{
		GTabButton* curButton = (GTabButton*)btn.Current;
		bool active = curButton->Page == tabc->CurrentPage;
		RenderTabButton(curButton,active);
	}*/
}

void GSchemedSkin::LayoutTabPage( GTabPage* tabp )
{
	
}

void GSchemedSkin::RenderTabButton( GButtonBase* tabb, bool active )
{
	Engine.Draw.SetTexture(SkinTexture);
	GSchemedSkinButtonQuad* qd;
	if (tabb == tabb->Parent->FirstItem)
	{
		qd = &TabPageLeft;
	}
	else if (tabb == tabb->Parent->LastItem)
	{
		qd = &TabPageRight;
	}
	else
	{
		qd = &TabPage;
	}

	if (active)
	{
		IRegion tmpRegion;
		tmpRegion.SetRegion(tabb->DrawRegion);
		tmpRegion.Inflate(1);
		tmpRegion.ChangeHeightDiff(1);
		tmpRegion.MoveYDiff(-1);
		qd->Render(tabb,tmpRegion);
	}
	else
	{
		qd->Render(tabb);
	}

	IRegion tmpRegion = tabb->DrawRegion;
	tmpRegion.Shrink( tabb->Padding );

	// TODO: we can move this to another loop
	tabb->Font->Render(tabb->Text,tmpRegion,tabb->TextAlign,tabb->ForeColor);
	Engine.Draw.SetTexture(SkinTexture);
}

void GSchemedSkin::RenderSystemButton( GSystemButton* sysb )
{
	switch(sysb->ButtonType)
	{
	case GSystemButton::BT_TOOLWINDOW_CLOSE:
		ToolWindowClose.Render(sysb);
		return;

	default:
		throw NotImplementedException();
	}
}

void GSchemedSkin::LayoutSystemButton( GSystemButton* sysb )
{
	switch(sysb->ButtonType)
	{
	case GSystemButton::BT_TOOLWINDOW_CLOSE:
		sysb->SetSize(ToolWindowClose.Width,ToolWindowClose.Height);
		return;

	default:
		throw NotImplementedException();
	}
}


/**
 * TODO: clean this mess up and probably move rendering code to treeview?
 */
void GSchemedSkin::RenderTreeNode(GTreeNode* n, int x, int y)
{
	Engine.Draw.SetTexture(SkinTexture);

	int curX = x; // 0 this time

	GTreeView* tv = n->TreeView;
	bool ShowRoot = tv->ShowRoot;

	if (ShowRoot && tv->ShowPlusMinus)
	{
		curX += 16;
	}
	if (tv->ShowLines)
	{
		int lineimg = 1;
		if (n->Parent)
		{
			if (n == n->Parent->Nodes.GetLast())
			{
				lineimg = 2;
			}
			else
			{
				lineimg = 1;
			}

			int lvl = n->Level-1;
			if (!ShowRoot)
			{
				lvl--;
			}

			if (tv->ShowPlusMinus && !ShowRoot)
			{
				curX += 16;
			}

			while(lvl-- > 0)
			{
				DotLine[0].Draw(curX,y);
				curX += 16;
			}
			bool isRootChild = n->Parent == &(n->TreeView->RootNode);
			if ((isRootChild && ShowRoot) || !isRootChild)
			{
				DotLine[lineimg].Draw(curX,y);
				curX += 16;
			}
		}
	}
	else
	{
		if (!tv->ShowRoot && !tv->ShowPlusMinus)
		{
			curX += (n->Level-1) * 16;
		}
		else
		{
			curX += (n->Level) * 16;
		}
		
	}

	if (tv->ShowPlusMinus && (n->Nodes.Count > 0))
	{
		/*if (!ShowRoot)
		{
			curX += 16;
		}*/
		
		int pmimg = 0;
		if (n->Expanded)
		{
			if (n->MouseOver)
			{
				pmimg = 3;
			}
			else
			{
				pmimg = 1;
			}
		}
		else
		{
			if (n->MouseOver)
			{
				pmimg = 2;
			}
		}

		// draw on middle
		int expX = (curX - 16) + ((16 - TreeViewPlusMinus->Width) / 2) + 1;
		int expY = y + ((tv->NodeHeight - TreeViewPlusMinus->Height) /2)+ 1;

		TreeViewPlusMinus[pmimg].Draw(expX,expY);
	}

	if (n->Image)
	{
		int imageY = ((int)tv->NodeHeight - n->Image->Height) / 2;
		n->Image->Render(curX,y + imageY);
		curX += n->Image->Width + 3;
	}

	int textLength = -1;
	if (tv->SelectedNode == n)
	{
		TCharacterEnumerator ce(n->Text); // TODO: fix this shit, getstringwidth should accept Tstring instead of char enumerator
		textLength = tv->Font->GetStringWidth(ce);
		RenderHilight(curX-2,y+1,textLength+4,tv->NodeHeight-2);
	}

	tv->Font->Render(n->Text,curX,tv->TextYOffset + y,tv->ForeColor);
}