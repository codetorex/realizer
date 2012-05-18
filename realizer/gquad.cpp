#include "stdafx.h"
#include "gquad.h"
#include "cengine.h"


void GScalableQuadParted::Render( GObject* obj )
{
	float cx,cy,topWidth,leftHeight,rightHeight,bottomWidth,centerWidth,centerHeight;
	cx = (float)obj->DrawRegion.X();
	cy = (float)obj->DrawRegion.Y();
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
	cx = (float)obj->DrawRegion.X();
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

void GScalableQuadParted::SetObjectContent( GObject* obj )
{
	obj->Border.SetPadding(Left.Width,Top.Height,Right.Width,Bottom.Height);
	obj->UpdateContent();
}

void GScalableQuad::Initialize( const ISize& textureRange, const IRectangle& rect )
{
	TextureCoords[0] = textureRange.WidthRatio((float)rect.X);
	TextureCoords[1] = textureRange.WidthRatio((float)(rect.X + Left));
	TextureCoords[2] = textureRange.WidthRatio((float)(rect.Right() - Right));
	TextureCoords[3] = textureRange.WidthRatio((float)rect.Right());

	TextureCoords[4] = textureRange.HeightRatio((float)(rect.Y + Top));
	TextureCoords[5] = textureRange.HeightRatio((float)(rect.Bottom() - Bottom));
	TextureCoords[6] = textureRange.HeightRatio((float)rect.Bottom());
	TextureCoords[7] = textureRange.HeightRatio((float)rect.Y);
}

void GScalableQuad::Initialize( const GScalableQuad& other )
{
	for (int i=0;i<8;i++)
	{
		TextureCoords[i] = other.TextureCoords[i];
	}
	Left = other.Left;
	Right = other.Right;
	Top = other.Top;
	Bottom = other.Bottom;
}


void GScalableQuad::Render( GObject* obj )
{
	Render(obj->DrawRegion);
}

void GScalableQuad::Render( const IRectangle& rect )
{
	if (rect.Width <= Left)
	{
		RenderLeftOnly(rect);
		return;
	}

	Engine.Draw.PreTranslate((float)rect.X,(float)rect.Y,0.0f);

	float q1,q2;
	q1 = (float)(rect.Width - Right);
	q2 = (float)(rect.Height - Bottom);

	Engine.Draw.Add2DQuadColor1Tex(0,0,(float)Left,(float)Top,TextureCoords[0],TextureCoords[7],TextureCoords[1],TextureCoords[4]); // sol ust
	Engine.Draw.Add2DQuadColor1Tex((float)Left,0,q1,(float)Top,TextureCoords[1],TextureCoords[7],TextureCoords[2],TextureCoords[4]); // ust orta
	Engine.Draw.Add2DQuadColor1Tex(q1,0,(float)rect.Width,(float)Top,TextureCoords[2],TextureCoords[7],TextureCoords[3],TextureCoords[4]); // sag ust

	Engine.Draw.Add2DQuadColor1Tex(0,(float)Top,(float)Left,q2,TextureCoords[0],TextureCoords[4],TextureCoords[1],TextureCoords[5]); // sol
	Engine.Draw.Add2DQuadColor1Tex((float)Left,(float)Top,q1,q2,TextureCoords[1],TextureCoords[4],TextureCoords[2],TextureCoords[5]);// orta
	Engine.Draw.Add2DQuadColor1Tex(q1,(float)Top,(float)rect.Width,q2,TextureCoords[2],TextureCoords[4],TextureCoords[3],TextureCoords[5]); // sag

	Engine.Draw.Add2DQuadColor1Tex(0,q2,(float)Left,(float)rect.Height,TextureCoords[0],TextureCoords[5],TextureCoords[1],TextureCoords[6]); // sol alt
	Engine.Draw.Add2DQuadColor1Tex((float)Left,q2,q1,(float)rect.Height,TextureCoords[1],TextureCoords[5],TextureCoords[2],TextureCoords[6]); // orta alt
	Engine.Draw.Add2DQuadColor1Tex(q1,q2,(float)rect.Width,(float)rect.Height,TextureCoords[2],TextureCoords[5],TextureCoords[3],TextureCoords[6]); // sag alt

	Engine.Draw.PreTranslate();
	Engine.Draw.Flush();
}

void GScalableQuad::RenderLeftOnly( const IRectangle& rect )
{
	Engine.Draw.PreTranslate((float)rect.X,(float)rect.Y,0.0f);
	float q = (float)(rect.Height - Bottom);

	Engine.Draw.Add2DQuadColor1Tex(0,0,(float)Left,(float)Top,TextureCoords[0],TextureCoords[7],TextureCoords[1],TextureCoords[4]); // sol ust
	Engine.Draw.Add2DQuadColor1Tex(0,(float)Top,(float)Left,q,TextureCoords[0],TextureCoords[4],TextureCoords[1],TextureCoords[5]); // sol
	Engine.Draw.Add2DQuadColor1Tex(0,q,(float)Left,(float)rect.Height,TextureCoords[0],TextureCoords[5],TextureCoords[1],TextureCoords[6]); // sol alt

	Engine.Draw.PreTranslate();
	Engine.Draw.Flush();
}

void GScalableQuad::SetObjectRegion( GObject* obj )
{
	obj->Border.SetPadding(Left,Top,Right,Bottom);
	obj->UpdateContent();
	//obj->Content.SetRectangle(LeftMargin,RightMargin,GetWidthSpace(obj->Width), GetHeightSpace(obj->Height));
}

void GScalableQuad::Swap( GScalableQuad& other )
{
	GScalableQuad tmp;
	tmp.Initialize(*this);
	Initialize(other);
	other.Initialize(tmp);
}