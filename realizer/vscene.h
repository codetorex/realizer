#ifndef VSCENE_H
#define VSCENE_H

#include "tstring.h"

class VScene
{
public:
	string SceneName;

	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
};

#endif