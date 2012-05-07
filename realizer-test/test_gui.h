#ifndef TEST_GUI
#define TEST_GUI

#include "rtestsuite.h"
#include "tstringbuilder.h"
#include "tregion.h"

#include "gcomponents.h"

class GSchemedSkin;
class VTexture;

/**
 * TODO: make this a whole application and plugin next time
 */
class RTestGUI: public RTestScene
{
public:
	RTestGUI()
	{
		SceneName = "GUI Test";
	}

	GSchemedSkin* TestSkin;

	GDesktop* TestDesktop;

	GProgressBar* testPbar;
	GCheckBox* testCbox;

	void testTimer_Elapsed();
	void testCbox_CheckedChanged();

	GDropDown* dropDown;

	void testBut_Click();
	void testBut2_Click();

	void Initialize();

	void Render();

	void OnActivated();
	void OnDeactivated();
};

extern RTestGUI TestGUI;

#endif