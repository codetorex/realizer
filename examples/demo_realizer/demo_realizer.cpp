// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>
#include "mmatrix.h"
//#include "tstreamwriter.h"


#include "gschemedskinbuilder.h"
#include "gwindow.h"
#include "gbutton.h"
#include "gfont.h"
#include "glabel.h"
#include "gcheckbox.h"
#include "gradiobutton.h"

#include <tencoding.h>
#include <tutf8encoding.h>
#include "tstring.h"

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

	void tstBut_Click()
	{
		TWinTools::ShowMessage("Hello!");
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

		//TestString.FormatInplace("Current frame per second (FPS): %i",fps);
		TestString = "Current frame per second (FPS): ";
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
	}

	void Initialize()
	{
		TestString = TString(512);

		lastOlcum = 0;

		SceneName = "Intro Scene";
		// Load resources here



		TestTexture = Engine.Textures.LoadTexture("test.bmp");

		// CREATE FONT CACHE FIRST, OR LOAD FROM FILE
		TStream* fontcacheStream = Engine.FileSystem.Open( "fontcache.rcf", fm_Write);
		Engine.GUI.Fonts.Cache.CreateCache();
		Engine.GUI.Fonts.Cache.SaveCache(fontcacheStream);

		TestTGATexture = Engine.Textures.LoadTexture("Acrylic 7/but_close.tga");

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
		otherWin->SetSize(500,500,200,200);
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


		Engine.GUI.Desktop->AddChild(otherWin);
		otherWin->AddChild(testLabel);
		otherWin->AddChild(testCbox);
		otherWin->AddChild(testRadio);
		otherWin->AddChild(testRadio2);

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
		Engine.Release(&TestTexture);

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
	

	Engine.Renderer.InitializeRenderer(1280,720,"Realizer3D",false,24);
	
	CTriggerAction* exitAction = Engine.Inputs.CreateAction("ExitEngine",&Engine.running);
	Engine.Inputs.CreateMappedKeyboard();
	Engine.Inputs.BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

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