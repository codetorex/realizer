#ifndef VMATERIAL_H
#define VMATERIAL_H

#include "realizertypes.h"
#include "tstring.h"

class VMaterial
{
public:
	TString Name;

	VPixelShader* MaterialShader;

	/// Shader variables and material related input should stored here.
};

#endif