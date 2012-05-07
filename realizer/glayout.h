#ifndef GLAYOUT_H
#define GLAYOUT_H

#include "trectangle.h"

class GObject;

/**
 * Base layout algorithm for laying out strategy of containers.
 */
class GLayout
{
public:
	static GLayout Instance;

	virtual TRectangle Layout(GObject* container, bool measureOnly);
};

class GLayoutHorizontalOverflow: public GLayout
{
public:
	static GLayoutHorizontalOverflow Instance;

	virtual TRectangle Layout(GObject* container, bool measureOnly);
};

class GLayoutVertical: public GLayout
{
public:
	static GLayoutVertical Instance;

	virtual TRectangle Layout(GObject* container, bool measureOnly);
};

#endif