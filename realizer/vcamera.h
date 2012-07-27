#ifndef VCAMERA_H
#define VCAMERA_H

#include "mvector3.h"
#include "mmatrix.h"

class VCamera
{
public:
	Vector3 Position;
	Vector3 Look;
	Vector3 Up;

	MMatrix4x4 View;

	void CalculateViewMatrix()
	{
		MViewMatrix* vm = (MViewMatrix*)&View;
		vm->LookAtR(Position,Look,Up);
	}
};

/**
 * Simple fps style fly cam
 */
class VCameraControllerFly
{
public:
	VCameraControllerFly();
	
	VCamera* Camera;


	void Forward(bool value);
	void Backward(bool value);
	void StrafeLeft(bool value);
	void StrafeRight(bool value);
	void Ascend(bool value);
	void Descend(bool value);

	void LookX(int value);
	void LookY(int value);

	/**
	 * This must be called everyframe?
	 */
	void Update();
};

#endif