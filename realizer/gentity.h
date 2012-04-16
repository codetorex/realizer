#ifndef GENTITY_H
#define GENTITY_H

/*
After years of coding, I am creating this class today.
05.November.2010 09.07
*/

class GEntity
{
public:
	vec3 Position;
	vec3 Rotation; // euler or something? pitch yaw roll and quaternion combined class needed

	virtual void Update(float time) = 0;
	virtual void Render() = 0;
};


#endif