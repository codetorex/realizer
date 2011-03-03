#include "stdafx.h"
#include "vtexture.h"


VTexture::VTexture()
{
	pathHash = 0;
	bitmap = 0;
	texID = 0;
	width = 0;
	height = 0;

	resourceType = Resource::TEXTURE;
}