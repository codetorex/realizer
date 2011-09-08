#include "stdafx.h"
#include "gtimer.h"
#include "cengine.h"

void GTimer::SetLastCheck()
{
	if (RealTime)
	{
		LastCheck = Engine.Time.RealTimeMS;
	}
	else
	{
		LastCheck = Engine.Time.CurrentTimeMS;
	}
}

dword GTimer::GetDiff()
{
	if (RealTime)
	{
		return Engine.Time.RealTimeMS - LastCheck;
	}
	return Engine.Time.CurrentTimeMS - LastCheck;	
}

GTimer::GTimer()
{
	Delay = 1000;
	RealTime = false;
	ClassID = GTIMER_CLASSID;

	SetLastCheck();	
}

void GTimer::Update()
{
	if (GetDiff() >= Delay)
	{
		Elapsed.call();
		SetLastCheck();
	}
}