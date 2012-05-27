#ifndef GSKIN_H
#define GSKIN_H

#include "tcolor.h"

class GWindow;
class GButton;
class GButtonBase;
class GObject;
class GFont;
class GLabel;
class GCheckBox;
class GRadioButton;
class GProgressBar;
class GTextBox;
class GMenuStrip;
class GDropDown;
class GMenuItem;
class GToolStrip;
class GToolStripButton;
class GScrollBarButton;
class GScrollBar;
class GTabControl;
class GTabPage;
class GTabButton;
class GSystemButton;
class GTreeNode;

class GSkin
{
public:
	GFont*	 SystemFont;
	TColor32 DefaultFontColor;

	virtual void RenderHilight(int x, int y, int w, int h) = 0;

	virtual void RenderWindow(GWindow* window) = 0;

	virtual void LayoutWindow(GWindow* window) = 0;

	virtual void RenderButton(GButton* button) = 0;

	virtual void RenderLabel(GLabel* label) = 0;

	virtual void RenderCheckBox(GCheckBox* checkbox) = 0;

	virtual void RenderRadioButton(GRadioButton* radiobutton) = 0;

	virtual void RenderProgressBar(GProgressBar* progressbar) = 0;

	virtual void RenderTextBox(GTextBox* textbox) = 0;

	virtual void RenderSunkEdge(GObject* object) = 0;

	virtual void LayoutSunkEdge(GObject* object) = 0;

	virtual void RenderMenuStrip(GMenuStrip* menustrip) = 0;

	virtual void RenderDropDown(GDropDown* dropdown) = 0;

	virtual void LayoutDropDown(GDropDown* dropdown) = 0;

	virtual void RenderMenuItem(GMenuItem* menuItem) = 0;

	virtual void RenderMenuStripItem(GMenuItem* menuItem) = 0;

	virtual void LayoutMenuStripItem(GMenuItem* menuItem) = 0;

	virtual void RenderToolStrip(GToolStrip* toolbox) = 0;

	virtual void RenderToolButton(GToolStripButton* button) = 0;

	virtual void LayoutToolButton(GToolStripButton* button) = 0;

	virtual void RenderScrollBar(GScrollBar* scrollbar) = 0;

	virtual void LayoutScrollBar(GScrollBar* scrollbar) = 0;
	
	virtual void RenderTabButton(GButtonBase* tabb, bool active) = 0;

	virtual void RenderTabControl(GTabControl* tabc) = 0;

	virtual void LayoutTabPage(GTabPage* tabp) = 0;

	virtual void RenderSystemButton(GSystemButton* sysb) = 0;

	virtual void LayoutSystemButton(GSystemButton* sysb) = 0;

	virtual void RenderTreeNode(GTreeNode* n, int x, int y) = 0;
};

#endif
