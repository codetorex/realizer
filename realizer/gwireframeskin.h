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
};

#endif