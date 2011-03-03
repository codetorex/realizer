// demo_realizer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <realizer.h>
#include "mmatrix.h"
#include "tstreamwriter.h"

class IntroScene: public VScene
{
public:
	VTexture* TestTexture;
	VVertexStream* TestMesh;

	void Render()
	{
		Engine.Renderer.Clear(RL_COLOR_BUFFER | RL_ZBUFFER);
		Engine.Renderer.BeginScene();

		mat4 trans;
		trans.Translate(100*sinf(GetTickCount()/100) + 100,100 * cosf(GetTickCount()/100) + 100,0);

		Engine.Renderer.Enter2D();

		Engine.Renderer.D3DDevice->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&trans);

		TestTexture->SetTexture();
		TestMesh->Render();

		Engine.Renderer.Exit2D();

		Engine.Renderer.EndScene();
		Engine.Renderer.SwapBackBuffer();
	}

	void Update()
	{
		// Update scene for animations here
	}

	void Initialize()
	{
		SceneName = L"Intro Scene";
		// Load resources here
		TestTexture = Engine.Textures.LoadTexture("test.bmp");

		TestMesh = new VVertexStream(VVertexBufferFormats::Textured1,4,RL_TRIANGLELIST);
		TestMesh->CreateVertexBuffer(4);
		TestMesh->LockBuffer();
		TestMesh->Add2DQuad1Tex(0,0,128,128,0.0f,0.0f,1.0f,1.0f);
		TestMesh->UnlockBuffer();
	}

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
	
	CTriggerAction* exitAction = (CTriggerAction*)Engine.Inputs.CreateAction("ExitEngine",&Engine.running);
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