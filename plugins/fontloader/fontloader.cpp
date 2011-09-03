// fontloader.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "fontloader.h"


// This is an example of an exported variable
FONTLOADER_API int nfontloader=0;

// This is an example of an exported function.
FONTLOADER_API int fnfontloader(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see fontloader.h for the class definition
Cfontloader::Cfontloader()
{
	return;
}
