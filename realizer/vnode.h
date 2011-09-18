#ifndef VNODE_H
#define VNODE_H

#include "tstring.h"
#include "tlinkedlist.h"

enum VNodeClassID
{
	VANIMATOR_CLASSID,
	VCAMERA_CLASSID,
};

class VNode: public TListNode< VNode* >
{
public:
	bool RenderEnabled;
	bool UpdateEnabled;
	bool DebugRenderEnabled;

	dword ClassID;

	string NodeName;

	virtual void Render() {};
	virtual void RenderDebug() {};
	virtual void Update() {};
	virtual void Initialize() {};
	virtual void Finalize() {};
};

#endif