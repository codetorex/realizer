// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>
#include "mmatrix.h"

#include "realizerversion.h"

#include "tlogstream.h"

//#include "tstreamwriter.h"


#include "gschemedskinbuilder.h"
#include "gcomponents.h"

#include <tencoding.h>
#include <tutf8encoding.h>
#include "tstring.h"

#include "vscenenodebased.h"
#include "vanimation.h"
#include "vanimationsimple.h"
#include "vanimationbuilder.h"

/*class NodeTest: public VSceneNodeBased
{
public:
	VAnimator* TestAnimator;

	void BeginScene()
	{
		Engine.Renderer.Clear(RL_COLOR_BUFFER | RL_ZBUFFER, 0xFF888888);
		Engine.Renderer.BeginScene();

		Engine.Renderer.D3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

		Engine.Renderer.EnableBlending();
		Engine.Renderer.EnableTextureAlphaVertexColorAlpha(0);
		Engine.Renderer.Enter2D();
	}

	void EndScene()
	{
		Engine.Renderer.Exit2D();

		Engine.Renderer.EndScene();
		Engine.Renderer.SwapBackBuffer();
	}

	void Initialize()
	{
		TestAnimator = new VAnimator();
		

		this->VSceneNodeBased::Initialize(); // initializes sub nodes
	}

};*/

static char* statusmsgs[] = 
{
	"AS_NOTSTARTED",
	"AS_RUNNING",
	"AS_ENDED",
};

class IntroScene: public VScene
{
public:
	VTexture* TestTexture;
	VTexture* TestTGATexture;

	VVertexStream* TestMesh;

	GSchemedSkin* SkinTest;
	GFont* FontTest;

	float newX;
	float newY;
	float CurAng;

	int frame;
	int fps;
	int lastOlcum;

	string TestString;

	GProgressBar* pb;
	
	VAnimationMultiCosine WindowAnimation;

	VAnimationMultiLinear ColorAnimation;


	//VAnimationIntWriteBack<2> Animator;



	void tstBut_Click()
	{
		//TWinTools::ShowMessage("Hello!");
		//Animator.Rewind();
	}

	void testTimer_Elapsed()
	{
		pb->Value++;
		if (pb->Value > pb->Maximum)
		{
			pb->Value = pb->Minimum;
		}
	}

	void DebugWrite(int x, int y,char* fmt , ...)
	{
		char text[256];
		va_list ap;

		va_start(ap,fmt);
		_vsnprintf(text,256,fmt,ap);
		va_end(ap);

		FontTest->Render(text,x,y,0xFFFFFFFF);
	}

	void Render()
	{
		if (Engine.Time.TickCount - lastOlcum > 1000)
		{
			fps = frame;
			frame = 0;
			lastOlcum = Engine.Time.TickCount;
		}
		frame++;

		Engine.Renderer.Clear(RL_COLOR_BUFFER | RL_ZBUFFER, 0xFF888888);
		Engine.Renderer.BeginScene();

		Engine.Renderer.D3DDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

		Engine.Renderer.EnableBlending();
		Engine.Renderer.EnableTextureAlphaVertexColorAlpha(0);

		mat4 trans;
		trans.Translate(newX,newY,0);
		//trans.Scale(1.5f,1.5f,1.5f);

		Engine.Renderer.Enter2D();

		//Engine.Renderer.SetWorld(trans);
		
		Engine.Draw.SetTexture(TestTGATexture);
		Engine.Draw.DrawQuad(0,0,TestTGATexture->Width,TestTGATexture->Height,0,0,1,1);

		//Engine.Draw.SetTexture(TestTexture->texID);
		/*Engine.Draw.DrawQuad(-128,-128,0,0,0,0,1,1);
		Engine.Draw.DrawQuad(128,128,192,192,0,0,1,1);

		*/
		

		//TestMesh->Render();
		Engine.GUI.Render();
		Engine.Draw.Flush();

		
		DebugWrite(30,30,"Animation time: %f", WindowAnimation.CurrentTime);
		DebugWrite(30,50,"Animation value X: %f", WindowAnimation.GetValue(0));
		DebugWrite(30,70,"Animation value Y: %f", WindowAnimation.GetValue(1));
		DebugWrite(30,90,"Animation status: %s", statusmsgs[WindowAnimation.Status]);

		DebugWrite(30,110,"Animation time: %f", ColorAnimation.CurrentTime);
		DebugWrite(30,130,"Animation value R: %f", ColorAnimation.GetValue(0));
		DebugWrite(30,150,"Animation value G: %f", ColorAnimation.GetValue(1));
		DebugWrite(30,170,"Animation value B: %f", ColorAnimation.GetValue(2));
		DebugWrite(30,190,"Animation value A: %f", ColorAnimation.GetValue(3));

		//TestString.FormatInplace("Current frame per second (FPS): %i",fps);
		//TestString = "Current frame per second (FPS): ";
		//FontTest->Render(TestString,30,30, 0xFFFFFFFF,THA_LEFT,TVA_BOTTOM);

		Engine.Renderer.Exit2D();

		Engine.Renderer.EndScene();
		Engine.Renderer.SwapBackBuffer();
	}

