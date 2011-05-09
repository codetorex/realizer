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
		MeasureStart = TTimeDriver::HighPrecision();
		MeasureFreq = (double)TTimeDriver::HighFrequency();
		TickCount = TTimeDriver::TickCount();
	}

	void Begining()
	{
		CurrentTime = 0;
		TimeDiff = 0;
		TimeScale = 1.0f;
		MeasureTime();
	}

	void TimePassed()
	{
		double MeasureDiff = (double)(TTimeDriver::HighPrecision() - MeasureStart);
		float MeasureFrac = (float)(MeasureDiff / MeasureFreq);

		TimeDiff = MeasureFrac * TimeScale;
		CurrentTime += TimeDiff;
		MeasureTime();
	}

};

#endif