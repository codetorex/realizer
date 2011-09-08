#ifndef CTIMING_H
#define CTIMING_H

#include "realizertypes.h"
#include "ttimedriver.h"

class CTiming
{
public:
	dword CurrentTimeMS;
	float CurrentTime;
	float TimeDiff;
	float TimeScale;

	float RealTime;
	float RealDiff;
	dword RealTimeMS;
	
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
		RealTime = 0;
		MeasureTime();
	}

	void TimePassed()
	{
		double MeasureDiff = (double)(TimeDriver::HighPrecision() - MeasureStart);
		float MeasureFrac = (float)(MeasureDiff / MeasureFreq);

		RealDiff = MeasureFrac;
		RealTime += MeasureFrac;
		RealTimeMS = (dword)(RealTime * 1000.0f);

		TimeDiff = MeasureFrac * TimeScale;
		CurrentTime += TimeDiff;
		CurrentTimeMS = (dword)(CurrentTime * 1000.0f);

		MeasureTime();
	}

};

#endif