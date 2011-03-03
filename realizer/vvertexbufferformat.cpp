#include "stdafx.h"
#include "vvertexbufferformat.h"
#include <tbitmap.h>

TFormatElement* VVertexBufferChannels::PosX = 0; // positional X
TFormatElement* VVertexBufferChannels::PosY = 0;
TFormatElement* VVertexBufferChannels::PosZ = 0;
TFormatElement* VVertexBufferChannels::PosRHW = 0; // WTF?

TFormatElement* VVertexBufferChannels::NX = 0; // normal
TFormatElement* VVertexBufferChannels::NY = 0;
TFormatElement* VVertexBufferChannels::NZ = 0;

TFormatElement* VVertexBufferChannels::TU = 0; // texture x
TFormatElement* VVertexBufferChannels::TV = 0; // texture y

TFormatElement* VVertexBufferChannels::Position = 0;
TFormatElement* VVertexBufferChannels::Normal = 0;
TFormatElement* VVertexBufferChannels::TexCoord0 = 0;
TFormatElement* VVertexBufferChannels::TexCoord1 = 0;
TFormatElement* VVertexBufferChannels::TexCoord2 = 0;
TFormatElement* VVertexBufferChannels::TexCoord3 = 0;
TFormatElement* VVertexBufferChannels::TexCoord4 = 0;
TFormatElement* VVertexBufferChannels::TexCoord5 = 0;
TFormatElement* VVertexBufferChannels::TexCoord6 = 0;
TFormatElement* VVertexBufferChannels::Color = 0; // ARGB order in dX

TArray<TFormatElement*>* VVertexBufferChannels::VBChannelRegistry = 0; // ARGB order in dX

VVertexBufferFormat* VVertexBufferFormats::Textured1 = 0;

void VVertexBufferChannels::Initialize()
{
	PosX = new TFormatElement("Position X","PX",tc_float);
	PosY = new TFormatElement("Position Y","PY",tc_float);
	PosZ = new TFormatElement("Position Z","PZ",tc_float);
	PosRHW = new TFormatElement("Position RHW","RHW",tc_float);

	NX = new TFormatElement("Normal X","NX",tc_float);
	NY = new TFormatElement("Normal Y","NY",tc_float);
	NZ = new TFormatElement("Normal Z","NZ",tc_float);

	TU = new TFormatElement("Texture U","TU",tc_float);
	TV = new TFormatElement("Texture V","TV",tc_float);

	VBChannelRegistry = new TArray<TFormatElement*>();
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

	Position = new TFormatElementGroup("Position","POS",VBChannelRegistry,"PXPYPZ");
	Normal = new TFormatElementGroup("Normal", "NORM",VBChannelRegistry,"NXNYNZ");
	TexCoord0 = new TFormatElementGroup("Texture 0 Coords", "T0",VBChannelRegistry,"TUTV");
	TexCoord1 = new TFormatElementGroup("Texture 1 Coords", "T1",VBChannelRegistry,"TUTV");
	TexCoord2 = new TFormatElementGroup("Texture 2 Coords", "T2",VBChannelRegistry,"TUTV");
	TexCoord3 = new TFormatElementGroup("Texture 3 Coords", "T3",VBChannelRegistry,"TUTV");
	TexCoord4 = new TFormatElementGroup("Texture 4 Coords", "T4",VBChannelRegistry,"TUTV");
	TexCoord5 = new TFormatElementGroup("Texture 5 Coords", "T5",VBChannelRegistry,"TUTV");
	TexCoord6 = new TFormatElementGroup("Texture 6 Coords", "T6",VBChannelRegistry,"TUTV");
	Color = new TFormatElementGroup("Color", "CLR",VBChannelRegistry,"A8R8G8B8");

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