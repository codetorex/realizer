#include "stdafx.h"
#include "test_modelobj.h"

#include "cengine.h"
#include "vmodelobj.h"
#include "vtexture.h"

#include "mangle.h"

#include "tfilestream.h"

RTestModelObj TestModelObj;

void RTestModelObj::Initialize()
{
	TStream* s = Engine.FileSystem.Open("first-test2.obj",fm_Read);
	VModelOBJ* objReader = new VModelOBJ();
	
	LoadedObj = new VModel();
	objReader->ReadModel(LoadedObj,s);

	Camera.Position.set(30,30,30);
	Camera.Up.set(0,1,0);
	Camera.SetLook(Point3(0,0,0));
	Camera.CalculateViewMatrix();

	TestTexture = Engine.Textures.LoadTexture("test.bmp");

	ProjMatrix.PerpectiveFovR( DegreeAngle(45.0f), 1280.0f / 720.0f, 0.1, 100.0f );

	Fly = new VCameraControllerFly();
	Fly->Camera = &Camera;
	Engine.Inputs.EnterMode(Fly->FlyCamMode);

}

void RTestModelObj::Render()
{
	Engine.Renderer.SetDepthTest(true);

	Engine.Renderer.SetProjection(ProjMatrix);
	Fly->Update();
	Engine.Renderer.SetView(Camera.View);

	Engine.Renderer.D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_LINEARFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_LINEARFILTER);

	TestTexture->SetTexture();

	Engine.Renderer.RenderMesh(&LoadedObj->BaseMesh);

	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_POINTFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_POINTFILTER);

	MMatrix4x4 identity;
	Engine.Renderer.SetView(identity); // reset view matrix in case of?
}

void RTestModelObj::DrawData()
{
	TStringBuilderStack<256> sb;
	sb.Append("X Diff: ");
	sb.Append(sfi(Fly->xdiff,-8));
	TestSuite.WriteTextRight(sb);

	sb.Clear();
	sb.Append("Y Diff: ");
	sb.Append(sfi(Fly->ydiff,-8));
	TestSuite.WriteTextRight(sb);
}