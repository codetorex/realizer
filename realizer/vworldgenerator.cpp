#include "stdafx.h"
#include "vworldgenerator.h"

#include "tgraphics.h"
#include "mcircle.h"

void VMapView::Visualize(TBitmap* bmp)
{
	TBitmapGraphics bg(bmp);
	bg.Clear(TColors::Black);

	TPen massPen( TColors::LightGreen );

	TArrayEnumerator< VLand* > le(View->Lands);
	while(le.MoveNext())
	{
		Circle2* c = &le.Current->Mass;

		Vector2i cp = GetPositionFromRatio(c->x,c->y);
		Vector2 right = c->PointOnCircle(0.0f);
		int rad = GetXFromRatio(right.x) - cp.x;
		/*cp.x -= rad;
		cp.y -= rad;
		rad *= 2;*/

		bg.SetPixel(cp.x,cp.y, TColors::Lime);

		//bg.DrawEllipse(massPen, cp.x, cp.y,rad,rad);
	}
}

void VMapGenerator::GenerateLands()
{
	srand(31);
	RandomGenerator = new MRandom();
	for (int i=0;i< 5;i++)
	{
		VLand* land = new VLand();
		land->Mass.Set(RandomGenerator->NextSingle(),RandomGenerator->NextSingle());
		land->Mass.Radius = RandomGenerator->NextSingle();
		Generation.Lands.Add(land);
	}
}