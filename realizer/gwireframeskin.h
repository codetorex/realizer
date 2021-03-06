#ifndef GWIREFRAMESKIN_H
#define GWIREFRAMESKIN_H

#include "gskin.h"


class GWireFrameSkin: public GSkin
{
public:
	void RenderHilight(int x,int y, int w,int h);

	void RenderWindow( GWindow* window );

	void LayoutWindow( GWindow* window );

	void RenderButton( GButton* button );

	void RenderLabel( GLabel* label );

	void RenderCheckBox( GCheckBox* checkbox ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void RenderRadioButton( GRadioButton* radiobutton ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void RenderProgressBar( GProgressBar* progressbar ) 
	{
		throw NotImplementedException(__FILE__,__LINE__);
	}

	void RenderTextBox( GTextBox* textbox );

	void RenderSunkEdge( GObject* object );

	void LayoutSunkEdge(GObject* object);

	void RenderMenuStrip(GMenuStrip* menustrip);

	void RenderDropDown(GDropDown* dropdown);
	
	void LayoutDropDown(GDropDown* dropdown);

	void RenderMenuItem(GMenuItem* menuItem);

	void RenderMenuStripItem(GMenuItem* menuItem);

	void LayoutMenuStripItem(GMenuItem* menuItem);

	void RenderToolStrip(GToolStrip* toolbox);

	void RenderToolButton(GToolStripButton* button);

	void LayoutToolButton(GToolStripButton* button);

	void RenderScrollBar(GScrollBar* scrollbar);
	
	void LayoutScrollBar(GScrollBar* scrollbar);

	void RenderTabControl(GTabControl* tabc);

	void RenderTabButton(GButtonBase* tabb, bool active);

	void LayoutTabPage(GTabPage* tabp);

	void RenderSystemButton(GSystemButton* sysb);

	void LayoutSystemButton(GSystemButton* sysb);

	void DrawDottedHorizontalLine(int x,int y, int width);

	void DrawDottedVerticalLine(int x, int y, int height);

	void RenderTreeNode(GTreeNode* n, int x, int y);

private:
	void RenderScrollBarButton(GScrollBarButton* button);
	void LayoutScrollBarButton(GScrollBarButton* button);
};

#endif