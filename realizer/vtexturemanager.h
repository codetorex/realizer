
#ifndef VTEXTUREMANAGER_H
#define VTEXTUREMANAGER_H

#include "realizertypes.h"
#include "tarray.h"
#include "tstring.h"

class VTexture;
class TBitmap;

class RDLL VTextureManager: public TArray<VTexture*>
{
public:
	TBitmap* LoadToBitmap(const str8& path);
	VTexture* LoadTexture(const str8& path, bool keepBitmap = false);
	VTexture* CreateTexture(TBitmap* source);

	void ReleaseTexture(VTexture* texture);
	
	inline void ReleaseTexture(VTexture** texture)
	{
		ReleaseTexture(*texture);
		*texture = 0;
	}
};


#endif