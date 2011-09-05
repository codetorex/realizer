#ifndef CTIMING_H
#define CTIMING_H

#include "realizertypes.h"
#include "ttimedriver.h"

class CTiming
{
public:
	float CurrentTime;
	float TimeDiff;
	float TimeScale;
	
	qword MeasureStart;
	double MeasureFreq;

	dword TickCount;

	inline void MeasureTime()
	{
		MeasureStart = TimeDriver::HighPrecision();
		MeasureFreq = (double)TimeDriver::HighFrequency();
		TickCount = TimeDriver::TickCount();
	}

	void Begining()
	{
		CurrentTime = 0;
		TimeDiff = 0;
		TimeScale = 1.0f;
//		LoopCount = 0;
		MeasureTime();
	}

	void TimePassed()
	{
		double MeasureDiff = (double)(TimeDriver::HighPrecision() - MeasureStart);
		float MeasureFrac = (float)(MeasureDiff / MeasureFreq);

		TimeDiff = MeasureFrac * TimeScale;
		CurrentTime += TimeDiff;
		MeasureTime();
	}

};

#endif