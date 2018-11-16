#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL/glew.h>

class Particle
{
private:
	float	_life;		// lyf (lol)
	float	_fade;		// fade
	float	_r, _g, _b;    // color
	GLfloat _x, _y, _z;    // pos
	GLfloat _vx, _vy, _vz; // velocity 
	GLfloat _ax, _ay, _az; // acceleration

public:
	Particle(float life, float fade, float r, float g, float b, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az);
	~Particle();

	float getLife();
	float getFade();
	float getR();
	float getG();
	float getB();
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ();
	GLfloat getVelocX();
	GLfloat getVelocY();
	GLfloat getVelocZ();
	GLfloat getAccelX();
	GLfloat getAccelY();
	GLfloat getAccelZ();
	
	void setX(GLfloat x);
	void setY(GLfloat y);
	void setZ(GLfloat z);
	void setVelocX(GLfloat vx);
	void setVelocY(GLfloat vy);
	void setVelocZ(GLfloat vz);
	void setLife( float life );
};

#endif