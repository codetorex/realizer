#include "stdafx.h"
#include "test_enginecolor.h"
#include "cengine.h"
#include "vtexture.h"
#include "mmathcolor.h"
#include "rtestsuite.h"
#include "gfont.h"
#include "tconvert.h"
#include "tcompositebuffer.h"

RTestColor TestColor;

void RTestColor::AddTest( const TColor32& ColorValue, const TString& COlorName )
{
	VColorTest* nTest = new VColorTest();
	nTest->ColorName = COlorName;
	nTest->ColorValue = ColorValue;

	Tests.Add(nTest);
}

void RTestColor::Initialize()
{
	AddTest(TColors::TestColor, "Test Color");
	AddTest(TColors::Red, "Red");
	AddTest(TColors::Green, "Green");
	AddTest(TColors::Blue, "Blue");
}

void RTestColor::Render()
{
	Engine.Renderer.Enter2D();

	x = y = 100;

	int texSize = 200;
	int padding = 10;
	int margin = 60;

	TArrayEnumerator<VColorTest*> te(Tests);
	while (te.MoveNext())
	{
		texreg.SetSize(x+padding,y + texSize + (4 * padding),texSize,10);

		Engine.Draw.NoTexture();
		Engine.Draw.DrawRectangle(x,y,texSize + (padding *2),texSize + (padding*2), TColors::White);


		Engine.Draw.DrawQuad(x+padding,y+padding,x+padding + texSize ,y+padding +texSize,0,0,0,0, te.Current->ColorValue);

		Engine.Draw.Flush();

		TestSuite.WriteText(te.Current->ColorName,texreg);

		x += texSize + (2 * padding) + margin;
	}


	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}
