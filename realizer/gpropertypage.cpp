#include "stdafx.h"
#include "gpropertypage.h"
#include "gfont.h"
#include "ttypemanager.h"

GPropertyPage::GPropertyPage()
{
	ClassID = GPROPERTYPAGE_CLASSID;
}

void GPropertyPage::Render()
{
	Skin->RenderSunkEdge(this);

	IRectangle drect = Content;
	drect += DrawRegion;

	TArrayEnumerator< GPropertyRow* > p(Properties);
	while(p.MoveNext())
	{
		GPropertyRow* cur = p.Current;

		int oldY = drect.Y;
		int halfX = drect.Width / 2;

		drect.Y += 2;

		Font->Render(cur->Field->Name,drect.X+2,drect.Y,ForeColor);
		
		if (cur->Field->FieldType == Types.ST_String)
		{
			TString* str = (TString*)cur->Value;
			Font->Render(*str,drect.X + halfX + 2,drect.Y, ForeColor);
		}
		
		//Font->Render(
		drect.Y += Font->Height + 2;

		Engine.Draw.NoTexture();
		Engine.Draw.DrawLine(drect.X + halfX,drect.Y,drect.X + halfX,oldY, ForeColor);
		Engine.Draw.DrawLine(drect.X,drect.Y,drect.X + drect.Width,drect.Y, ForeColor);

		
	}
}

void GPropertyPage::Initialize()
{
	HBar.SetRectangle(0,0,100,100);
	HBar.Dock = DCK_RIGHT;
	AddChild(&HBar);
}

void GPropertyPage::Layout()
{
	OwnObject(&HBar); // update its skin etc

	Skin->LayoutSunkEdge(this);

	this->GObject::Layout();
}

void GPropertyPage::SetPropertyObject( void* newObject, TType* objType )
{
	ObjType = objType;
	PropertyObject = newObject;

	LoadClass(newObject,objType);
}

void GPropertyPage::LoadClass( void* obj, TType* typ )
{
	Properties.DeletePointers();

	if (!typ->IsClass)
	{
		throw Exception("Type is not class");
	}

	if (typ->BaseClass)
	{
		LoadClass(obj,typ->BaseClass);
	}

	TArrayEnumerator< TFieldInfo* > fld(typ->Fields);
	while(fld.MoveNext())
	{
		TFieldInfo* cur = fld.Current;
		
		GPropertyRow* newRow = new GPropertyRow();
		newRow->Field = cur;
		newRow->Value = cur->GetPointer(obj);

		Properties.Add(newRow);
	}
}

