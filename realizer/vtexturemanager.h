
#ifndef VTEXTUREMANAGER_H
#define VTEXTUREMANAGER_H

#include "realizertypes.h"
#include "tarray.h"
#include "tstring.h"

class VTexture;
class TBitmap;
class TStream;

class RDLL VTextureManager: public TArray<VTexture*>
{
public:
	TBitmap* LoadToBitmap(const TString& path);
	TBitmap* LoadToBitmap(TStream* srcStream,bool closestream = true);

	VTexture* LoadTexture(const TString& path, bool keepBitmap = false);
	VTexture* LoadTexture(TStream* srcStream,bool closestream = true);

	VTexture* CreateTexture(TBitmap* source);

	void ReleaseTexture(VTexture* texture);
	
	inline void ReleaseTexture(VTexture** texture)
	{
		ReleaseTexture(*texture);
		*texture = 0;
	}
};


#endif