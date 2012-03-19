#ifndef VSCENEMANAGER_H
#define VSCENEMANAGER_H

#include "tarray.h"
#include "vscenelayered.h"
#include "vscenerender.h"

class VSceneManager: public VSceneLayered
{
public:
	inline void LoadSceneAfterBegin(VScene* scene)
	{
		VScene* bs = FindSceneByType(ST_BEGIN);
		scene->Initialize();
		InsertAfter(bs,scene);
	}

	inline void LoadScaneBeforeEnd(VScene* scene)
	{
		VScene* bs = FindSceneByType(ST_END);
		scene->Initialize();
		InsertBefore(bs,scene);
	}


	inline void LoadScene(VScene* scene)
	{
		scene->Initialize();
		Add(scene);
	}

	inline void UnloadScene(VScene* scene)
	{
		scene->Finalize();
		Remove(scene);
	}

	inline void ActivateScene(VScene* scene)
	{
		scene->Flags += SF_ACTIVE;
	}

	inline void DeactivateScene(VScene* scene)
	{
		scene->Flags += SF_ACTIVE;
	}

	inline void ChangeScene(VScene* oldScene, VScene* newScene)
	{

	}

	/**
	 * Setups basic stack for rendering. Clears screen and swaps buffers. You can insert additional scenes between them.
	 */
	inline void SetupBasicStack()
	{
		LoadScene( new VSceneRenderBegin() );

		LoadScene( new VSceneRenderEnd() );
	}

	inline void Run()
	{
		Update();
		Render();
	}
};

#endif