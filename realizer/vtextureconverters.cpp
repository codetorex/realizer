#include "stdafx.h"
#include "vtextureformats.h"
#include "tcompositegenericconverter.h"

class TBitmapConverterBGRtoBGRA: public TCompositeGenericConverter
{
public:
	TBitmapConverterBGRtoBGRA()
	{
		SourceFormat = TextureFormats->fBGR;
		DestinationFormat = TextureFormats->fBGRA;
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

class TBitmapConverteRGBtoBGRA: public TCompositeGenericConverter
{
public:
	TBitmapConverteRGBtoBGRA()
	{
		SourceFormat = TextureFormats->fRGB;
		DestinationFormat = TextureFormats->fBGRA;
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
	// 3 to 4 no swapping
	AddConverter(new TBitmapConverterBGRtoBGRA());

	AddConverter("BGR", "BGRX", new TBitmapConverterBGRtoBGRA() );

	// 3 to 4 swapping

	AddConverter(new TBitmapConverteRGBtoBGRA());

	AddConverter("RGB", "BGRX", new TBitmapConverteRGBtoBGRA() );

	AddConverter("BGR", "RGBA", new TBitmapConverteRGBtoBGRA() );

	AddConverter("BGR", "RGBX", new TBitmapConverteRGBtoBGRA() );
}