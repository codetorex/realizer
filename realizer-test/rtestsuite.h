#ifndef RTESTSUITE_H
#define RTESTSUITE_H

#include "tarray.h"
#include "vscene.h"
#include "mregion2.h"
#include "genums.h"
#include "gconsole.h"

class GSkin;
class GFont;
class VScene;
class RTestScene;
class TMappedKeyboard;
class GDesktop;

class RTestScene: public VScene
{
public:
	bool Initialized;

	RTestScene()
	{
		Flags.Set( SF_UPDATE | SF_RENDER | SF_ACTIVE | SF_ALWAYSONTOP );
		Initialized = false;
		SceneName = "Unnamed Test";
	}

	virtual void OnActivated() { }

	virtual void OnDeactivated() { }

	virtual void Render() { }
	virtual void Update() { }
	virtual void Initialize() { }
	virtual void Finalize() { }
};

/**
 * Main test façade.
 */
class RTestSuite: public VScene
{
private:
	TStringBuilderStack<512> sb;
	IRegion mreg;
	IRegion topreg;
	
	GDesktop* OldDesktop;
	bool OldEnabled;
	GObject* OldFocused;
	bool DebugInputEnabled;

	ui32 LastFrame;
	ui32 LastFPS;
	ui32 FrameCount;
	ui32 FPS;

public:
	RTestSuite()
	{
		Flags.Set( SF_UPDATE | SF_RENDER | SF_ACTIVE | SF_ALWAYSONTOP );
		LastFrame = 0;
		LastFPS = 0;
		FrameCount = 0;
		FPS = 100;
	}

	/// First prerequisite is working font stuff
	GFont* DebugFont;

	/// Second prerequisite is working Gui skin
	GSkin* DebugSkin;

	GDesktop* TestDesktop;
	GConsole* TestConsole;

	TArray< RTestScene* > Tests;

	TMappedKeyboard* Keyboard;

	int CurrentTestIndex;
	RTestScene* CurrentTest;

	void AddTest(RTestScene* test);

	void ActivateTest(RTestScene* Test);

	void ActivateNextTest(bool value);

	void ActivatePrevTest(bool value);

	void ActivateConsole(bool value);

	void LoadResources();

	void WriteText( TCharacterEnumerator schars, IRegion& reg, Alignment alg = CA_TopLeft);

	void Render();

	void Update();
};

extern RTestSuite TestSuite;

#endif