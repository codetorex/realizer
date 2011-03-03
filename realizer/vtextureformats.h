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

	VTextureFormat(const str8& shortName, const str8& channels):TBufferFormat(shortName, TBitmapChannels::BitmapChannelRegistry, channels )
	{

	}

	static TBufferFormat* CreateFormat(const str8& shortName, const str8& channels)
	{
		VTextureFormat* vtf = new VTextureFormat (shortName,channels);
		return (TBufferFormat*)vtf;
	}
};

class VTextureChannels: public TBitmapChannels
{
public:
	static bool ExtensionsInitialized;

	static TFormatElement* cDepth32;
	static TFormatElement* cDepth24;
	static TFormatElement* cDepth16;

	static TFormatElement* cStencil;

	static TFormatElement* cUnused;

	static TFormatElement* cRedf;
	static TFormatElement* cGreenf;
	static TFormatElement* cBluef;
	static TFormatElement* cAlphaf;

	static TFormatElement* cReds;
	static TFormatElement* cGreens;
	static TFormatElement* cBlues;
	static TFormatElement* cAlphas;

	static TFormatElement* cLuminance;

	static TFormatElement* cCompressedStream;

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

