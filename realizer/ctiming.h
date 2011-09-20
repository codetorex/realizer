#ifndef CTIMING_H
#define CTIMING_H

#include "realizertypes.h"
#include "ttimedriver.h"

class CTiming
{
public:
	ui32 CurrentTimeMS;
	float CurrentTime;
	float TimeDiff;
	float TimeScale;

	float RealTime;
	float RealDiff;
	ui32 RealTimeMS;
	
	qword MeasureStart;
	double MeasureFreq;

	ui32 TickCount;

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
		RealTimeMS = (ui32)(RealTime * 1000.0f);

		TimeDiff = MeasureFrac * TimeScale;
		CurrentTime += TimeDiff;
		CurrentTimeMS = (ui32)(CurrentTime * 1000.0f);

		MeasureTime();
	}

};

#endif