#ifndef VNODEANIMATION_H
#define VNODEANIMATION_H

#include "vnode.h"
#include "vanimation.h"

class VNodeAnimation: public VNode, public VAnimationManager
{
public:
	VNodeAnimation()
	{
		RenderEnabled = false;
		UpdateEnabled = true;
		DebugRenderEnabled = false;
		ClassID = VANIMATOR_CLASSID;
	}

	void Update();
};

#endif