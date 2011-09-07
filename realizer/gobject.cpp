#include "stdafx.h"
#include "vgui.h"

void GObject::ActivateObject( GObject* obj )
{
	obj->Active = true;
	if (LastItem == obj) return;

	LastItem->Active = false;
	if (FirstItem == obj)
	{
		FirstItem = obj->NextItem;
		FirstItem->PrevItem = 0;

		obj->PrevItem = LastItem;
		obj->NextItem = 0;
		LastItem->NextItem = obj;
		LastItem = obj;
	}
	else
	{
		GObject *Next,*Prev;
		Next = obj->NextItem;
		Prev = obj->PrevItem;

		Prev->NextItem = Next;
		Next->PrevItem = Prev;

		LastItem->NextItem = obj;
		obj->PrevItem = LastItem;
		obj->NextItem = 0;
		LastItem = obj;
	}
}

GObject* GObject::FindObject( int x,int y )
{
	if (!Visible)
	{
		return 0;
	}

	if (!ScreenRegion.IsInside(x,y))
	{
		return 0;
	}

	GObject* curObj = LastItem;
	int i = ItemCount;
	while(i--)
	{
		GObject* found = curObj->FindObject(x , y ); // hmm was tried relative finding like x - X
		if (found) return found;
		curObj = curObj->PrevItem;
	}

	return this;
}

void GObject::SetFocus()
{
	Master->Focused->Focused = false;
	Master->Focused->LostFocus();
	Master->Focused = this;
	Focused = true;
	GotFocus();


}

void GObject::Render()
{
	GObject* curObj = FirstItem;
	int i = ItemCount;
	while(i--)
	{
		if (curObj->Visible)
		{
			curObj->Render();
		}
		curObj = curObj->NextItem;
	}
}

void GObject::Update()
{
	GObject* p = (GObject*)Parent;
	ScreenRegion.SetSize(p->ScreenRegion.X + X,p->ScreenRegion.Y + Y,Width,Height);

	GObject* curObj = FirstItem;
	int i = ItemCount;
	while(i--)
	{
		if (curObj->Visible)
		{
			curObj->Update();
		}
		curObj = curObj->NextItem;
	}
}

/*bool GObject::DeliverMove()
{
	GObject* p = (GObject*)Parent;
	if (p->MouseInside)
	{
		if (ScreenRegion.IsInside(Master->X,Master->Y))
		{
			MouseInside = true;
		}
		else
		{
			if (MouseInside)
			{
				MouseEntered = false;
				MouseExit();
			}
			MouseInside = false;
		}
	}
	else
	{
		if (MouseInside)
		{
			MouseEntered = false;
			MouseExit();
		}
		MouseInside = false;
	}

	bool subResult = false;
	GObject* curObj = LastItem;
	int i = ItemCount;
	while(i--)
	{
		if (curObj->Visible)
		{
			subResult = curObj->DeliverMove();
		}
		curObj = curObj->PrevItem;
	}

	if (!subResult)
	{
		if (MouseInside)
		{
			if (!MouseEntered)
			{
				MouseEntered = true;
				MouseEnter();
			}
			// else?
			MouseMove(Master->X - ScreenRegion.X, Master->Y - ScreenRegion.X);
			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}*/

GObject* GObject::FindObject()
{
	GObject* p = (GObject*)Parent;
	if (p->MouseInside)
	{
		if (ScreenRegion.IsInside(Master->X,Master->Y))
		{
			MouseInside = true;
		}
		else
		{
			if (MouseInside)
			{
				MouseEntered = false;
				MouseExit();
			}
			MouseInside = false;
		}
	}
	else
	{
		if (MouseInside)
		{
			MouseEntered = false;
			MouseExit();
		}
		MouseInside = false;
	}

	GObject* subResult = 0;
	GObject* curObj = LastItem;
	while(curObj)
	{
		if (curObj->Visible)
		{
			subResult = curObj->FindObject();
			if (subResult)
			{
				return subResult;
			}
		}
		curObj = curObj->PrevItem;
	}

	if (!subResult)
	{
		if (MouseInside)
		{
			if (!MouseEntered)
			{
				MouseEntered = true;
				MouseEnter();
			}
			// else?
			return this;
		}
	}

	return 0;
}
