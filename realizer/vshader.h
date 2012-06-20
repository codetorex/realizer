#ifndef VSHADER_H
#define VSHADER_H

#include "tstring.h"

class TFieldInfo;

enum EShaderTypes
{
	ST_PIXEL,
	ST_VERTEX,
	ST_GEOMETRY,
};

enum EShaderLevel
{
	SMR_10,
	SMR_20,
	SMR_30,
	SMR_40,
	SMR_50,
};

enum EQuality
{
	QR_LOW,
	QR_MEDIUM,
	QR_HIGH,
	QR_EXPERIMENTAL,
};

enum ESourceLanguage
{
	SL_HLSL,
	SL_GLSL,
	SL_Cg,
};

/**
 * Abstract class that holds common shader information
 */
class VShader
{
public:
	TString SourcePath;
	ESourceLanguage SourceLanguage;
	EShaderTypes Type;
	EShaderLevel Level;

	/// How much raw computational power this shader needs?
	ui32 ComplexityScore;

	/// These are not necessary but could be useful I guess
	TArray< TFieldInfo* > Uniforms;
	TArray< TFieldInfo* > Inputs;

	virtual void EnableShader() 
	{
		throw 0;
	}

	virtual void CalculateComplexity() 
	{
		throw 0;
	}
};

class VVertexShader: public VShader
{
public:

};

class VPixelShader: public VShader
{
public:

};

class VGeometryShader: public VShader
{
public:

};

/// TODO: we should implement in somewhere multiple shader levels. like low quality shaders for lowend gpus and high quality shaders for high end gpus


class VShaderPipeline
{
public:
	TString Name;

	VVertexShader* VertexShader;
	VPixelShader* PixelShader;
	VGeometryShader* GeometryShader;

	VShaderPipeline(VVertexShader* vs, VPixelShader* ps, VGeometryShader* gs = 0)
	{
		VertexShader = vs;
		PixelShader = ps;
		GeometryShader = gs;
	}


	void EnablePipeline()
	{
		VertexShader->EnableShader();
		PixelShader->EnableShader();
		if (GeometryShader)
		{
			GeometryShader->EnableShader();
		}
	}
};



#endif