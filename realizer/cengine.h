#ifndef CENGINE_H
#define CENGINE_H

#include "vrenderer.h"
#include "vtexturemanager.h"
#include "cinput.h"

/**
* Main engine block.
*/
class RDLL REngine
{
public:
	bool running;

	VRenderer			Renderer;
	VTextureManager		Textures;
	CInputManager		Inputs;
	//CHookManager		Hooks;

	void Initialize(int width,int height, string title, bool fullscreen = false,int bits = 24);
	void Run();
};



#endif