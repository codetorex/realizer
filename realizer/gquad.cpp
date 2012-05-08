#include "stdafx.h"
#include "gquad.h"
#include "cengine.h"


void GScalableQuadParted::Render( GObject* obj )
{
	float cx,cy,topWidth,leftHeight,rightHeight,bottomWidth,centerWidth,centerHeight;
	cx = (float)obj->ScreenRegion.X;
	cy = (float)obj->ScreenRegion.Y;
	topWidth = (float)(obj->Width - TopLeft.Width - TopRight.Width);
	bottomWidth = (float)(obj->Width - BottomLeft.Width - BottomRight.Width);
	leftHeight = (float)(obj->Height - TopLeft.Height - BottomLeft.Height);
	rightHeight = (float)(obj->Height - TopRight.Height - BottomRight.Height);
	centerWidth = (float)(obj->Width - Left.Width - Right.Width);
	centerHeight = (float)(obj->Height - Top.Height - Bottom.Height);


	TopLeft.Draw(cx,cy);
	cx += TopLeft.Width;
	Top.DrawPart(cx,cy,topWidth,(float)Top.Height);
	cx += topWidth;
	TopRight.Draw(cx,cy);
	cx = (float)obj->ScreenRegion.X;
	cy += Top.Height;
	Left.DrawPart(cx,cy,(float)Left.Width,leftHeight);
	cy += leftHeight;
	BottomLeft.Draw(cx,cy);
	cx += BottomLeft.Width;
	Bottom.DrawPart(cx,cy,bottomWidth,(float)Bottom.Height);
	cx += bottomWidth;
	BottomRight.Draw(cx,cy);
	cx += BottomRight.Width - Right.Width;
	cy -= rightHeight;
	Right.DrawPart(cx,cy,(float)Right.Width,rightHeight);
	cx -= centerWidth;
	Center.DrawScaled(cx,cy,centerWidth,centerHeight,CenterColor);

	Engine.Draw.Flush();
}

void GScalableQuadParted::SetObjectRegion( GObject* obj )
{
	obj->ObjectRegion.SetRectangle(Left.Width,Top.Height,GetWidthSpace(obj->Width), GetHeightSpace(obj->Height));
}

void GScalableQuad::Initialize( TRange* textureRange, const TRegion& rect )
{
	TextureCoords[0] = textureRange->GetXRatio((float)rect.X);
	TextureCoords[1] = textureRange->GetXRatio((float)(rect.Left + LeftMargin));
	TextureCoords[2] = textureRange->GetXRatio((float)(rect.Right - RightMargin));
	TextureCoords[3] = textureRange->GetXRatio((float)rect.Right);

	TextureCoords[4] = textureRange->GetYRatio((float)(rect.Top + TopMargin));
	TextureCoords[5] = textureRange->GetYRatio((float)(rect.Bottom - BottomMargin));
	TextureCoords[6] = textureRange->GetYRatio((float)rect.Bottom);
	TextureCoords[7] = textureRange->GetYRatio((float)rect.Top);
}

void GScalableQuad::Initialize( const GScalableQuad& other )
{
	for (int i=0;i<8;i++)
	{
		TextureCoords[i] = other.TextureCoords[i];
	}
	LeftMargin = other.LeftMargin;
	RightMargin = other.RightMargin;
	TopMargin = other.TopMargin;
	BottomMargin = other.BottomMargin;
}

void GScalableQuad::Render( GObject* obj )
{
	TRegion tmpRegion;
	tmpRegion.SetFrom(&obj->ScreenRegion);
	Render(&tmpRegion);

	/*Engine.Draw.PreTranslate((float)obj->ScreenRegion.X,(float)obj->ScreenRegion.Y,0.0f);

	float q1,q2;
	q1 = (float)(obj->Width - RightMargin);
	q2 = (float)(obj->Height - BottomMargin);

	Engine.Draw.Add2DQuadColor1Tex(0,0,(float)LeftMargin,(float)TopMargin,TextureCoords[0],TextureCoords[7],TextureCoords[1],TextureCoords[4]); // sol ust
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,0,q1,(float)TopMargin,TextureCoords[1],TextureCoords[7],TextureCoords[2],TextureCoords[4]); // ust orta
	Engine.Draw.Add2DQuadColor1Tex(q1,0,(float)obj->Width,(float)TopMargin,TextureCoords[2],TextureCoords[7],TextureCoords[3],TextureCoords[4]); // sag ust

	Engine.Draw.Add2DQuadColor1Tex(0,(float)TopMargin,(float)LeftMargin,q2,TextureCoords[0],TextureCoords[4],TextureCoords[1],TextureCoords[5]); // sol
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,(float)TopMargin,q1,q2,TextureCoords[1],TextureCoords[4],TextureCoords[2],TextureCoords[5]);// orta
	Engine.Draw.Add2DQuadColor1Tex(q1,(float)TopMargin,(float)obj->Width,q2,TextureCoords[2],TextureCoords[4],TextureCoords[3],TextureCoords[5]); // sag

	Engine.Draw.Add2DQuadColor1Tex(0,q2,(float)LeftMargin,(float)obj->Height,TextureCoords[0],TextureCoords[5],TextureCoords[1],TextureCoords[6]); // sol alt
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,q2,q1,(float)obj->Height,TextureCoords[1],TextureCoords[5],TextureCoords[2],TextureCoords[6]); // orta alt
	Engine.Draw.Add2DQuadColor1Tex(q1,q2,(float)obj->Width,(float)obj->Height,TextureCoords[2],TextureCoords[5],TextureCoords[3],TextureCoords[6]); // sag alt
	
	Engine.Draw.PreTranslate();
	Engine.Draw.Flush();*/
}

