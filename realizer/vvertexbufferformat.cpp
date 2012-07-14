#include "stdafx.h"
#include "vvertexbufferformat.h"
#include "tbitmapformats.h"

VVertexBufferFormats* VertexBufferFormats;

TCompositeFormatFactoryVertex TCompositeFormatFactoryVertex::Instance;

void VVertexBufferFormats::InitializeVertexFormats()
{
	SetPrimitive(PosX,"Position X","PX",tc_float);
	SetPrimitive(PosY,"Position Y","PY",tc_float);
	SetPrimitive(PosZ,"Position Z","PZ",tc_float);
	SetPrimitive(PosRHW,"Position RHW","RHW",tc_float);

	SetPrimitive(NX,"Normal X","NX",tc_float);
	SetPrimitive(NY,"Normal Y","NY",tc_float);
	SetPrimitive(NZ,"Normal Z","NZ",tc_float);

	SetPrimitive(TU,"Texture U","TU",tc_float);
	SetPrimitive(TV,"Texture V","TV",tc_float);

	AddPrimitive(&BitmapFormats->cRed);
	AddPrimitive(&BitmapFormats->cGreen);
	AddPrimitive(&BitmapFormats->cBlue);
	AddPrimitive(&BitmapFormats->cAlpha);

	SetCompositePrimitive(Position,"Position","POS","PXPYPZ");
	SetCompositePrimitive(Normal,"Normal", "NORM","NXNYNZ");
	SetCompositePrimitive(TexCoord0,"Texture 0 Coords", "T0","TUTV");
	SetCompositePrimitive(TexCoord1,"Texture 1 Coords", "T1","TUTV");
	SetCompositePrimitive(TexCoord2,"Texture 2 Coords", "T2","TUTV");
	SetCompositePrimitive(TexCoord3,"Texture 3 Coords", "T3","TUTV");
	SetCompositePrimitive(TexCoord4,"Texture 4 Coords", "T4","TUTV");
	SetCompositePrimitive(TexCoord5,"Texture 5 Coords", "T5","TUTV");
	SetCompositePrimitive(TexCoord6,"Texture 6 Coords", "T6","TUTV");
	SetCompositePrimitive(Color,"Color", "CLR","B8G8R8A8");
}