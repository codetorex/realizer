#include "stdafx.h"
#include "test_texture.h"
#include "cengine.h"
#include "vtexture.h"
#include "mmathcolor.h"
#include "rtestsuite.h"
#include "gfont.h"
#include "tconvert.h"
#include "tcompositebuffer.h"

RTestTexture TestTexture;

void RTestTexture::AddTest( VTexture* tex )
{
	VTextureTest* nTest = new VTextureTest();
	nTest->Texture = tex;
	nTest->PixelFormat = MathColor::ChannelOrder(tex->bitmap->Data, tex->bitmap->BufferFormat->BytesPerItem);
	nTest->BufferFormat = tex->bitmap->BufferFormat->ShortName;
	nTest->EngineId = ((VTextureFormat*)tex->bitmap->BufferFormat)->FormatDescriptor;
	nTest->Fallback = ((VTextureFormat*)tex->bitmap->BufferFormat)->FallbackFormat;

	Tests.Add(nTest);
}

void RTestTexture::Initialize()
{
	AddTest( Engine.Textures.LoadTexture("debug/color-test.png",true) );
	AddTest( Engine.Textures.LoadTexture("debug/color-test.bmp",true) );
	AddTest( Engine.Textures.LoadTexture("debug/color-test.tga",true) );
}

void RTestTexture::Render()
{
	Engine.Renderer.Enter2D();

	x = y = 100;
	
	int texSize = 200;
	int padding = 10;
	int margin = 60;

	TArrayEnumerator<VTextureTest*> te(Tests);
	while (te.MoveNext())
	{
		texreg.SetSize(x+padding,y + texSize + (4 * padding),texSize,10);

		Engine.Draw.SetTexture(0);
		Engine.Draw.DrawRectangle(x,y,texSize + (padding *2),texSize + (padding*2),TColors::white);
		
		Engine.Draw.SetTexture(te.Current->Texture);
		Engine.Draw.DrawQuad(x+padding,y+padding,x+padding + texSize ,y+padding +texSize,0,0,1,1,TColors::white);

		Engine.Draw.Flush();
		
		TestSuite.WriteText(te.Current->Texture->path,texreg);

		sb.Clear();
		sb.Append("Pixel Format: ");
		sb.Append(te.Current->PixelFormat);
		TestSuite.WriteText(sb,texreg);

		sb.Clear();
		sb.Append("Buffer Format: ");
		sb.Append(te.Current->BufferFormat);
		TestSuite.WriteText(sb,texreg);

		sb.Clear();
		sb.Append("Engine Descriptor ID: ");
		sb.Append(sfu(te.Current->EngineId));
		TestSuite.WriteText(sb,texreg);


		x += texSize + (2 * padding) + margin;
	}


	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}
