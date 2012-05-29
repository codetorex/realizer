#ifndef GLAYOUT_H
#define GLAYOUT_H

#include "mrectangle.h"

class GObject;

class GAlign
{
public:
	void Align( IRectangle& srcRect, const IRectangle& dstRect, Alignment alignment);
};

/**
 * Base layout algorithm for laying out strategy of containers.
 */
class GLayout
{
public:
	static GLayout Instance;

	virtual IRectangle Layout(GObject* container, bool measureOnly);
};

class GLayoutHorizontalOverflow: public GLayout
{
public:
	static GLayoutHorizontalOverflow Instance;

	virtual IRectangle Layout(GObject* container, bool measureOnly);
};

class GLayoutVertical: public GLayout
{
public:
	static GLayoutVertical Instance;

	virtual IRectangle Layout(GObject* container, bool measureOnly);
};

#endif