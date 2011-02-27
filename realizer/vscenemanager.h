#ifndef VSCENEMANAGER_H
#define VSCENEMANAGER_H

#include "tstring.h"
#include "tarray.h"

class VScene
{
public:
	string SceneName;

	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
};

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


	inline void Render()
	{
		// TODO: check for is any active scene?
		ActiveScene->Render();
	}
};

#endif