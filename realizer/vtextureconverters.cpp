#include "stdafx.h"
#include "vtextureformats.h"

class TBitmapConverterBGRtoXRGB: public TBufferFormatGenericConverter
{
public:
	TBitmapConverterBGRtoXRGB()
	{
		SourceFormat = VTextureFormats::fBGR;
		DestinationFormat = VTextureFormats::fXRGB;
	}

	void DoConversion(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = 255;
			dst += 4;
			src += 3;
		}
	}
};

class TBitmapConverteRGBtoXRGB: public TBufferFormatGenericConverter
{
public:
	TBitmapConverteRGBtoXRGB()
	{
		SourceFormat = VTextureFormats::fRGB;
		DestinationFormat = VTextureFormats::fXRGB;
	}

	void DoConversion(byte* src, byte* dst,int pixelCount)
	{
		while(pixelCount--)
		{
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = 255;
			dst += 4;
			src += 3;
		}
	}
};


void VTextureFormats::CreateExtendedConverters()
{
	CreateDefaultConverters();

	TBitmapConverterBGRtoXRGB* BGRtoXRGB = new TBitmapConverterBGRtoXRGB();
	VTextureFormats::fBGR->Converters.Add(BGRtoXRGB);

	TBitmapConverterBGRtoXRGB* BGRtoARGB = new TBitmapConverterBGRtoXRGB();
	BGRtoARGB->DestinationFormat = VTextureFormats::fARGB;
	VTextureFormats::fBGR->Converters.Add(BGRtoARGB);

	TBitmapConverteRGBtoXRGB* RGBtoXRGB = new TBitmapConverteRGBtoXRGB();
	VTextureFormats::fRGB->Converters.Add(RGBtoXRGB);

	TBitmapConverteRGBtoXRGB* BGRtoXBGR = new TBitmapConverteRGBtoXRGB();
	BGRtoXBGR->SourceFormat = VTextureFormats::fBGR;
	BGRtoXBGR->DestinationFormat = VTextureFormats::fXBGR;
	VTextureFormats::fBGR->Converters.Add(BGRtoXBGR);
}