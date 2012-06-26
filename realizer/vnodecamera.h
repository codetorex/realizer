#ifndef VNODECAMERA_H
#define VNODECAMERA_H

#include "vnode.h"
#include "vcamera.h"

class VNodeCamera: public VNode, public VCamera
{
public:
	inline void Render() 
	{
		SetRenderer();
	};
};

class CController
{
public:

};

class VCameraController
{
public:

};

#endif