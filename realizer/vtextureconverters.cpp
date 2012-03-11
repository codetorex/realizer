#include "stdafx.h"
#include "vtextureformats.h"
#include "tcompositegenericconverter.h"

class TBitmapConverterBGRtoXRGB: public TCompositeGenericConverter
{
public:
	TBitmapConverterBGRtoXRGB()
	{
		SourceFormat = TextureFormats->fBGR;
		DestinationFormat = TextureFormats->fXRGB;
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

class TBitmapConverteRGBtoXRGB: public TCompositeGenericConverter
{
public:
	TBitmapConverteRGBtoXRGB()
	{
		SourceFormat = TextureFormats->fRGB;
		DestinationFormat = TextureFormats->fXRGB;
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


void VTextureFormats::CreateTextureConverters()
{
	TBitmapConverterBGRtoXRGB* BGRtoXRGB = new TBitmapConverterBGRtoXRGB();
	fBGR->Converters.Add(BGRtoXRGB);

	TBitmapConverterBGRtoXRGB* BGRtoARGB = new TBitmapConverterBGRtoXRGB();
	BGRtoARGB->DestinationFormat = TextureFormats->fARGB;
	fBGR->Converters.Add(BGRtoARGB);

	TBitmapConverteRGBtoXRGB* RGBtoXRGB = new TBitmapConverteRGBtoXRGB();
	fRGB->Converters.Add(RGBtoXRGB);

	TBitmapConverteRGBtoXRGB* BGRtoXBGR = new TBitmapConverteRGBtoXRGB();
	BGRtoXBGR->SourceFormat = TextureFormats->fBGR;
	BGRtoXBGR->DestinationFormat = TextureFormats->fXBGR;
	fBGR->Converters.Add(BGRtoXBGR);
}