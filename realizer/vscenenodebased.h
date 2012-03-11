#ifndef VSCENENODEBASED_H
#define VSCENENODEBASED_H

#include "vscene.h"
#include "vnode.h"

typedef TLinkedListEnumerator<VNode*> VSceneNodeEnumerator;

/**
 * Node based scene
 */
class VSceneNodeBased: public VScene, public TList< VNode* >
{
public:

	bool DebugMode;

	VSceneNodeBased()
	{
		DebugMode = false;
		Flags += SF_RENDER | SF_UPDATE;
	}
	
	virtual void Update()
	{
		VSceneNodeEnumerator snodes(this);
		while(snodes.MoveNext())
		{
			snodes.Current->Update();
		}
	}

	virtual void Render()
	{
		BeginScene();

		VSceneNodeEnumerator snodes(this);
		while(snodes.MoveNext())
		{
			snodes.Current->Render();
		}

		if (DebugMode)
		{
			snodes.Reset();
			while(snodes.MoveNext())
			{
				snodes.Current->RenderDebug();
			}
		}

		EndScene();
	}

	virtual void Initialize()
	{
		VSceneNodeEnumerator snodes(this);
		while(snodes.MoveNext())
		{
			snodes.Current->Initialize();
		}
	}
	
	virtual void Finalize()
	{
		VSceneNodeEnumerator snodes(this);
		while(snodes.MoveNext())
		{
			snodes.Current->Finalize();
		}
	}

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
};

#endif