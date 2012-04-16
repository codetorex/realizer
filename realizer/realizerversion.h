#ifndef REALIZERVERSION_H
#define REALIZERVERSION_H

#define REALIZER_BUILD					181
#define REALIZER_BUILD_STRING			"181"

#define REALIZER_PHASE					&TVersionPhase::PreAlpha

#define REALIZERMODULE					TModule("Realizer",TVersion(REALIZER_BUILD,REALIZER_PHASE))

#define USESREALIZER					Application.Modules.Add( new REALIZERMODULE )

#endif
