#ifndef GPROXY_H
#define GPROXY_H

#include "gobject.h"

/**
 * Proxy GUI object.
 * That just forwards messages to activeObject which is easily changeable view control.
 */
class GProxy: public GObject
{
public:
	GProxy();

	GObject* ActiveObject;

	inline void SetObject(GObject* newObject)
	{
		ActiveObject = newObject;
	}

	void Render();
	void Update();
	void Layout();
	GObject* FindObject();
};

#endif