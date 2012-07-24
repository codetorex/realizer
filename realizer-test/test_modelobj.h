#ifndef TEST_MODELOBJ_H
#define TEST_MODELOBJ_H

#include "rtestsuite.h"
#include "vcamera.h"

class VTexture;
class VModel;
class MMatrix4x4;
class D3DXMATRIX;

class TFileStream;

class RTestModelObj: public RTestScene
{
public:
	RTestModelObj()
	{
		SceneName = "OBJ Loading test";
	}

	void ExportMatrix(const TString& exportName, MMatrix4x4& mat );
	void ExportMatrix(const TString& exportName, D3DXMATRIX& mat);
	

	TFileStream* fs;

	VModel* LoadedObj;
	VCamera Camera;
	VTexture* TestTexture;

	void Initialize();

	void Render();

};

extern RTestModelObj TestModelObj;

#endif