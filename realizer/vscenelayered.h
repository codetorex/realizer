#ifndef VSCENELAYERED_H
#define VSCENELAYERED_H

#include "vscene.h"


/**
 * Makes you able to use multiple scenes on top of each other. Like hierarchical scenes.
 * 
 */
class VSceneLayered: public VScene, public TList< VScene* >
{
public:
	VSceneLayered()
	{
		Type = ST_LAYERED;
		Flags += SF_RENDER | SF_UPDATE;
	}

	void Render()
	{
		TLinkedListEnumerator<VScene*> scenes( this );
		while(scenes.MoveNext())
		{
			if (scenes.Current->Flags == SF_RENDER)
			{
				scenes.Current->Render();
			}
		}
	}

	void Update()
	{
		TLinkedListEnumerator<VScene*> scenes( this );
		while(scenes.MoveNext())
		{
			if (scenes.Current->Flags == SF_UPDATE)
			{
				scenes.Current->Update();
			}
		}
	}

	void Initialize()
	{
		TLinkedListEnumerator<VScene*> scenes( this );
		while(scenes.MoveNext())
		{
			scenes.Current->Initialize();
		}
	}

	void Finalize()
	{
		TLinkedListEnumerator<VScene*> scenes( this );
		while(scenes.MoveNext())
		{
			scenes.Current->Finalize();
		}
	}

	inline VScene* FindSceneByType(ui32 sceneType)
	{
		TLinkedListEnumerator<VScene*> scenes(this);
		while(scenes.MoveNext())
		{
			if (scenes.Current->Type == sceneType)
			{
				return scenes.Current;
			}
		}

		return 0;
	}

	inline VScene* FindSceneByID(ui32 ID)
	{
		TLinkedListEnumerator<VScene*> scenes(this);
		while(scenes.MoveNext())
		{
			if (scenes.Current->ID == ID)
			{
				return scenes.Current;
			}
		}

		return 0;
	}
};


#endif