#include "stdafx.h"
#include "vnodeanimation.h"
#include "cengine.h"


void VNodeAnimation::Update()
{
	AdvanceTime( Engine.Time.TimeDiff );
}