#ifndef VNODECANVAS_H
#define VNODECANVAS_H

#include "vnode.h"
#include "mvector3.h"
#include "tdiagram.h"
#include "tcolor.h"
#include "tarray.h"
#include "genums.h"
#include "tstring.h"
#include "mrectangle.h"

class VNodeCanvasItem
{
public:
	TString		ItemName;
	
	Vector3		Position;
	Vector3		Rotation;
	Vector3		Scale;
	TColor32	Color;
	
	virtual void Render() = 0;
	virtual void Update() = 0;
};


class VNodeCanvasImage;
class VTexture;

class VNodeCanvas: public VNode
{
public:
	
	TArray< VNodeCanvasItem* > Items;

	VNodeCanvas()
	{
		RenderEnabled = true;
		UpdateEnabled = true;

		ClassID = VCANVAS_CLASSID;
	}

	void Render();

	void Update()
	{
		int i=Items.Count;
		while(i--)
		{
			Items.Item[i]->Update();
		}
	}

	VNodeCanvasImage* DrawImage(VTexture* Texture, int srcX,int srcY, int srcWidth,int srcHeight,int dstX,int dstY,int dstWidth, int dstHeight, Alignment align );

	VNodeCanvasImage* DrawImage(VTexture* Texture, int dstX,int dstY, float scale, Alignment align );

};

#endif