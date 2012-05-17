#ifndef GENTITY_H
#define GENTITY_H

#include "mvector3.h"

/*
After years of coding, I am creating this class today.
05.November.2010 09.07
*/

class GEntity
{
public:
	Vector3 Position;
	Vector3 Rotation; // euler or something? pitch yaw roll and quaternion combined class needed

	virtual void Update(float time) = 0;
	virtual void Render() = 0;
};


#endif