
#ifndef VRENDERER_H
#define VRENDERER_H

//#include <realizertypes.h>

#include "stdafx.h"

//TODO: implement linux based Vengine

class IRenderer
{
public:

};


// Current Default = DirectX
#ifdef USE_DIRECTX9
#include "vrendererdx9.h"
typedef VRendererDX9 VRenderer;
#endif

/*#ifdef USE_OPENGL
#include <venginegl.h>
typedef VEngineGL VEngine;
#endif*/


#endif