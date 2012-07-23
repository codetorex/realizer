#ifndef VMODELOBJ_H
#define VMODELOBJ_H

#include "vmodel.h"
#include "mvector3.h"

class TStream;
class VOBJFace;
class VOBJVertex;

class VModelReader
{
public:
	virtual void ReadModel(VModel* mdl, TStream* src) = 0;
};

class VModelVertex
{
public:
	ui32 Index;

	Vector3 Position;
	Vector3 Normal;
	Vector3 TexCoord;
	// Should be extensible?
};

class VModelFace
{
public:
	VModelVertex* V1;
	VModelVertex* V2;
	VModelVertex* V3;
};

/**
 * Wavefront obj model reader
 */
class VModelOBJ: public VModelReader
{
protected:
	TArray<Vector3> Vertices;
	TArray<Vector3> Normals;
	TArray<Vector3> TextureCoords;

	void Seperate(const TString& input, int& cmdLength, int& prmStart);
	void ParseVector3(const TString& input, int start, Vector3& vec);
	void ParseFace(const TString& input, int start, VOBJFace& face);
	void ParseFaceVertex(const TString& input, VOBJVertex& faceVertex);

public:
	VModelOBJ()
	{
		ReadAsTriangleList = true;
		ReadAsIndexed = false; // not implemented anyway...
	}

	bool ReadAsIndexed;
	bool ReadAsTriangleList;

	void ReadModel(VModel* mdl, TStream* src);
};

#endif