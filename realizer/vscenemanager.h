#ifndef VSCENEMANAGER_H
#define VSCENEMANAGER_H

#include "tarray.h"
#include "vscene.h"

class VSceneManager
{
public:
	TArray<VScene*> Scenes;

	VScene* ActiveScene;

	VSceneManager()
	{
		ActiveScene = 0;
	}

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
		if (Scenes.Contains(scene))
		{
			ActiveScene = scene;
		}
		else
		{
			LoadScene(scene);
			ActiveScene = scene;
		}
	}

	inline void Update()
	{
		ActiveScene->Update();
	}

	inline void Render()
	{
		// TODO: check for is any active scene?
		ActiveScene->Render();
	}

	inline void Run()
	{
		Update();
		Render();
	}
};

#endif