void GScalableQuad::Render( TRegion* region )
{
	if (region->Width <= LeftMargin)
	{
		RenderLeftOnly(region);
		return;
	}

	Engine.Draw.PreTranslate((float)region->X,(float)region->Y,0.0f);

	float q1,q2;
	q1 = (float)(region->Width - RightMargin);
	q2 = (float)(region->Height - BottomMargin);

	Engine.Draw.Add2DQuadColor1Tex(0,0,(float)LeftMargin,(float)TopMargin,TextureCoords[0],TextureCoords[7],TextureCoords[1],TextureCoords[4]); // sol ust
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,0,q1,(float)TopMargin,TextureCoords[1],TextureCoords[7],TextureCoords[2],TextureCoords[4]); // ust orta
	Engine.Draw.Add2DQuadColor1Tex(q1,0,(float)region->Width,(float)TopMargin,TextureCoords[2],TextureCoords[7],TextureCoords[3],TextureCoords[4]); // sag ust

	Engine.Draw.Add2DQuadColor1Tex(0,(float)TopMargin,(float)LeftMargin,q2,TextureCoords[0],TextureCoords[4],TextureCoords[1],TextureCoords[5]); // sol
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,(float)TopMargin,q1,q2,TextureCoords[1],TextureCoords[4],TextureCoords[2],TextureCoords[5]);// orta
	Engine.Draw.Add2DQuadColor1Tex(q1,(float)TopMargin,(float)region->Width,q2,TextureCoords[2],TextureCoords[4],TextureCoords[3],TextureCoords[5]); // sag

	Engine.Draw.Add2DQuadColor1Tex(0,q2,(float)LeftMargin,(float)region->Height,TextureCoords[0],TextureCoords[5],TextureCoords[1],TextureCoords[6]); // sol alt
	Engine.Draw.Add2DQuadColor1Tex((float)LeftMargin,q2,q1,(float)region->Height,TextureCoords[1],TextureCoords[5],TextureCoords[2],TextureCoords[6]); // orta alt
	Engine.Draw.Add2DQuadColor1Tex(q1,q2,(float)region->Width,(float)region->Height,TextureCoords[2],TextureCoords[5],TextureCoords[3],TextureCoords[6]); // sag alt

	Engine.Draw.PreTranslate();
	Engine.Draw.Flush();
}

void GScalableQuad::RenderLeftOnly( TRegion* region )
{
	Engine.Draw.PreTranslate((float)region->X,(float)region->Y,0.0f);
	float q = (float)(region->Height - BottomMargin);

	Engine.Draw.Add2DQuadColor1Tex(0,0,(float)LeftMargin,(float)TopMargin,TextureCoords[0],TextureCoords[7],TextureCoords[1],TextureCoords[4]); // sol ust
	Engine.Draw.Add2DQuadColor1Tex(0,(float)TopMargin,(float)LeftMargin,q,TextureCoords[0],TextureCoords[4],TextureCoords[1],TextureCoords[5]); // sol
	Engine.Draw.Add2DQuadColor1Tex(0,q,(float)LeftMargin,(float)region->Height,TextureCoords[0],TextureCoords[5],TextureCoords[1],TextureCoords[6]); // sol alt

	Engine.Draw.PreTranslate();
	Engine.Draw.Flush();
}

void GScalableQuad::SetObjectRegion( GObject* obj )
{
	obj->ObjectRegion.SetRectangle(LeftMargin,RightMargin,GetWidthSpace(obj->Width), GetHeightSpace(obj->Height));
}

void GScalableQuad::Swap( GScalableQuad& other )
{
	GScalableQuad tmp;
	tmp.Initialize(*this);
	Initialize(other);
	other.Initialize(tmp);
}