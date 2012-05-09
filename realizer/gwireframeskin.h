#ifndef GWIREFRAMESKIN_H
#define GWIREFRAMESKIN_H

#include "gskin.h"
#include "texception.h"

class GWireFrameSkin: public GSkin
{
public:
	void RenderWindow( GWindow* window );

	void LayoutWindow( GWindow* window );

	void RenderButton( GButton* button );

	void RenderLabel( GLabel* label );

	void RenderCheckBox( GCheckBox* checkbox ) 
	{
		throw NotImplementedException();
	}

	void RenderRadioButton( GRadioButton* radiobutton ) 
	{
		throw NotImplementedException();
	}

	void RenderProgressBar( GProgressBar* progressbar ) 
	{
		throw NotImplementedException();
	}

	void RenderTextBox( GTextBox* textbox );

	void RenderSunkEdge( GObject* object );

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

private:
	void RenderScrollBarButton(GScrollBarButton* button);
	void LayoutScrollBarButton(GScrollBarButton* button);
};

#endif