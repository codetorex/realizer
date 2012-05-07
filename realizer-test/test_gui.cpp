#include "stdafx.h"
#include "test_gui.h"
#include "gschemedskin.h"
#include "tbitmapcodecs.h"
#include "vtexture.h"

RTestGUI TestGUI;


void RTestGUI::testBut_Click()
{
	dropDown->Show();
}


void RTestGUI::testBut2_Click()
{
	Log.Output(LG_INF, "Test button 2 clicked");
}

void RTestGUI::Initialize()
{
	TestSkin = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");

	TestDesktop = Engine.GUI.CreateDesktop(false);

	OnActivated();

	GWindow* testWin = new GWindow();
	testWin->SetSize(100,100,300,200);
	testWin->Text = "Testing Window";

	GButton* testBut = new GButton();
	testBut->SetSize(50,50,100,27);
	testBut->Text = "Testing Button";
	testBut->Click += GetHandler(this, &RTestGUI::testBut_Click);

	GButton* testBut2 = new GButton();
	testBut2->SetSize(50,85,100,27);
	testBut2->Text = "Log Button 2";
	testBut2->Click += GetHandler(this, &RTestGUI::testBut2_Click);

	//testBut->Click += GetHandler(this, &IntroScene::tstBut_Click);


	GMenuStrip* testMenu = new GMenuStrip();
	testMenu->SetSize(0,0,100,20);
	testMenu->Dock = DCK_TOP;


	Engine.GUI.Desktop->AddChild(testWin);
	testWin->AddChild(testBut);
	testWin->AddChild(testBut2);
	testWin->AddChild(testMenu);



	testMenu->AddItem("File");
	testMenu->AddItem("Edit");
	testMenu->AddItem("View");
	testMenu->AddItem("Help");
	testMenu->AddItem("DOP",GetHandler(this, &RTestGUI::testBut2_Click));

	testMenu->Layout();

	dropDown = new GDropDown();
	dropDown->SetSize(0,0,100,100);

	GImageList* imgList = new GImageList(TestSkin->SkinTexture, TestSkin->Pack);
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/wand.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/page_white.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/disk.png") );
	
	GMenuItem* menuItem1 = new GMenuItem();
	menuItem1->SetSize(0,0,10,20);
	menuItem1->Text = "Menu Item test 1";

	GMenuItem* menuItem2 = new GMenuItem();
	menuItem2->SetSize(0,0,10,20);
	menuItem2->Text = "Another menu item";
	menuItem2->Image.SetImage( imgList->GetImage(0) );
	
	testWin->OwnObject(dropDown);
	dropDown->AddChild(menuItem1);
	dropDown->AddChild(menuItem2);
	
	dropDown->Layout();

	GToolStrip* toolBar = new GToolStrip();
	toolBar->SetSize(0,0,100,25);
	toolBar->Dock = DCK_TOP;
	testWin->AddChild(toolBar);

	toolBar->AddButton("New", imgList->GetImage(1),GetHandler(this, &RTestGUI::testBut2_Click));
	toolBar->AddButton("Save", imgList->GetImage(2),GetHandler(this, &RTestGUI::testBut2_Click),false);

	toolBar->Layout();

	GScrollBar* sb = new GScrollBar();
	sb->SetSize(300,100,20,200);
	sb->Dock = DCK_RIGHT;
	testWin->AddChild(sb);

	GWindow* otherWin = new GWindow();
	otherWin->SetSize(500,70,300,250);
	otherWin->Text = "Other Window";

	GLabel* testLabel = new GLabel();
	testLabel->SetSize(50,50,100,27);
	testLabel->Text = "Test label here";

	testCbox = new GCheckBox();
	testCbox->SetSize(50,80,100,20);
	//testCbox->Text = "A";
	testCbox->CheckedChanged += GetHandler(this, &RTestGUI::testCbox_CheckedChanged);
	testCbox->Text = "Debug Mode";

	GCheckBox* testCbox2 = new GCheckBox();
	testCbox2->SetSize(0,0,100,20);
	testCbox2->Text = "BOKTURR";

	GRadioButton* testRadio = new GRadioButton();
	testRadio->SetSize(50,100,100,20);
	testRadio->Text = "option 1";

	GRadioButton* testRadio2 = new GRadioButton();
	testRadio2->SetSize(50,120,100,20);
	testRadio2->Text = "option 2";

	testPbar = new GProgressBar();
	testPbar->SetSize(50,145,200,32);
	testPbar->Value = 50;
	testPbar->ShowPercent = true;

	GTimer* testTimer = new GTimer();
	testTimer->Delay = 100;
	testTimer->RealTime = true;
	testTimer->Elapsed += GetHandler(this, &RTestGUI::testTimer_Elapsed);

	GTextBox* testText = new GTextBox();
	testText->SetSize(50,180,200,20);


	Engine.GUI.Desktop->AddChild(otherWin);
	otherWin->AddChild(testLabel);
	otherWin->AddChild(testCbox);
	otherWin->AddChild(testRadio);
	otherWin->AddChild(testRadio2);
	otherWin->AddChild(testPbar);
	otherWin->AddChild(testTimer);
	otherWin->AddChild(testText);
	otherWin->AddChild(testCbox2);

	Engine.GUI.Desktop->Layout();

	TStream* fs = Engine.FileSystem.Open("outputSKIN.png",fm_Write);
	TestSkin->SkinTexture->bitmap->Save(fs,TBitmapCodecs::Png);
	fs->Close();

	
}

void RTestGUI::Render()
{
	Engine.Renderer.Enter2D();

	Engine.GUI.Render();

	Engine.Draw.Flush();
	Engine.Renderer.Exit2D();
}

void RTestGUI::OnActivated()
{
	Engine.GUI.EnableGUI(TestSkin,TestDesktop);
}

void RTestGUI::OnDeactivated()
{
	Engine.GUI.DisableGUI();
}

void RTestGUI::testTimer_Elapsed()
{
	testPbar->Value++;
	if (testPbar->Value > testPbar->Maximum)
	{
		testPbar->Value = testPbar->Minimum;
	}
}

void RTestGUI::testCbox_CheckedChanged()
{
	if (testCbox->Checked)
	{
		Engine.GUI.Desktop->DebugMode = GDB_MOUSEON;
	}
	else
	{
		Engine.GUI.Desktop->DebugMode = GDB_DISABLED;
	}
}