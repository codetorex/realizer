#ifndef GPROGRESSBAR_H
#define GPROGRESSBAR_H

#include "gobject.h"

class GProgressBar: public GObject
{
public:
	GProgressBar();

	int Value;
	int Minimum;
	int Maximum;

	bool ShowPercent;
	Alignment PercentAlign;

	void Render();
	void Update();

	inline int GetPercentWidth()
	{
		int totalValues = Maximum - Minimum;
		int adjValue = Value - Minimum;
		return (adjValue * Width) / totalValues;
	}

	inline int GetPercentage()
	{
		int totalValues = Maximum - Minimum;
		int adjValue = Value - Minimum;
		return (adjValue * 100) / totalValues;
	}
};


#endif