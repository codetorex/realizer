#ifndef VSCENELAYERED_H
#define VSCENELAYERED_H

#include "vscene.h"


/**
 * Makes you able to use multiple scenes on top of each other.
 */
class VSceneLayered: public VScene
{
public:
	TArray< VScene* > Layers;

	void Render()
	{
		TArrayEnumerator<VScene*> scenes( Layers );
		while(scenes.MoveNext())
		{
			scenes.Current->Render();
		}
	}

	void Update()
	{
		TArrayEnumerator<VScene*> scenes( Layers );
		while(scenes.MoveNext())
		{
			scenes.Current->Update();
		}
	}

	void Initialize()
	{
		TArrayEnumerator<VScene*> scenes( Layers );
		while(scenes.MoveNext())
		{
			scenes.Current->Initialize();
		}
	}

	void Finalize()
	{
		TArrayEnumerator<VScene*> scenes( Layers );
		while(scenes.MoveNext())
		{
			scenes.Current->Finalize();
		}
	}
};


#endif