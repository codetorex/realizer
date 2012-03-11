#ifndef VSCENE_H
#define VSCENE_H

#include "tstring.h"
#include "tflag.h"
#include "tlinkedlist.h"

enum VSceneFlags
{
	SF_UPDATE = 1,
	SF_RENDER = 2,
	SF_ACTIVE = 4,
	SF_ALWAYSONTOP = 8,
};

enum VSceneTypes
{
	ST_UNDEFINED,
	ST_LAYERED,
	ST_NODEBASED,
	ST_BEGIN,
	ST_END,
	ST_2D, // good for gui and hud rendering
	ST_DEBUG2D, // gui for easy debugging facilities
};

class VScene: public TListNode<VScene*>
{
public:
	ui32		ID; // index/identification number for scene
	string		SceneName;
	TFlag32		Flags;
	ui32		Type;

	inline bool IsAlwaysOnTop()
	{
		return (Flags == SF_ALWAYSONTOP);
	}

	virtual void Render()  { Flags -= SF_RENDER; };
	virtual void Update()  { Flags -= SF_UPDATE; };
	virtual void Initialize() {};
	virtual void Finalize() {};
};

#endif