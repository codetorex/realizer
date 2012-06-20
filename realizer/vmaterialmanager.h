#ifndef VMATERIALMANAGER_H
#define VMATERIALMANAGER_H

#include "tstring.h"

class VMaterial;

class VMaterialManager: TArray< VMaterial* >
{
public:
	VMaterial* GetMaterial(const TString& name);
};

#endif