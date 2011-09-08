#ifndef GTIMER_H
#define GTIMER_H

#include "gobject.h"

class GTimer: public GObject // I don't know why I derived this from GObject?
{
private:

public:
	bool RealTime;
	dword Delay;
	dword LastCheck;

	GTimer();

	event<NoArgEvent> Elapsed;

	void Render() {};
	void Update();

	void SetLastCheck();
	dword GetDiff();
};

#endif