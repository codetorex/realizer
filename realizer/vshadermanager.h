#ifndef VSHADERMANAGER_H
#define VSHADERMANAGER_H

#include "realizertypes.h"
#include "tstring.h"
#include "tarray.h"
#include "vshader.h"

class VPixelShader;
class VVertexShader;
class VShaderPipeline;

class VShaderManager: public TArray< VShader* >
{
public:
	TArray<VShaderPipeline*> Pipelines;

	VShader* GetShader(const TString& name, EShaderTypes Type);

	VVertexShader* GetVertexShader(const TString& name);
	VPixelShader* GetPixelShader(const TString& name);

	VShaderPipeline* GetPipeline(VPixelShader* ps, VVertexShader* vs, VGeometryShader* gs);
	VShaderPipeline* GetPipeline(VPixelShader* ps, VVertexShader* vs);
	VShaderPipeline* GetPipeline(const TString& name);

	VShaderPipeline* CreatePipeline(const TString& name, VPixelShader* ps, VVertexShader* vs, VGeometryShader* gs = 0);
};

#endif