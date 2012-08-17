#ifndef TEST_GUI
#define TEST_GUI

#include "rtestsuite.h"
#include "tstringbuilder.h"
#include "mregion2.h"

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
	GListBox* lb;

	GTreeView* tv;
	GCheckBox* showRoot;
	GCheckBox* showPlus;
	GCheckBox* showLines;

	void testTimer_Elapsed();
	void testCbox_CheckedChanged();
	void showRoot_CheckedChanged();
	void showPlus_CheckedChanged();
	void showLines_CheckedChanged();

	GDropDown* dropDown;

	void testBut_Click();
	void testBut2_Click();

	void Initialize();

	void DrawData();

	void Render();

	void OnActivated();
	void OnDeactivated();
};

extern RTestGUI TestGUI;

#endif