	void Update()
	{
		float updateTime = Engine.Time.TimeDiff;
		int HalfW = Engine.Renderer.vWidth / 2;
		int HalfH = Engine.Renderer.vHeight / 2;
		
		// Update scene for animations here

		CurAng += 60.0f * updateTime; // 180 degrees per sec.

		newX = (cosf( DEGTORAD(CurAng) ) * 200) + HalfW - 64; // 200 px movement
		newY = HalfH - 64;
		//newY = (sinf( DEGTORAD(CurAng) ) * 200) + HalfH - 64; // 200 px movement
		

		WindowAnimation.AdvanceTime(Engine.Time.TimeDiff);
		ColorAnimation.AdvanceTime(Engine.Time.TimeDiff);

		/*wX.AdvanceTime(Engine.Time.TimeDiff);
		wY.AdvanceTime(Engine.Time.TimeDiff);*/
	}

	void Initialize()
	{
		TestString = TString(512);

		lastOlcum = 0;

		SceneName = "Intro Scene";
		// Load resources here

		Log.Output( LG_INF, "Initializing scene...");


		TestTexture = Engine.Textures.LoadTexture("test.bmp");

		// CREATE FONT CACHE FIRST, OR LOAD FROM FILE
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write);
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		Log.Output( LG_INF, "Font cache created and saved");

		TestTGATexture = Engine.Textures.LoadTexture("Acrylic 7/but_close.tga");

		FontTest = Engine.GUI.Fonts.GetFont("Dina",12);
		Log.Output( LG_INF, "Font 'Dina' with height '12' loaded");

		SkinTest = (GSchemedSkin*)Engine.GUI.Skins.LoadSkin("Acrylic 7/Acrylic 7.uis");
		Engine.GUI.EnableGUI(SkinTest);

		GWindow* testWin = new GWindow();
		testWin->SetSize(100,100,300,200);
		testWin->Text = "Testing Window";

		GButton* testBut = new GButton();
		testBut->SetSize(50,50,100,27);
		testBut->Text = "Testing Button";
		testBut->Click += GetHandler(this, &IntroScene::tstBut_Click);
		
		Engine.GUI.Desktop->AddChild(testWin);
		testWin->AddChild(testBut);

		GWindow* otherWin = new GWindow();
		otherWin->SetSize(500,70,300,250);
		otherWin->Text = "Other Window";

		GLabel* testLabel = new GLabel();
		testLabel->SetSize(50,50,100,27);
		testLabel->Text = "Test label here";

		GCheckBox* testCbox = new GCheckBox();
		testCbox->SetSize(50,80,100,20);
		//testCbox->Text = "A";
		testCbox->Text += L"йельский";

		GRadioButton* testRadio = new GRadioButton();
		testRadio->SetSize(50,100,100,20);
		testRadio->Text = "option 1";

		GRadioButton* testRadio2 = new GRadioButton();
		testRadio2->SetSize(50,120,100,20);
		testRadio2->Text = "option 2";

		GProgressBar* testPbar = new GProgressBar();
		testPbar->SetSize(50,145,200,32);
		testPbar->Value = 50;
		testPbar->ShowPercent = true;
		pb = testPbar;

		GTimer* testTimer = new GTimer();
		testTimer->Delay = 100;
		testTimer->RealTime = true;
		testTimer->Elapsed += GetHandler(this, &IntroScene::testTimer_Elapsed);

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

		VAnimationBuilder vb(WindowAnimation,2);

		vb.AddKeyFrame(0,250.0f,250.0f);
		vb.AddKeyFrame(30,350.0f,350.0f);
		vb.AddKeyFrame(60,250.0f,450.0f);
		vb.AddKeyFrame(90,150.0f,350.0f);
		vb.AddKeyFrame(120,250.0f,250.0f);

		vb.ConnectMemoryConverting(&otherWin->X,TDG_INT);
		vb.ConnectMemoryConverting(&otherWin->Y,TDG_INT);

		WindowAnimation.Loop = true;

		VAnimationBuilder ab(ColorAnimation,4);

		ab.AddKeyFrame(0,0,0,0,255);
		ab.AddKeyFrame(60,255,255,255,255);

		ColorAnimation.Loop = true;

		ab.ConnectMemoryConverting(&testLabel->ForeColor.r,TDG_BYTE);
		ab.ConnectMemoryConverting(&testLabel->ForeColor.g,TDG_BYTE);
		ab.ConnectMemoryConverting(&testLabel->ForeColor.b,TDG_BYTE);
		ab.ConnectMemoryConverting(&testLabel->ForeColor.a,TDG_BYTE);

		Log.Output( LG_INF, "Initialization completed");
		
