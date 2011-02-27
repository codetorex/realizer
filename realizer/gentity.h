#ifndef GENTITY_H
#define GENTITY_H

/*
After years of coding, I am creating this class today.
05.November.2010 09.07
*/

class GEntity
{
public:
	REngine* engine;

	vec3 position;
	vec3 rotation; // euler or something? pitch yaw roll and quaternion combined class needed

	virtual void Update(float time) = 0;
	virtual void Render() = 0;
};


#endif