#include "stdafx.h"
#include "vtextureformats.h"

class TBitmapConverterBGRtoXRGB: public TBufferFormatConverter
{
public:
	TBitmapConverterBGRtoXRGB()
	{
		SourceFormat = VTextureFormats::fBGR;
		DestinationFormat = VTextureFormats::fXRGB;
	}

	void Convert(TFlexibleBuffer* srcBuffer)
	{
		TBitmap* bmp = (TBitmap*)srcBuffer;
		byte* srcData = srcBuffer->Buffer;
		byte* src = srcData;

		dword newSize = DestinationFormat->BytesPerItem * bmp->pixels;
		byte* dstData = new byte [newSize];
		byte* dst = dstData;
		

		int curpixel = bmp->pixels;
		while(curpixel--)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst[3] = 255;
			dst += 4;
			src += 3;
		}
		srcBuffer->BufferFormat = DestinationFormat;
		srcBuffer->ExchangeBuffer(dstData,newSize);
	}
};


class TBitmapConverteRGBtoXRGB: public TBufferFormatConverter
{
public:
	TBitmapConverteRGBtoXRGB()
	{
		SourceFormat = VTextureFormats::fRGB;
		DestinationFormat = VTextureFormats::fXRGB;
	}

	void Convert(TFlexibleBuffer* srcBuffer)
	{
		TBitmap* bmp = (TBitmap*)srcBuffer;
		byte* srcData = srcBuffer->Buffer;
		byte* src = srcData;

		dword newSize = DestinationFormat->BytesPerItem * bmp->pixels;
		byte* dstData = new byte [newSize];
		byte* dst = dstData;
		
		int curpixel = bmp->pixels;
		while(curpixel--)
		{
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = 255;
			dst += 4;
			src += 3;
		}

		srcBuffer->BufferFormat = DestinationFormat;
		srcBuffer->ExchangeBuffer(dstData,newSize);
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