		/*Animator.Setup(VAnimationAlgorithm::GetAlgorithm(VAnimationAlgorithm::AA_LINEAR),8,&(otherWin->X),&(otherWin->Y));

		Animator.AddKeyFrame(0,250.0f,250.0f);
		Animator.AddKeyFrame(30,350.0f,350.0f);
		Animator.AddKeyFrame(60,250.0f,450.0f);
		Animator.AddKeyFrame(90,150.0f,350.0f);
		Animator.AddKeyFrame(120,250.0f,250.0f);

		Animator.Loop = true;
		Animator.UpdateTimeReferences();*/

		//TColor32 testcolor(255,128,64);

		/*TStream* testBitmapStream = Engine.FileSystem.Open( "bok.bmp" , fm_Write);
		TBitmap stupid(512,512,TBitmapFormats::fBGRA);

		stupid.DrawRectangle(128,128,128,128,testcolor.bclr);
		stupid.WriteBMP(testBitmapStream);*/

		/*TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Read );

		if (fontcacheStream != 0)
		{
			Engine.GUI.Fonts.Cache.LoadCache(fontcacheStream);
		}
		else
		{
			fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write);
			Engine.GUI.Fonts.Cache.CreateCache();
			Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);
		}*/



//		FontTest = Engine.GUI.Fonts.LoadFont("dina_12.fnt");
		//FontTest = Engine.GUI.Fonts.LoadFont("segoe_11.fnt");

		/*TestMesh = new VVertexStream(VVertexBufferFormats::Textured1,4,RL_TRIANGLELIST,true);
		TestMesh->Add2DQuad1Tex(0,0,128,128,0.0f,0.0f,1.0f,1.0f);
		TestMesh->UnlockBuffer();*/

		CurAng = 0;
	}

	// TODO: we can automatize it with SceneManager, like it activates resourcePool that holds what resource created when
	// and on finalize it just empties the pool.
	void Finalize()
	{
		// Engine.Release(&TestTexture); MOVED THIS SHIT TO RESOURCE STUFF

	}
};

TString MatrixToString(mat4& src)
{
	TString result(1024);
/*	result += TString::FormatNew("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._11,src._12,src._13,src._14);
	result += TString::FormatNew("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._21,src._22,src._23,src._24);
	result += TString::FormatNew("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._31,src._32,src._33,src._34);
	result += TString::FormatNew("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._41,src._42,src._43,src._44);*/
	return result;
}

string Totor(const TString& value)
{
	string result;
	result += value;
	result += " some more text";
	return result;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{

	/*TStream* fs = File::OpenWrite("c://booook.txt");

	string japanese = "マルチバイト文字";
	string ascii = "Some random english text";


	if (japanese.Have(12496))
	{
		MessageBoxA(0,"Yes I have","No",MB_OK);
	}

	japanese += " this string will be appended to japanese one";
	japanese += "\nthis is not a const string anymore you know?";

	string kapanese = "kamil";
	kapanese = Totor(japanese);

	if (ascii.Have('z'))
	{
		MessageBoxA(0,"Yes I have too","Yes",MB_OK);
	}

	fs->Write(kapanese.Data,1,kapanese.ByteLength);
	fs->Close();
	
	return 0;
	*/

	/*ch32 c1 = Encoding::UTF8->GetCharAdv(dataPtr);
	ch32 c2 = Encoding::UTF8->GetCharAdv(dataPtr);*/

	//fs->Write(,1,StringDriverFixedWidth::Length(kp));
	//fs->Close();
	
	Application.Begin("Realizer Demo", TVersion(REALIZER_BUILD,REALIZER_PHASE), RAISE_DEFAULT_COMPANYNAME);
	USESREALIZER;

	Engine.Renderer.InitializeRenderer(1280,720,"Realizer3D",false,24);
	
	TTriggerAction* exitAction = Engine.Inputs.CreateAction("ExitEngine",&Engine.Running);
	Engine.Inputs.CreateMappedKeyboard();
	Engine.Inputs.BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	TLogStream textLog(Engine.FileSystem.Open("log.txt", fm_Write));
	Log.RegisterOutput(&textLog);

	IntroScene iScene;
	Engine.Scenes.ActivateScene(&iScene);

	/*mat4 dxIdent;

	D3DXMatrixIdentity((D3DXMATRIX*)&dxIdent);
	D3DXMatrixTranslation((D3DXMATRIX*)&dxIdent,1,2,3);

	TFileStream* fs = new TFileStream("c:\\test.txt",fm_Write);
	TStreamWriter* sw = new TStreamWriter(fs);

	sw->WriteLine( MatrixToString(dxIdent) );
	
	dxIdent.Translate(vec3(1,2,3));
	sw->WriteLine( MatrixToString(dxIdent) );

	dxIdent.Translate2(vec3(1,2,3));
	sw->WriteLine( MatrixToString(dxIdent) );

	sw->Close();*/

	Engine.Run();
	return 0;
}