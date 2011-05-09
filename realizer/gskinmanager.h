#ifndef GSKINMANAGER_H
#define GSKINMANAGER_H

#include <tstring.h>
#include <tarray.h>
#include "gskin.h"

class GSkinManager: public TArray< GSkin* >
{
public:
	/**
	* Loads schemed skin from different possible formats.
	* Internally "rss" (realizer schemed skin) and "uis" (window blinds skin scheme) is supported.
	*/
	GSkin* LoadSkin(const str8& path,int textureSize = 1024);

	void ReleaseSkin(GSkin* skin);
};


#endif