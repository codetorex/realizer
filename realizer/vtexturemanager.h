
#ifndef VTEXTUREMANAGER_H
#define VTEXTUREMANAGER_H

class VTexture;

class RDLL VTextureManager: public TArray<VTexture*>
{
public:
	VTexture* LoadTexture(str8& path, bool keepBitmap = false);
};


#endif