#include "stdafx.h"
#include "test_modelobj.h"

#include "cengine.h"
#include "vmodelobj.h"
#include "vtexture.h"

RTestModelObj TestModelObj;

void RTestModelObj::Initialize()
{
	TStream* s = Engine.FileSystem.Open("first-test2.obj",fm_Read);
	VModelOBJ* objReader = new VModelOBJ();
	
	LoadedObj = new VModel();
	objReader->ReadModel(LoadedObj,s);

	Camera.Position.set(20,20,20);
	Camera.Look.set(0,0,0);
	Camera.Up.set(0,1,0);
	Camera.CalculateViewMatrix();

	TestTexture = Engine.Textures.LoadTexture("test.bmp");

	

	//LoadedObj->BaseMesh.SetMeshType(RL_LINELIST);
}

void RTestModelObj::Render()
{

	D3DXMATRIX persmat;
	D3DXMatrixPerspectiveFovRH(&persmat,D3DXToRadian(45.0f),1280.0f/720.0f, 0.1f,100.0f);

	D3DXMATRIX viewmat;
	D3DXVECTOR3 campos(30,30,30);
	D3DXVECTOR3 camto(0,0,0);
	D3DXVECTOR3 camup(0,1,0);

	D3DXMatrixLookAtRH(&viewmat,&campos,&camto,&camup);

	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);
	

	Engine.Renderer.SetProjection(*(MMatrix4x4*)&persmat);
	Engine.Renderer.SetView(*(MMatrix4x4*)&viewmat);
	Engine.Renderer.SetWorld(*(MMatrix4x4*)&ident);

	Engine.Renderer.D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_LINEARFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_LINEARFILTER);

	Engine.Renderer.SetDepthTest(true);

	TestTexture->SetTexture();
	
	Engine.Renderer.RenderMesh(&LoadedObj->BaseMesh);

	MMatrix4x4 identity2;
	Engine.Renderer.SetView(identity2); // reset view matrix in case of?

	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_POINTFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_POINTFILTER);

	return;

	Engine.Renderer.D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*MProjectionMatrix mOrtho;
	mOrtho.PerspectiveR(160,90,0,100);
	Engine.Renderer.SetProjection(mOrtho);*/

	/*D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 1280.0f / 720.0f, 0.1f, 100.0f );
	Engine.Renderer.SetProjection(*(MMatrix4x4*)&matProj);*/

	Engine.Renderer.SetView(Camera.View);

	TestTexture->SetTexture();
	Engine.Renderer.RenderMesh(&LoadedObj->BaseMesh);

	Engine.Draw.NoTexture();
	Engine.Draw.DrawLine(0,0,50,50,TColors::Aqua);
	Engine.Draw.Flush();


	MMatrix4x4 identity;
	Engine.Renderer.SetView(identity); // reset view matrix in case of?
}