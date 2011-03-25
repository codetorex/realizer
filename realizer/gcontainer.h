#ifndef GCONTAINER_H
#define GCONTAINER_H

#include "gobject.h"


/*class GContainer: public GObject, public TList< GObject >
{
public:
	TRegion Padding;



	void DeliverMouseMove(int x,int y)
	{

	}

	


	//virtual void MouseEnter() {};
	//virtual void MouseExit() {};
	//virtual void MouseWheel(int x,int y, int delta) {};
	virtual void MouseDown(int x,int y, int button)
	{
		GObject* curObj = LastItem;
		int i = ItemCount;
		while(i--)
		{
			if (curObj->IsInside(x,y))
			{
				if (Master->Focused != curObj)
				{
					Master->Focused->LostFocus();
					Master->Focused = curObj;
				}
				curObj->MouseDown(x-curObj->X,y-curObj->Y,button);
				break;
			}
			curObj = curObj->PrevItem;
		}
	}

	virtual void MouseUp(int x,int y,int button)
	{
		GObject* curObj = LastItem;
		int i = ItemCount;
		while(i--)
		{
			if (curObj->IsInside(x,y))
			{
				curObj->MouseUp(x-curObj->X,y-curObj->Y,button);
				break;
			}
			curObj = curObj->PrevItem;
		}
	}

	virtual void MouseMove(int x,int y) 
	{
		GObject* curObj = LastItem;
		int i = ItemCount;
		while(i--)
		{
			if (curObj->IsInside(x,y))
			{
				if (!curObj->MouseInside)
				{
					curObj->MouseEnter();
					curObj->MouseInside = true;
				}
				curObj->MouseMove(x - curObj->X, y-curObj->Y);
			}
			else
			{
				if (curObj->MouseInside)
				{
					curObj->MouseExit();
					curObj->MouseInside = false;
				}
			}
			curObj = curObj->PrevItem;
		}
	};

	//virtual void GotFocus() {};
	//virtual void LostFocus() {};
	//virtual void KeyPress(dword keyID) {};
	//virtual void KeyDown(dword keyID) {};
	//virtual void KeyUp(dword keyID) {};

	virtual void Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Layout(); // done when resize happens


	void DebugRender();
};*/

#endif