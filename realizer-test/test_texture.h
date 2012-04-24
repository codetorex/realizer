#ifndef TEST_TEXTURE_H
#define TEST_TEXTURE_H

#include "rtestsuite.h"
#include "tstringbuilder.h"
#include "tregion.h"

class VTexture;
class TComposition;

typedef TComposition TBufferFormat;


class VTextureTest
{
public:
	VTexture* Texture;
	TString	PixelFormat;
	TString BufferFormat;
	ui32 EngineId;
	TBufferFormat* Fallback;
};

class RTestTexture: public RTestScene
{
private:
	TStringBuilder sb;
	int x;
	int y;
	TRegion texreg;

public:
	RTestTexture()
	{
		SceneName = "Texture loading test";
	}

	TArray< VTextureTest* > Tests;

	void AddTest( VTexture* tex );

	void Initialize();

	void Render();

};

extern RTestTexture TestTexture;


#endif