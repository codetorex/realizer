#include "stdafx.h"
#include "vvertexbufferformat.h"
#include <tbitmap.h>

TCompositionPrimitive* VVertexBufferChannels::PosX = 0; // positional X
TCompositionPrimitive* VVertexBufferChannels::PosY = 0;
TCompositionPrimitive* VVertexBufferChannels::PosZ = 0;
TCompositionPrimitive* VVertexBufferChannels::PosRHW = 0; // WTF?

TCompositionPrimitive* VVertexBufferChannels::NX = 0; // normal
TCompositionPrimitive* VVertexBufferChannels::NY = 0;
TCompositionPrimitive* VVertexBufferChannels::NZ = 0;

TCompositionPrimitive* VVertexBufferChannels::TU = 0; // texture x
TCompositionPrimitive* VVertexBufferChannels::TV = 0; // texture y

TCompositionPrimitive* VVertexBufferChannels::Position = 0;
TCompositionPrimitive* VVertexBufferChannels::Normal = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord0 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord1 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord2 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord3 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord4 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord5 = 0;
TCompositionPrimitive* VVertexBufferChannels::TexCoord6 = 0;
TCompositionPrimitive* VVertexBufferChannels::Color = 0; // ARGB order in dX

TArray<TCompositionPrimitive*>* VVertexBufferChannels::VBChannelRegistry = 0; // ARGB order in dX

VVertexBufferFormat* VVertexBufferFormats::Textured1 = 0;
VVertexBufferFormat* VVertexBufferFormats::ColoredTextured1 = 0;

void VVertexBufferChannels::Initialize()
{
	PosX = new TCompositionPrimitive("Position X","PX",tc_float);
	PosY = new TCompositionPrimitive("Position Y","PY",tc_float);
	PosZ = new TCompositionPrimitive("Position Z","PZ",tc_float);
	PosRHW = new TCompositionPrimitive("Position RHW","RHW",tc_float);

	NX = new TCompositionPrimitive("Normal X","NX",tc_float);
	NY = new TCompositionPrimitive("Normal Y","NY",tc_float);
	NZ = new TCompositionPrimitive("Normal Z","NZ",tc_float);

	TU = new TCompositionPrimitive("Texture U","TU",tc_float);
	TV = new TCompositionPrimitive("Texture V","TV",tc_float);

	VBChannelRegistry = new TArray<TCompositionPrimitive*>();
	VBChannelRegistry->Add(PosX);
	VBChannelRegistry->Add(PosY);
	VBChannelRegistry->Add(PosZ);
	VBChannelRegistry->Add(PosRHW);
	VBChannelRegistry->Add(NX);
	VBChannelRegistry->Add(NY);
	VBChannelRegistry->Add(NZ);
	VBChannelRegistry->Add(TU);
	VBChannelRegistry->Add(TV);

	VBChannelRegistry->Add(TBitmapChannels::cAlpha);
	VBChannelRegistry->Add(TBitmapChannels::cRed);
	VBChannelRegistry->Add(TBitmapChannels::cGreen);
	VBChannelRegistry->Add(TBitmapChannels::cBlue);

	Position = new TComposition("Position","POS",VBChannelRegistry,"PXPYPZ");
	Normal = new TComposition("Normal", "NORM",VBChannelRegistry,"NXNYNZ");
	TexCoord0 = new TComposition("Texture 0 Coords", "T0",VBChannelRegistry,"TUTV");
	TexCoord1 = new TComposition("Texture 1 Coords", "T1",VBChannelRegistry,"TUTV");
	TexCoord2 = new TComposition("Texture 2 Coords", "T2",VBChannelRegistry,"TUTV");
	TexCoord3 = new TComposition("Texture 3 Coords", "T3",VBChannelRegistry,"TUTV");
	TexCoord4 = new TComposition("Texture 4 Coords", "T4",VBChannelRegistry,"TUTV");
	TexCoord5 = new TComposition("Texture 5 Coords", "T5",VBChannelRegistry,"TUTV");
	TexCoord6 = new TComposition("Texture 6 Coords", "T6",VBChannelRegistry,"TUTV");
	Color = new TComposition("Color", "CLR",VBChannelRegistry,"A8R8G8B8");

	VBChannelRegistry->Add(Position);
	VBChannelRegistry->Add(Normal);
	VBChannelRegistry->Add(TexCoord0);
	VBChannelRegistry->Add(TexCoord1);
	VBChannelRegistry->Add(TexCoord2);
	VBChannelRegistry->Add(TexCoord3);
	VBChannelRegistry->Add(TexCoord4);
	VBChannelRegistry->Add(TexCoord5);
	VBChannelRegistry->Add(TexCoord6);
	VBChannelRegistry->Add(Color);
}