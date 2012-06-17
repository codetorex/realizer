#ifndef GPROPERTYPAGE_H
#define GPROPERTYPAGE_H

#include "gobject.h"
#include "gscrollbar.h"
#include "ttype.h"

class GPropertyRow
{
public:
	TFieldInfo* Field;
	void* Value;
};

class GPropertyPage: public GObject
{
private:
	GScrollBar HBar;
	void* PropertyObject;
	TType* ObjType;

	TArray<GPropertyRow*> Properties;

	void LoadClass(void* obj, TType* typ);

public:
	GPropertyPage();

	void SetPropertyObject(void* newObject, TType* objType);
	
	inline void SetNoObject()
	{
		PropertyObject = 0;
		ObjType = 0;
	}
	

	void Initialize();
	void Layout();
	void Render();

};


#endif