#ifndef REALIZERVERSION_H
#define REALIZERVERSION_H

#include "raiseversion.h"

#define REALIZER_BUILD					445
#define REALIZER_BUILD_STRING			"445"

#define REALIZER_PHASE					&TVersionPhase::PreAlpha

#define REALIZERMODULE					TModule("Realizer",TVersion(REALIZER_BUILD,REALIZER_PHASE))

#define USESREALIZER					Application.Modules.Add( new REALIZERMODULE )

#endif
