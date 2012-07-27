#ifndef TEST_MODELOBJ_H
#define TEST_MODELOBJ_H

#include "rtestsuite.h"
#include "vcamera.h"

class VTexture;
class VModel;
class MMatrix4x4;

class RTestModelObj: public RTestScene
{
public:
	RTestModelObj()
	{
		SceneName = "OBJ Loading test";
	}

	VModel* LoadedObj;
	VCamera Camera;
	VTexture* TestTexture;

	void Initialize();

	void Render();

};

extern RTestModelObj TestModelObj;

#endif