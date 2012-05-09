#ifndef RTESTSUITE_H
#define RTESTSUITE_H

#include "tarray.h"
#include "vscene.h"
#include "tregion.h"
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
 * Main test fa�ade.
 */
class RTestSuite: public VScene
{
private:
	TStringBuilder sb;
	TRegion mreg;
	TRegion topreg;
	
	GDesktop* OldDesktop;
	bool OldEnabled;
	GObject* OldFocused;
	bool DebugInputEnabled;

public:
	RTestSuite()
	{
		Flags.Set( SF_UPDATE | SF_RENDER | SF_ACTIVE | SF_ALWAYSONTOP );
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

	void WriteText( TCharacterEnumerator schars, TRegion& reg, ContentAlignment alg = CA_TopLeft);

	void Render();

	void Update();
};

extern RTestSuite TestSuite;

#endif