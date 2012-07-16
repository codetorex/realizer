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

	void CalculateViewMatrix();
	void SetRenderer();
};

#endif