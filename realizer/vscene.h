#ifndef VSCENE_H
#define VSCENE_H

#include "tstring.h"

enum VSceneFlags
{
	SF_ALWAYSONTOP = 1,

};

class VScene
{
public:
	string		SceneName;
	bool		Active;
	ui32		Flags;

	inline bool IsAlwaysOnTop()
	{
		return (Flags & SF_ALWAYSONTOP) != 0;
	}

	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
};

#endif