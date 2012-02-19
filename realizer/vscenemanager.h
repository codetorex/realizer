#ifndef VSCENEMANAGER_H
#define VSCENEMANAGER_H

#include "tarray.h"
#include "vscene.h"

class VSceneManager
{
public:
	TArray<VScene*> Scenes;


	inline void LoadScene(VScene* scene)
	{
		scene->Initialize();
		Scenes.Add(scene);
	}

	inline void UnloadScene(VScene* scene)
	{
		scene->Finalize();
		Scenes.Remove(scene);
	}

	inline void ActivateScene(VScene* scene)
	{
		if (!Scenes.Contains(scene))
		{
			LoadScene(scene);
		}
		scene->Active = true;
	}

	inline void Update()
	{
		TArrayEnumerator<VScene*> se(Scenes);
		while(se.MoveNext())
		{
			se.Current->Update();
		}
	}

	inline void Render()
	{
		TArrayEnumeratorReverse<VScene*> se(Scenes);
		while(se.MoveNext())
		{
			se.Current->Render();
		}
	}

	inline void Run()
	{
		Update();
		Render();
	}
};

#endif