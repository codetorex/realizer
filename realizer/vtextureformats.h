#ifndef VTEXTUREFORMATS_H
#define VTEXTUREFORMATS_H

#include "tbitmapformats.h"

class VTextureFormat: public TBufferFormat
{
public:
	ui32 FormatDescriptor;
	bool IsSupported;
	TBufferFormat* FallbackFormat;

	void Supported(ui32 fmtDescriptor)
	{
		FormatDescriptor = fmtDescriptor;
		IsSupported = true;
		FallbackFormat = 0;
	}

	void UnSupported(TBufferFormat* fallback)
	{
		FormatDescriptor = 0;
		IsSupported = false;
		FallbackFormat = fallback;
	}

	VTextureFormat(const TString& name, PrimitiveArray* primitives, const TString& components): TBufferFormat(name,primitives,components)
	{

	}

	VTextureFormat(const TString& longname,const TString& shortname, PrimitiveArray* primitives, const TString& components): TBufferFormat(longname,shortname,primitives,components)
	{

	}
};



class VTextureFormats: public TBitmapFormats
{
public:
	TCompositionPrimitive cDepth32;
	TCompositionPrimitive cDepth24;
	TCompositionPrimitive cDepth16;

	TCompositionPrimitive cStencil;

	TCompositionPrimitive cUnused;

	TCompositionPrimitive cRedf;
	TCompositionPrimitive cGreenf;
	TCompositionPrimitive cBluef;
	TCompositionPrimitive cAlphaf;

	TCompositionPrimitive cReds;
	TCompositionPrimitive cGreens;
	TCompositionPrimitive cBlues;
	TCompositionPrimitive cAlphas;

	TCompositionPrimitive cLuminance;

	TCompositionPrimitive cCompressedStream;

	VTextureFormat* fXRGB;
	VTextureFormat* fXBGR;

	VTextureFormat* fABGR16; // each component is short
	VTextureFormat* fDXT1;
	VTextureFormat* fDXT3;
	VTextureFormat* fDXT5;

	VTextureFormat* fD32; 
	VTextureFormat* fD24S8;
	VTextureFormat* fD16;
	VTextureFormat* fL8;

	VTextureFormat* fR32F;
	VTextureFormat* fGR32F;
	VTextureFormat* fABGR32F;

	void InitializeTextureFormats();
	void CreateTextureConverters();

	VTextureFormats(TCompositeFormatFactory* _factory): TBitmapFormats(_factory)
	{
		InitializeTextureFormats();
		CreateTextureConverters();
	}

};

extern VTextureFormats* TextureFormats;

class TCompositeFormatFactoryTexture: public TCompositeFormatFactory
{
public:
	static TCompositeFormatFactoryTexture Instance;

	TBufferFormat* CreateFormat(const TString& name, PrimitiveArray* primitives, const TString& components)
	{ 
		return new VTextureFormat(name,primitives,components);
	}

	TBufferFormat* CreateFormat(const TString& longname, const TString& shortname,PrimitiveArray* primitives, const TString& components) 
	{
		return new VTextureFormat(longname,shortname,primitives,components);
	}
};

#endif

