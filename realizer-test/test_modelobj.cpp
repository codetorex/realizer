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
	Camera.Look.set(0,0,0);
	Camera.Up.set(0,1,0);
	Camera.CalculateViewMatrix();

	TestTexture = Engine.Textures.LoadTexture("test.bmp");

	fs = new TFileStream("c://outputMATRIX.txt",fm_Write);

	D3DXMATRIX persmat;
	D3DXMatrixPerspectiveFovRH(&persmat,D3DXToRadian(45.0f),1280.0f/720.0f, 0.1f,100.0f);
	ExportMatrix("PerspectiveFovR", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixPerspectiveFovLH(&persmat,D3DXToRadian(45.0f),1280.0f/720.0f, 0.1f,100.0f);
	ExportMatrix("PerspectiveFovL", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixOrthoLH(&persmat,1280.0f,720.0f,0.0f, 1.0f);
	ExportMatrix("OrthoL", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixOrthoRH(&persmat,1280.0f,720.0f,0.0f, 1.0f);
	ExportMatrix("OrthoR", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixOrthoOffCenterRH(&persmat,0.0f,1280.0f,720.0f, 0.0f,0.0f, 1.0f);
	ExportMatrix("OrthoOffCenterR", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixOrthoOffCenterLH(&persmat,0.0f,1280.0f,720.0f, 0.0f,0.0f, 1.0f);
	ExportMatrix("OrthoOffCenterL", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixPerspectiveLH(&persmat,1280.0f,720.0f,0.1f,100.f);
	ExportMatrix("PerspectiveL", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixPerspectiveRH(&persmat,1280.0f,720.0f,0.1f,100.f);
	ExportMatrix("PerspectiveR", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixPerspectiveOffCenterLH(&persmat,0.0f,1280.0f,720.0f,0.0f,0.1f, 100.0f);
	ExportMatrix("PerspectiveOffCenterL", persmat);

	D3DXMatrixIdentity(&persmat);
	D3DXMatrixPerspectiveOffCenterRH(&persmat,0.0f,1280.0f,720.0f,0.0f,0.1f, 100.0f);
	ExportMatrix("PerspectiveOffCenterR", persmat);

	D3DXMATRIX viewmat;
	D3DXVECTOR3 campos(30,30,30);
	D3DXVECTOR3 camto(0,0,0);
	D3DXVECTOR3 camup(0,1,0);

	D3DXMatrixLookAtRH(&viewmat,&campos,&camto,&camup);

	ExportMatrix("LookAtR", viewmat);

	D3DXMatrixIdentity(&viewmat);
	D3DXMatrixLookAtLH(&viewmat,&campos,&camto,&camup);

	ExportMatrix("LookAtL", viewmat);




	fs->Close();
	

	//LoadedObj->BaseMesh.SetMeshType(RL_LINELIST);
}

void RTestModelObj::Render()
{
	Engine.Renderer.SetDepthTest(true);

	MProjectionMatrix proj;
	proj.PerpectiveFovR( DegreeAngle(45.0f), 1280.0f / 720.0f, 0.1, 100.0f );

	Engine.Renderer.SetProjection(proj);
	Engine.Renderer.SetView(Camera.View);

	Engine.Renderer.D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_LINEARFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_LINEARFILTER);

	TestTexture->SetTexture();

	Engine.Renderer.RenderMesh(&LoadedObj->BaseMesh);

	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_POINTFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_POINTFILTER);

	MMatrix4x4 identity2;
	Engine.Renderer.SetView(identity2); // reset view matrix in case of?

	return;

	D3DXMATRIX persmat2;
	D3DXMatrixPerspectiveFovRH(&persmat2,D3DXToRadian(45.0f),1280.0f/720.0f, 0.1f,100.0f);

	D3DXMATRIX viewmat;
	D3DXVECTOR3 campos(30,30,30);
	D3DXVECTOR3 camto(0,0,0);
	D3DXVECTOR3 camup(0,1,0);

	D3DXMatrixLookAtRH(&viewmat,&campos,&camto,&camup);

	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);
	

	Engine.Renderer.SetProjection(*(MMatrix4x4*)&persmat2);
	Engine.Renderer.SetView(*(MMatrix4x4*)&viewmat);
	Engine.Renderer.SetWorld(*(MMatrix4x4*)&ident);

	Engine.Renderer.D3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Engine.Renderer.SetTextureFilter(0,RL_MAGFILTER,RL_LINEARFILTER);
	Engine.Renderer.SetTextureFilter(0,RL_MINFILTER,RL_LINEARFILTER);

	Engine.Renderer.SetDepthTest(true);

	TestTexture->SetTexture();
	
	Engine.Renderer.RenderMesh(&LoadedObj->BaseMesh);

	Engine.Renderer.SetView( MMatrix4x4() ); // reset view matrix in case of?

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

void RTestModelObj::ExportMatrix(const TString& exportName, MMatrix4x4& mat )
{
	TStringBuilderStack<512> sb;

	sb.Append("MMatrix4x4 % ( ",sfs(exportName));
	sb.Append("%f, ", sff(mat._11));
	sb.Append("%f, ", sff(mat._12));
	sb.Append("%f, ", sff(mat._13));
	sb.Append("%f, ", sff(mat._14));
	sb.Append("%f, ", sff(mat._21));
	sb.Append("%f, ", sff(mat._22));
	sb.Append("%f, ", sff(mat._23));
	sb.Append("%f, ", sff(mat._24));
	sb.Append("%f, ", sff(mat._31));
	sb.Append("%f, ", sff(mat._32));
	sb.Append("%f, ", sff(mat._33));
	sb.Append("%f, ", sff(mat._34));
	sb.Append("%f, ", sff(mat._41));
	sb.Append("%f, ", sff(mat._42));
	sb.Append("%f, ", sff(mat._43));
	sb.Append("%f );\r\n", sff(mat._44));

	fs->Write(sb.GetData(),1,sb.ByteLength);
}

void RTestModelObj::ExportMatrix( const TString& exportName, D3DXMATRIX& mat )
{
	ExportMatrix(exportName,*(MMatrix4x4*)&mat);
}