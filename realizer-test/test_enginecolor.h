#ifndef TEST_ENGINECOLOR_H
#define TEST_ENGINECOLOR_H

#include "rtestsuite.h"
#include "tstringbuilder.h"
#include "mregion2.h"





class VColorTest
{
public:
	TColor32 ColorValue;
	TString ColorName;
};

class RTestColor: public RTestScene
{
private:
	TStringBuilder sb;
	int x;
	int y;
	IRegion texreg;

public:
	RTestColor()
	{
		SceneName = "Engine color test";
	}

	TArray< VColorTest* > Tests;

	//void AddTest( VColorTest* tex );

	void AddTest( const TColor32& ColorValue, const TString& COlorName);

	void Initialize();

	void Render();

};

extern RTestColor TestColor;


#endif