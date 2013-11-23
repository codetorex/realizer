#ifndef VCAMERA_H
#define VCAMERA_H

#include "mvector3.h"
#include "mmatrix.h"

#include "tbinary.h"
#include "cinput.h"

class VCamera
{
public:
	/// Absolute camera position
	Point3 Position;
	
	/// Normalized direction vector
	Vector3 Direction;

	/// Normalized up vector
	Vector3 Up;

	/// Normalized right vector
	Vector3 Right;

	MMatrix4x4 View;

	/**
	 * Sets position and calculates direction vector
	 */
	inline void SetPositionLook(Point3& newPosition, Point3& newLook )
	{
		Position = newPosition;
		SetLook(newLook);
	}

	/**
	 * Calculates direction vector for given absolute look at position
	 */
	inline void SetLook(Point3& absoluteLook)
	{
		Direction = Position - absoluteLook;
		Direction.normalize();
		RecalculateRight();
	}

	inline void RecalculateRight()
	{
		Right = Direction.cross(Up);
	}

	void CalculateViewMatrix()
	{
		MViewMatrix* vm = (MViewMatrix*)&View;
		vm->LookAtDirectionR(Position,Direction,Up);
	}
};

/**
 * Simple fps fly cam controller by user input
 * NOTE: This should be singleton?
 */
class VCameraControllerFly
{
public:
	VCameraControllerFly();
	
	VCamera* Camera;

	int lastX, lastY;
	int xdiff, ydiff;

	CInputMode* FlyCamMode; // flycam input mode, enable this mode in Engine.Input to make this work
	TTriggerAction* Forward;
	TTriggerAction* Backward;
	TTriggerAction* StrafeLeft;
	TTriggerAction* StrafeRight;
	TTriggerAction* Ascend;
	TTriggerAction* Descend;
	TTriggerAction* Fast;
	TMoveAction* X;
	TMoveAction* Y;

	float NormalSpeed; // normalSPeed
	float FastSpeed; // can be controlled by shift as in GMOD?
	float SlowSpeed; // can be controlled by ctrl as in GMOD?

	/**
	 * This must be called every frame?
	 */
	void Update();
};

#endif