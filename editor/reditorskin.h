#ifndef REDITORSKIN_H
#define REDITORSKIN_H

#include "gschemedskinbuilder.h"

class REditorSkin: public GSchemedSkin
{
public:
	GSchemedSkinBuilder Builder;

	GSchemedSkinButtonQuad StartPageButton;

	void LoadEditorSkin();
};



#endif