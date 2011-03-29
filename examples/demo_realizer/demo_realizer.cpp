// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>
#include "mmatrix.h"
#include "tstreamwriter.h"


#include "gschemedskinbuilder.h"
#include "gwindow.h"
#include "gbutton.h"

class IntroScene: public VScene
{
public:
	VTexture* TestTexture;
	VTexture* TestTGATexture;

	VVertexStream* TestMesh;

	float newX;
	float newY;
	float CurAng;

	void tstBut_Clicked()
	{
		MessageBoxA(0,"Hello!","Realizer",MB_OK);
	}

	void Render()
	{
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
		SceneName = L"Intro Scene";
		// Load resources here
		TestTexture = Engine.Textures.LoadTexture("test.bmp");
		TestTGATexture = Engine.Textures.LoadTexture("Acrylic 7/but_close.tga");


		GSchemedSkinBuilder gsb;
		gsb.Begin(1024,1024);
		gsb.LoadFromScheme("Acrylic 7/Acrylic 7.uis");
		GSchemedSkin* result = gsb.Finish();

		Engine.GUI.EnableGUI(result);

		GWindow* testWin = new GWindow();
		testWin->SetSize(100,100,300,200);

		GButton* testBut = new GButton();
		testBut->SetSize(50,50,100,27);
		testBut->Clicked += GetHandler(this, &IntroScene::tstBut_Clicked);
		

		Engine.GUI.Desktop->AddChild(testWin);
		testWin->AddChild(testBut);

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

str8 MatrixToString(mat4& src)
{
	str8 result(1024);
	result += str8::Format("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._11,src._12,src._13,src._14);
	result += str8::Format("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._21,src._22,src._23,src._24);
	result += str8::Format("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._31,src._32,src._33,src._34);
	result += str8::Format("%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n",src._41,src._42,src._43,src._44);
	return result;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	Engine.Renderer.InitializeRenderer(1280,720,L"Realizer3D",false,24);
	
	CTriggerAction* exitAction = Engine.Inputs.CreateAction("ExitEngine",&Engine.running);
	Engine.Inputs.CreateMappedKeyboard();
	Engine.Inputs.BindKey(Keys::Esc,exitAction);

	Engine.FileSystem.MountSystemFolder("../data/", TMount::Readable);
	Engine.FileSystem.MountSystemFolder("../save/", TMount::Writeable | TMount::Readable);

	IntroScene iScene;
	Engine.Scenes.ActivateScene(&iScene);

	int not = 0;
	not += 31;

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