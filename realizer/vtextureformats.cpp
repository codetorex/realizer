#include "stdafx.h"
#include "vtextureformats.h"

VTextureFormats* TextureFormats = 0;

TCompositeFormatFactoryTexture TCompositeFormatFactoryTexture::Instance;

void VTextureFormats::InitializeTextureFormats()
{
	TextureFormats = this;

	SetPrimitive(cDepth32         ,"Depth (float)","D32",tc_float);
	SetPrimitive(cDepth24         ,"Depth (24-bit float)","D24",tc_float24);
	SetPrimitive(cDepth16         ,"Depth (short)","D16",tc_short);
	SetPrimitive(cStencil         ,"Stencil (byte)","S8",tc_byte);

	SetPrimitive(cUnused          ,"Unused (byte)","X8",tc_byte);

	SetPrimitive(cRedf            ,"Red (float)","RF",tc_float);
	SetPrimitive(cGreenf          ,"Green (float)","GF",tc_float);
	SetPrimitive(cBluef           ,"Blue (float)","BF",tc_float);
	SetPrimitive(cAlphaf          ,"Alpha (float)","AF",tc_float);

	SetPrimitive(cReds            ,"Red (short)","R16",tc_short);
	SetPrimitive(cGreens          ,"Green (short)","G16",tc_short);
	SetPrimitive(cBlues           ,"Blue (short)","B16",tc_short);
	SetPrimitive(cAlphas          ,"Alpha (short)","A16",tc_short);

	SetPrimitive(cLuminance       ,"Luminance (byte)","L8",tc_byte);

	SetPrimitive(cCompressedStream,"Compressed stream","STREAM",tc_notdefined);

	fBGRX     = (VTextureFormat*)CreateFormat("BGRX","B8G8R8X8");
	fRGBX	  = (VTextureFormat*)CreateFormat("RGBX","R8G8B8X8");

	fABGR16   = (VTextureFormat*)CreateFormat("ABGR16","A16B16G16R16");
	fDXT1     = (VTextureFormat*)CreateFormat("DXT1","STREAM");
	fDXT3     = (VTextureFormat*)CreateFormat("DXT3","STREAM");
	fDXT5     = (VTextureFormat*)CreateFormat("DXT5","STREAM");

	fD32      = (VTextureFormat*)CreateFormat("D32","D32");
	fD24S8    = (VTextureFormat*)CreateFormat("D24S8","D24S8");
	fD16      = (VTextureFormat*)CreateFormat("D16","D16");
	fL8       = (VTextureFormat*)CreateFormat("L8","L8");

	fR32F     = (VTextureFormat*)CreateFormat("R32F","RF");
	fGR32F    = (VTextureFormat*)CreateFormat("GR32F","GFRF");
	fABGR32F  = (VTextureFormat*)CreateFormat("ABGR32F","AFBFGFRF");
}