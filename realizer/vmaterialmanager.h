#ifndef VMATERIALMANAGER_H
#define VMATERIALMANAGER_H

#include "tstring.h"

class VMaterial;

class VMaterialManager: Array< VMaterial* >
{
public:
	VMaterial* GetMaterial(const String& name);
};

#endif