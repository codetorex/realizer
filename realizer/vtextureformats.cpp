#include "stdafx.h"
#include "vtextureformats.h"

bool VTextureChannels::ExtensionsInitialized = false;

TFormatElement* VTextureChannels::cDepth32 = 0;
TFormatElement* VTextureChannels::cDepth24 = 0;
TFormatElement* VTextureChannels::cDepth16 = 0;
TFormatElement* VTextureChannels::cStencil = 0;
TFormatElement* VTextureChannels::cUnused = 0;
TFormatElement* VTextureChannels::cRedf = 0;
TFormatElement* VTextureChannels::cGreenf = 0;
TFormatElement* VTextureChannels::cBluef = 0;
TFormatElement* VTextureChannels::cAlphaf = 0;
TFormatElement* VTextureChannels::cReds = 0;
TFormatElement* VTextureChannels::cGreens = 0;
TFormatElement* VTextureChannels::cBlues = 0;
TFormatElement* VTextureChannels::cAlphas = 0;
TFormatElement* VTextureChannels::cLuminance = 0;
TFormatElement* VTextureChannels::cCompressedStream = 0;

void VTextureChannels::CreateExtendedChannels()
{
	if (BitmapChannelRegistry == NULL)
	{
		throw Exception("There is no bitmap channel registry");
	}

	cDepth32 = new TFormatElement("Depth (float)","D32",tc_float);
	cDepth24 = new TFormatElement("Depth (24-bit float)","D24",tc_float24);
	cDepth16 = new TFormatElement("Depth (short)","D16",tc_short);
	cStencil = new TFormatElement("Stencil (byte)","S8",tc_byte);

	cUnused = new TFormatElement ("Unused (byte)","X8",tc_byte);

	cRedf    = new TFormatElement("Red (float)","RF",tc_float);
	cGreenf  = new TFormatElement("Green (float)","GF",tc_float);
	cBluef   = new TFormatElement("Blue (float)","BF",tc_float);
	cAlphaf  = new TFormatElement("Alpha (float)","AF",tc_float);

	cReds    = new TFormatElement("Red (short)","R16",tc_short);
	cGreens  = new TFormatElement("Green (short)","G16",tc_short);
	cBlues   = new TFormatElement("Blue (short)","B16",tc_short);
	cAlphas  = new TFormatElement("Alpha (short)","A16",tc_short);

	cLuminance = new TFormatElement("Luminance (byte)","L8",tc_byte);

	cCompressedStream = new TFormatElement("Compressed stream","STREAM",tc_notdefined);

	BitmapChannelRegistry->Add(cDepth32);
	BitmapChannelRegistry->Add(cDepth24);
	BitmapChannelRegistry->Add(cDepth16);
	BitmapChannelRegistry->Add(cStencil);
	BitmapChannelRegistry->Add(cUnused);
	BitmapChannelRegistry->Add(cRedf);
	BitmapChannelRegistry->Add(cGreenf);
	BitmapChannelRegistry->Add(cBluef);
	BitmapChannelRegistry->Add(cAlphaf);
	BitmapChannelRegistry->Add(cReds);
	BitmapChannelRegistry->Add(cGreens);
	BitmapChannelRegistry->Add(cBlues);
	BitmapChannelRegistry->Add(cAlphas);
	BitmapChannelRegistry->Add(cLuminance);
	BitmapChannelRegistry->Add(cCompressedStream);
}

VTextureFormat* VTextureFormats::fXRGB = 0;
VTextureFormat* VTextureFormats::fXBGR = 0;

VTextureFormat* VTextureFormats::fABGR16 = 0;
VTextureFormat* VTextureFormats::fDXT1 = 0;
VTextureFormat* VTextureFormats::fDXT3 = 0;
VTextureFormat* VTextureFormats::fDXT5 = 0;

VTextureFormat* VTextureFormats::fD32 = 0;
VTextureFormat* VTextureFormats::fD24S8 = 0;
VTextureFormat* VTextureFormats::fD16 = 0;
VTextureFormat* VTextureFormats::fL8 = 0;

VTextureFormat* VTextureFormats::fR32F = 0;
VTextureFormat* VTextureFormats::fGR32F = 0;
VTextureFormat* VTextureFormats::fABGR32F = 0;

void VTextureFormats::CreateExtendedFormats()
{
	if (BitmapFormatRegistry != 0)
	{
		delete BitmapFormatRegistry;
	}

	if (TBitmapChannels::BitmapChannelRegistry == NULL)
	{
		VTextureChannels k;
		k.Initialize();
	}

	BitmapFormatRegistry = new TArray<TBufferFormat*>();

	// Initialize initial formats
	fARGB         = VTextureFormat::CreateFormat("ARGB","A8R8G8B8");
	fABGR         = VTextureFormat::CreateFormat("ABGR","A8B8G8R8");
	fRGBA         = VTextureFormat::CreateFormat("RGBA","R8G8B8A8");
	fBGRA         = VTextureFormat::CreateFormat("BGRA","B8G8R8A8");
	fRGB          = VTextureFormat::CreateFormat("RGB","R8G8B8");
	fBGR          = VTextureFormat::CreateFormat("BGR","B8G8R8");

	//Initialize Extended Formats
	fXRGB		  = new VTextureFormat("XRGB","X8R8G8B8");
	fXBGR		  = new VTextureFormat("XBGR","X8B8G8R8");

	fABGR16       = new VTextureFormat("ABGR16","A16B16G16R16");
	fDXT1         = new VTextureFormat("DXT1","STREAM");
	fDXT3         = new VTextureFormat("DXT3","STREAM");
	fDXT5         = new VTextureFormat("DXT5","STREAM");

	fD32          = new VTextureFormat("D32","D32");
	fD24S8        = new VTextureFormat("D24S8","D24S8");
	fD16          = new VTextureFormat("D16","D16");
	fL8           = new VTextureFormat("L8","L8");

	fR32F         = new VTextureFormat("R32F","RF");
	fGR32F        = new VTextureFormat("GR32F","GFRF");
	fABGR32F      = new VTextureFormat("ABGR32F","AFBFGFRF");

	BitmapFormatRegistry->Add(fARGB);
	BitmapFormatRegistry->Add(fABGR);
	BitmapFormatRegistry->Add(fRGBA);
	BitmapFormatRegistry->Add(fBGRA);
	BitmapFormatRegistry->Add(fRGB);
	BitmapFormatRegistry->Add(fBGR);

	BitmapFormatRegistry->Add((TBufferFormat*)fXRGB);
	BitmapFormatRegistry->Add((TBufferFormat*)fXBGR);
	BitmapFormatRegistry->Add((TBufferFormat*)fABGR16);
	BitmapFormatRegistry->Add((TBufferFormat*)fDXT1);
	BitmapFormatRegistry->Add((TBufferFormat*)fDXT3);
	BitmapFormatRegistry->Add((TBufferFormat*)fDXT5);
	BitmapFormatRegistry->Add((TBufferFormat*)fD32);
	BitmapFormatRegistry->Add((TBufferFormat*)fD24S8);
	BitmapFormatRegistry->Add((TBufferFormat*)fD16);
	BitmapFormatRegistry->Add((TBufferFormat*)fL8);
	BitmapFormatRegistry->Add((TBufferFormat*)fR32F);
	BitmapFormatRegistry->Add((TBufferFormat*)fGR32F);
	BitmapFormatRegistry->Add((TBufferFormat*)fABGR32F);
}
