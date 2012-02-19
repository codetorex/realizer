#ifndef VVERTEXBUFFERFORMAT_H
#define VVERTEXBUFFERFORMAT_H

#include "tcompositionmanager.h"

class VVertexBufferFormat: public TBufferFormat
{
public:

	VVertexBufferFormat(const TString& name, PrimitiveArray* primitives, const TString& components): TBufferFormat(name,primitives,components)
	{

	}

	VVertexBufferFormat(const TString& longname,const TString& shortname, PrimitiveArray* primitives, const TString& components): TBufferFormat(longname,shortname,primitives,components)
	{
		// TODO: TFlexibleFormatBuilder
	}

	ui32 FormatDescriptor; // Used by DirectX 9 as FVF and OpenGL for defining witch items available.
};

class VVertexBufferFormats: public TCompositionManager
{
public:
	TCompositionPrimitive PosX; // positional X
	TCompositionPrimitive PosY;
	TCompositionPrimitive PosZ;
	TCompositionPrimitive PosRHW; // WTF?
	
	TCompositionPrimitive NX; // normal
	TCompositionPrimitive NY;
	TCompositionPrimitive NZ;

	TCompositionPrimitive TU; // texture x
	TCompositionPrimitive TV; // texture y

	TComposition Position;
	TComposition Normal;
	TComposition TexCoord0;
	TComposition TexCoord1;
	TComposition TexCoord2;
	TComposition TexCoord3;
	TComposition TexCoord4;
	TComposition TexCoord5;
	TComposition TexCoord6;
	TComposition Color; // ARGB order in dX

	VVertexBufferFormat* Textured1; // should be initialized by renderer
	VVertexBufferFormat* ColoredTextured1;

	void InitializeVertexFormats();

	inline VVertexBufferFormat* CreateVertexFormat(const TString& name, const TString& components,ui32 desc)
	{
		VVertexBufferFormat* result = (VVertexBufferFormat*)Factory->CreateFormat(name, &Primitives,components);
		AddFormat(result);
		result->FormatDescriptor = desc;
		return result;
	}

	VVertexBufferFormats(TCompositeFormatFactory* _factory): TCompositionManager(_factory)
	{
		InitializeVertexFormats();
	}
};

extern VVertexBufferFormats* VertexBufferFormats;

class TCompositeFormatFactoryVertex: public TCompositeFormatFactory
{
public:
	static TCompositeFormatFactoryVertex Instance;

	TBufferFormat* CreateFormat(const TString& name, PrimitiveArray* primitives, const TString& components)
	{ 
		return new VVertexBufferFormat(name,primitives,components);
	}

	TBufferFormat* CreateFormat(const TString& longname, const TString& shortname,PrimitiveArray* primitives, const TString& components) 
	{
		return new VVertexBufferFormat(longname,shortname,primitives,components);
	}
};

#endif