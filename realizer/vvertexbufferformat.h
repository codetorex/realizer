#ifndef VVERTEXBUFFERFORMAT_H
#define VVERTEXBUFFERFORMAT_H


#include <tcompositebuffer.h>
#include "realizertypes.h"

class VVertexBufferChannels
{
public:
	static TCompositionPrimitive* PosX; // positional X
	static TCompositionPrimitive* PosY;
	static TCompositionPrimitive* PosZ;
	static TCompositionPrimitive* PosRHW; // WTF?

	static TCompositionPrimitive* NX; // normal
	static TCompositionPrimitive* NY;
	static TCompositionPrimitive* NZ;

	static TCompositionPrimitive* TU; // texture x
	static TCompositionPrimitive* TV; // texture y

	static TCompositionPrimitive* Position;
	static TCompositionPrimitive* Normal;
	static TCompositionPrimitive* TexCoord0;
	static TCompositionPrimitive* TexCoord1;
	static TCompositionPrimitive* TexCoord2;
	static TCompositionPrimitive* TexCoord3;
	static TCompositionPrimitive* TexCoord4;
	static TCompositionPrimitive* TexCoord5;
	static TCompositionPrimitive* TexCoord6;
	static TCompositionPrimitive* Color; // ARGB order in dX


	static TArray<TCompositionPrimitive*>* VBChannelRegistry; // vertex buffer channel registry

	void Initialize();
};

class VVertexBufferFormat: public TBufferFormat
{
public:
	VVertexBufferFormat(const str8& _Name, const str8& elementNames,dword fmtDesc = 0): TBufferFormat(_Name,VVertexBufferChannels::VBChannelRegistry,elementNames)
	{
		FormatDescriptor = fmtDesc;
		/*for (int i=0;i<ElementCount;i++) offsetler belirleyerek, generic islemleri yapabilen bi fonksiyon dusunmustum ama
		{
			TFormatElement* curE = Elements[i];
			if(curE == VVertexBufferChannels::Position)
			{

			}
		}*/
		// TODO: TFlexibleFormatBuilder
	}

	dword FormatDescriptor; // Used by DirectX 9 as FVF and OpenGL for defining witch items available.
};

class VVertexBufferFormats
{
public:
	static VVertexBufferFormat* Textured1; // should be initialized by renderer
	static VVertexBufferFormat* ColoredTextured1;
};


#endif