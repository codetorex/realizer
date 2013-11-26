#ifndef VSHADERMANAGER_H
#define VSHADERMANAGER_H

#include "realizertypes.h"
#include "tstring.h"
#include "tarray.h"
#include "vshader.h"

class VPixelShader;
class VVertexShader;
class VShaderPipeline;

class VShaderManager: public Array< VShader* >
{
public:
	Array<VShaderPipeline*> Pipelines;

	VShader* GetShader(const String& name, EShaderTypes Type);

	VVertexShader* GetVertexShader(const String& name);
	VPixelShader* GetPixelShader(const String& name);

	VShaderPipeline* GetPipeline(VPixelShader* ps, VVertexShader* vs, VGeometryShader* gs);
	VShaderPipeline* GetPipeline(VPixelShader* ps, VVertexShader* vs);
	VShaderPipeline* GetPipeline(const String& name);

	VShaderPipeline* CreatePipeline(const String& name, VPixelShader* ps, VVertexShader* vs, VGeometryShader* gs = 0);
};

#endif