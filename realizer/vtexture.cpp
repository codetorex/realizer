#include "stdafx.h"
#include "vtexture.h"


VTexture::VTexture()
{
	pathHash = 0;
	bitmap = 0;
	texID = 0;
	Width = 0;
	Height = 0;
	format = 0;

	ResourceType = Resource::TEXTURE;
}