#ifndef VTEXTUREFORMATS_H
#define VTEXTUREFORMATS_H

#include <tbitmap.h>

class VTextureFormat: public TBufferFormat
{
public:
	dword FormatDescriptor;
	bool IsSupported;
	TBufferFormat* FallbackFormat;

	void Supported(dword fmtDescriptor)
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

	VTextureFormat(const TString& shortName, const TString& channels):TBufferFormat(shortName, TBitmapChannels::BitmapChannelRegistry, channels )
	{

	}

	static TBufferFormat* CreateFormat(const TString& shortName, const TString& channels)
	{
		VTextureFormat* vtf = new VTextureFormat (shortName,channels);
		return (TBufferFormat*)vtf;
	}
};

class VTextureChannels: public TBitmapChannels
{
public:
	static bool ExtensionsInitialized;

	static TCompositionPrimitive* cDepth32;
	static TCompositionPrimitive* cDepth24;
	static TCompositionPrimitive* cDepth16;

	static TCompositionPrimitive* cStencil;

	static TCompositionPrimitive* cUnused;

	static TCompositionPrimitive* cRedf;
	static TCompositionPrimitive* cGreenf;
	static TCompositionPrimitive* cBluef;
	static TCompositionPrimitive* cAlphaf;

	static TCompositionPrimitive* cReds;
	static TCompositionPrimitive* cGreens;
	static TCompositionPrimitive* cBlues;
	static TCompositionPrimitive* cAlphas;

	static TCompositionPrimitive* cLuminance;

	static TCompositionPrimitive* cCompressedStream;

	void CreateExtendedChannels();

	void Initialize()
	{
		if (BitmapChannelRegistry == 0)
		{
			CreateDefaultChannels();
		}
		CreateExtendedChannels();
	}
};

class VTextureFormats: public TBitmapFormats
{
public:
	static VTextureFormat* fXRGB;
	static VTextureFormat* fXBGR;

	static VTextureFormat* fABGR16; // each component is short
	static VTextureFormat* fDXT1;
	static VTextureFormat* fDXT3;
	static VTextureFormat* fDXT5;

	static VTextureFormat* fD32; 
	static VTextureFormat* fD24S8;
	static VTextureFormat* fD16;
	static VTextureFormat* fL8;

	static VTextureFormat* fR32F;
	static VTextureFormat* fGR32F;
	static VTextureFormat* fABGR32F;

	void CreateExtendedFormats();
	void CreateExtendedConverters();

	void Initialize()
	{
		CreateExtendedFormats();
		CreateExtendedConverters();
	}
};


#endif

