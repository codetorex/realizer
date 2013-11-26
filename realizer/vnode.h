#ifndef VNODE_H
#define VNODE_H

#include "tstring.h"
#include "tlinkedlist.h"

enum VNodeClassID
{
	VANIMATOR_CLASSID,
	VCAMERA_CLASSID,
	VCANVAS_CLASSID,
};

class VNode: public TListNode< VNode* >
{
public:
	bool RenderEnabled;
	bool UpdateEnabled;
	bool DebugRenderEnabled;

	ui32 ClassID;

	String NodeName;

	virtual void Render() {};
	virtual void RenderDebug() {};
	virtual void Update() {};
	virtual void Initialize() {};
	virtual void Finalize() {};
};

/**
 * VNodeScene <- Scene inside node scene Sceneception
 */
#endif