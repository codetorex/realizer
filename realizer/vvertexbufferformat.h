#ifndef VVERTEXBUFFERFORMAT_H
#define VVERTEXBUFFERFORMAT_H


#include <tflexiblebuffer.h>
#include "realizertypes.h"

class VVertexBufferChannels
{
public:
	static TFormatElement* PosX; // positional X
	static TFormatElement* PosY;
	static TFormatElement* PosZ;
	static TFormatElement* PosRHW; // WTF?

	static TFormatElement* NX; // normal
	static TFormatElement* NY;
	static TFormatElement* NZ;

	static TFormatElement* TU; // texture x
	static TFormatElement* TV; // texture y

	static TFormatElement* Position;
	static TFormatElement* Normal;
	static TFormatElement* TexCoord0;
	static TFormatElement* TexCoord1;
	static TFormatElement* TexCoord2;
	static TFormatElement* TexCoord3;
	static TFormatElement* TexCoord4;
	static TFormatElement* TexCoord5;
	static TFormatElement* TexCoord6;
	static TFormatElement* Color; // ARGB order in dX


	static TArray<TFormatElement*>* VBChannelRegistry; // vertex buffer channel registry

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