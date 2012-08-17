#include "stdafx.h"
#include "vcamera.h"
#include "cengine.h"

VCameraControllerFly::VCameraControllerFly()
{
	FlyCamMode  = Engine.Inputs.GetCreateMode		("FlyCamera");
	Forward     = FlyCamMode->GetCreateTriggerAction("Forward"    , Keys::W);
	StrafeLeft  = FlyCamMode->GetCreateTriggerAction("StrafeLeft" , Keys::A);
	Backward    = FlyCamMode->GetCreateTriggerAction("Backward"   , Keys::S);
	StrafeRight = FlyCamMode->GetCreateTriggerAction("StrafeRight", Keys::D);

	FlyCamMode->GetMouseMoveAction(X,"XAxis", Y, "YAxis");

	NormalSpeed = 10.0f;

	lastX = 500;
	lastY = 500;
}

void VCameraControllerFly::Update()
{
	D3DXMATRIX matRotation;

	POINT p;
	GetCursorPos(&p);

	xdiff = p.x - 500;
	ydiff = p.y - 500;

	SetCursorPos(500,500);

	if (ydiff != 0)
	{
		D3DXMatrixRotationAxis( &matRotation, (D3DXVECTOR3*)&Camera->Right, D3DXToRadian((float)ydiff / 6.0f));
		D3DXVec3TransformCoord( (D3DXVECTOR3*)&Camera->Direction, (D3DXVECTOR3*)&Camera->Direction, &matRotation );
		D3DXVec3TransformCoord( (D3DXVECTOR3*)&Camera->Up, (D3DXVECTOR3*)&Camera->Up, &matRotation );
		//Camera->RecalculateRight();
	}

	if (xdiff != 0)
	{
		D3DXMatrixRotationAxis( &matRotation, &D3DXVECTOR3(0,1,0), D3DXToRadian((float)-xdiff / 6.0f) );
		D3DXVec3TransformCoord( (D3DXVECTOR3*)&Camera->Direction, (D3DXVECTOR3*)&Camera->Direction, &matRotation );
		D3DXVec3TransformCoord( (D3DXVECTOR3*)&Camera->Up, (D3DXVECTOR3*)&Camera->Up, &matRotation );
		//Camera->RecalculateRight();
	}


	Camera->RecalculateRight();
	
	
	float curSpeed = NormalSpeed;
	float amount = curSpeed * Engine.Time.TimeDiff;
	

	if (Forward->Value)
	{
		Camera->Position -= Camera->Direction * (amount);
	}
	else if (Backward->Value)
	{
		Camera->Position += Camera->Direction * (amount);
	}

	if (StrafeLeft->Value)
	{
		Camera->Position += Camera->Right * (amount);
	}
	else if (StrafeRight->Value)
	{
		Camera->Position -= Camera->Right * (amount);
	}

	Camera->CalculateViewMatrix();
}



