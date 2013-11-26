#ifndef TEST_ENGINECOLOR_H
#define TEST_ENGINECOLOR_H

#include "rtestsuite.h"
#include "tstringbuilder.h"
#include "mregion2.h"





class VColorTest
{
public:
	TColor32 ColorValue;
	String ColorName;
};

class RTestColor: public RTestScene
{
private:
	TStringBuilderStack<512> sb;
	int x;
	int y;
	IRegion texreg;

public:
	RTestColor()
	{
		SceneName = "Engine color test";
	}

	Array< VColorTest* > Tests;

	//void AddTest( VColorTest* tex );

	void AddTest( const TColor32& ColorValue, const String& COlorName);

	void Initialize();

	void Render();

};

extern RTestColor TestColor;


#endif