#ifndef CENGINE_H
#define CENGINE_H

#include "vrenderer.h"
#include "vtexturemanager.h"
#include "cinput.h"
#include "vscenemanager.h"
#include "tfilesystem.h"
#include "ctiming.h"
#include "vdraw.h"
#include "vgui.h"

/**
* Main Realizer engine block.
*/
class RDLL REngine
{
public:
	bool Running;

	REngine()
	{
		GUI.Parent = this;
	}

	VRenderer			Renderer;
	VTextureManager		Textures;
	CInputManager		Inputs;
	VSceneManager		Scenes;
	TFileSystem			FileSystem;
	CTiming				Time;
	VDraw				Draw; // a vbo for rendering 2d, makes less creation of VBO's for small renderings.
	VGUI				GUI;

	//TPluginManager	Plugins;
	//NNetworkManager	Network;
	//CHookManager		Hooks;
	//CAudioManager		Audio;

	//Mesh, model, shader, material managers?

	void Initialize(int width,int height, const TString& title, bool fullscreen = false,int bits = 24);
	void Run();

	inline void Release(VTexture** res)
	{
		Textures.ReleaseTexture(res);
	}
};

extern REngine Engine;

/*class CInputMap
{
public:
	REngine* engine;

	virtual void Setup() = 0;
};

class CHelicopterMap: public CInputMap
{
	static CHelicopterInputs* Instance;

	CAction Fire;
	CAction X;
	CAction Y;


};

class CCarMap: public CInputMap
{
public:

	CAction Gas;
	CAction Brake;
	CAction TurnLeft;
	CAction TurnRight;
	CAction GearUp;
	CAction GearDown;
	CAction Handbrake;
};


class GCar: public GEntity
{


	void OnEnter()
	{
		engine.Inputs.SetActionMap(new CHelicopterInputs()); ?
	}
};

class GCarRemoteController: public GWeapon
{
	GCar* ControllingCar;

	void OnAlternativeFire()
	{
		TraceResult tr;
		TraceRay(lookray,&tr);

		if (tr.ClassName == "Vehicle_Car")
		{
			ControllingCar = tr.Entity;
			debug.print("Remote controller set to car: %X", tr.Entity);
		}
		else
		{
			debug.print("Remote controller can't find car");
		}
	}

	void OnFire()
	{
		engine.Inputs.SetActionMap(new CCarInputs(ControllingCar));
	}
};*/

#endif