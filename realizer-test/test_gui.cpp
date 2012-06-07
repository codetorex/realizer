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

	Log.Output(LG_INF, "Sizeof GObject is %", sfu(sizeof(GObject)));
	Log.Output(LG_INF, "Sizeof GTreeNode is %", sfu(sizeof(GTreeNode)));

	TString dop = TString::Format("New item with id %",sfu(lb->Items.Count)); 
	lb->AddItem(dop);
	Log.Output(LG_INF,dop);
}

void RTestGUI::Initialize()
{
	TestSkin = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");

	TestDesktop = Engine.GUI.CreateDesktop(false);

	OnActivated();

	GWindow* testWin = new GWindow();
	testWin->SetRectangle(100,100,300,200);
	testWin->Text = "Testing Window";

	GButton* testBut = new GButton();
	testBut->SetRectangle(50,50,100,27);
	testBut->Text = "Testing Button";
	testBut->Click += GetHandler(this, &RTestGUI::testBut_Click);

	GButton* testBut2 = new GButton();
	testBut2->SetRectangle(50,85,100,27);
	testBut2->Text = "Log Button 2";
	testBut2->Click += GetHandler(this, &RTestGUI::testBut2_Click);

	//testBut->Click += GetHandler(this, &IntroScene::tstBut_Click);


	GMenuStrip* testMenu = new GMenuStrip();
	testMenu->SetRectangle(0,0,100,20);
	testMenu->Dock = DCK_TOP;


	Engine.GUI.Desktop->AddChild(testWin);
	testWin->AddChild(testBut);
	testWin->AddChild(testBut2);
	testWin->AddChild(testMenu);


	throw NotImplementedException();
	// IMPLEMENT THESE
	/*GImageList* imgList = new GImageList(TestSkin->SkinTexture, TestSkin->Pack);
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/silk/wand.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/silk/page_white.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/silk/disk.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/fugue/folder.png") );
	imgList->AddImage( Engine.Textures.LoadToBitmap("icons/silk/folder.png") );*/


	GMenuItem* fileMenu = testMenu->AddItem("File");
	GMenuItem* fileNewItem = new GMenuItem();
	fileNewItem->SetRectangle(0,0,10,20);
	fileNewItem->Text = "New";
	fileNewItem->Image.SetImage(imgList->GetImage(1));
	fileNewItem->Click += GetHandler(this, &RTestGUI::testBut2_Click );
	fileMenu->SubItems.AddChild(fileNewItem);

	GMenuItem* fileOpenItem = new GMenuItem();
	fileOpenItem->SetRectangle(0,0,10,20);
	fileOpenItem->Text = "Open";
	fileOpenItem->Image.SetImage(imgList->GetImage(4));
	fileMenu->SubItems.AddChild(fileOpenItem);

	GMenuItem* fileOpenRecentItem = new GMenuItem();
	fileOpenRecentItem->SetRectangle(0,0,10,20);
	fileOpenRecentItem->Text = "Open Recent";
	fileOpenRecentItem->Image.SetImage(imgList->GetImage(4));
	fileMenu->SubItems.AddChild(fileOpenRecentItem);

	GMenuItem* testItem2 = new GMenuItem();
	testItem2->SetRectangle(0,0,10,20);
	testItem2->Text = "../arecentfile.cpp";
	testItem2->Image.SetImage(imgList->GetImage(1));
	testItem2->Click += GetHandler(this, &RTestGUI::testBut2_Click );
	fileOpenRecentItem->SubItems.AddChild(testItem2);

	fileMenu->Layout();


	testMenu->AddItem("Edit");
	testMenu->AddItem("View");
	testMenu->AddItem("Help");
	testMenu->AddItem("DOP",GetHandler(this, &RTestGUI::testBut2_Click));

	testMenu->Layout();

	dropDown = new GDropDown();
	dropDown->SetRectangle(0,0,100,100);


	GMenuItem* menuItem1 = new GMenuItem();
	menuItem1->SetRectangle(0,0,10,20);
	menuItem1->Text = "Menu Item test 1";

	GMenuItem* menuItem2 = new GMenuItem();
	menuItem2->SetRectangle(0,0,10,20);
	menuItem2->Text = "Another menu item";
	menuItem2->Image.SetImage( imgList->GetImage(0) );
	
	testWin->OwnObject(dropDown);
	dropDown->AddChild(menuItem1);
	dropDown->AddChild(menuItem2);
	
	dropDown->Layout();

	GToolStrip* toolBar = new GToolStrip();
	toolBar->SetRectangle(0,0,100,25);
	toolBar->Dock = DCK_TOP;
	testWin->AddChild(toolBar);

	toolBar->AddButton("New", imgList->GetImage(1),GetHandler(this, &RTestGUI::testBut2_Click));
	toolBar->AddButton("Save", imgList->GetImage(2),GetHandler(this, &RTestGUI::testBut2_Click),false);

	toolBar->Layout();

	GScrollBar* sb = new GScrollBar();
	sb->SetRectangle(300,100,20,200);
	sb->Dock = DCK_RIGHT;
	testWin->AddChild(sb);

	GScrollBar* sb2 = new GScrollBar();
	sb2->setOrientation(GO_HORIZONTAL);
	sb2->SetRectangle(300,100,200,20);
	sb2->Dock = DCK_BOTTOM;
	testWin->AddChild(sb2);
	sb2->Layout();

	GWindow* otherWin = new GWindow();
	otherWin->SetRectangle(500,70,400,370);
	otherWin->Text = "Other Window";
	Engine.GUI.Desktop->AddChild(otherWin);


	GTabControl* tabc = new GTabControl();
	tabc->SetRectangle(20,20,340,300);
	//tabc->Dock = DCK_FILL;
	otherWin->AddChild(tabc);
	GTabPage* p1 = tabc->AddPage("Initial Tests");
	GTabPage* p2 = tabc->AddPage("Split Test");
	GTabPage* p3 = tabc->AddPage("TreeView Test");

	GLabel* testLabel = new GLabel();
	testLabel->SetRectangle(10,10,100,20);
	testLabel->Text = "Test label here";

	testCbox = new GCheckBox();
	testCbox->SetRectangle(10,30,100,20);
	//testCbox->Text = "A";
	testCbox->CheckedChanged += GetHandler(this, &RTestGUI::testCbox_CheckedChanged);
	testCbox->Text = "Debug Mode";



	GRadioButton* testRadio = new GRadioButton();
	testRadio->SetRectangle(10,50,100,20);
	testRadio->Text = "option 1";

	GRadioButton* testRadio2 = new GRadioButton();
	testRadio2->SetRectangle(10,70,100,20);
	testRadio2->Text = "option 2";

	testPbar = new GProgressBar();
	testPbar->SetRectangle(10,100,200,32);
	testPbar->Value = 50;
	testPbar->ShowPercent = true;

	GTimer* testTimer = new GTimer();
	testTimer->Delay = 100;
	testTimer->RealTime = true;
	testTimer->Elapsed += GetHandler(this, &RTestGUI::testTimer_Elapsed);

	GTextBox* testText = new GTextBox();
	testText->SetRectangle(10,140,200,20);

	lb = new GListBox();
	lb->SetRectangle(10,170,200,100);
	
	p1->AddChild(testLabel);
	p1->AddChild(testCbox);
	p1->AddChild(testRadio);
	p1->AddChild(testRadio2);
	p1->AddChild(testPbar);
	p1->AddChild(testTimer);
	p1->AddChild(testText);
	p1->AddChild(lb);


	GSplitContainer* sc = new GSplitContainer();
	sc->Dock = DCK_FILL;
	sc->SetRectangle(0,0,100,100);
	p2->AddChild(sc);

	showRoot = new GCheckBox();
	showRoot->SetRectangle(220,10,100,20);
	showRoot->Text = "Show root";
	showRoot->setChecked(true);
	showRoot->CheckedChanged += GetHandler(this,&RTestGUI::showRoot_CheckedChanged);
	p3->AddChild(showRoot);

	showLines = new GCheckBox();
	showLines->SetRectangle(220,30,100,20);
	showLines->Text = "Show lines";
	showLines->setChecked(true);
	showLines->CheckedChanged += GetHandler(this,&RTestGUI::showLines_CheckedChanged);
	p3->AddChild(showLines);

	showPlus = new GCheckBox();
	showPlus->SetRectangle(220,50,100,20);
	showPlus->Text = "Show plus minus";
	showPlus->setChecked(true);
	showPlus->CheckedChanged += GetHandler(this, &RTestGUI::showPlus_CheckedChanged);
	p3->AddChild(showPlus);
	

	tv = new GTreeView();
	//tv->Dock = DCK_FILL;
	tv->ImageList = imgList;
	tv->ImageIndex = 3;
	tv->ShowRoot = true;
	tv->SetRectangle(10,10,200,200);
	tv->RootNode->Image = &imgList->GetImage(3);
	p3->AddChild(tv);

	GTreeNode* nod = tv->RootNode->AddNode("Burak");
	nod->Expanded = true;
	nod->AddNode("Child 1");
	GTreeNode* nod2 = nod->AddNode("Child 2");
	nod2->Expanded = true;
	nod2->AddNode("Grandchild 1");
	nod2->AddNode("Grandchild 2");
	nod2->AddNode("Grandchild 3");

	for (int i=0;i<20;i++)
	{
		nod2->AddNode("More Grandchilds");
	}

	nod->AddNode("Child 3");
	nod->AddNode("Child 4");


	tv->RootNode->AddNode("Karub");
	tv->RootNode->AddNode("Alex");
	tv->RootNode->Expanded = true;

	Engine.GUI.Desktop->Layout();

	TStream* fs = Engine.FileSystem.Open("outputSKIN.png",fm_Write);
	TestSkin->SkinTexture->bitmap->Save(fs,TBitmapCodecs::Png);
	fs->Close();

	
}

void RTestGUI::Render()
{
	Engine.Renderer.Enter2D();

	Engine.GUI.Render();

	int topCount = Engine.GUI.Desktop->OnTopObjects.Count;
	
	TStringBuilderStack<256> sb;
	sb.Append("Top objects: ");
	sb.Append(sfi(topCount,-8));
	TestSuite.WriteTextRight(sb);

	sb.Clear();
	sb.Append("Focused: ");
	sb.Append(sfx((ui32)Engine.GUI.Focused,-8));
	TestSuite.WriteTextRight(sb);


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

void RTestGUI::showRoot_CheckedChanged()
{
	tv->ShowRoot = showRoot->Checked;
}

void RTestGUI::showPlus_CheckedChanged()
{
	tv->ShowPlusMinus = showPlus->Checked;
}

void RTestGUI::showLines_CheckedChanged()
{
	tv->ShowLines = showLines->Checked;
}