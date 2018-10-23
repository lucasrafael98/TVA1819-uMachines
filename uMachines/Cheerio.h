#ifndef CHEERIO_H
#define CHEERIO_H
#include "GameElement.h"

class Cheerio :
	public GameElement
{
private:
	float _angle;
	float _velocity;
	float _brakeAcceleration;
	int _direction;
public:
	Cheerio(int id, float x, float y, float z, float *ambient, float *diffuse, float *specular, float *emissive, float shininess, int texcount, float angle, float velocity, float brakeAcceleration, int direction);
	~Cheerio();

	float getAngle() const { return _angle; }
	float getVelocity() const { return _velocity; }
	float getAcceleration() const { return _brakeAcceleration; }
	int getDirection() const { return _direction; }

	void setAngle(float angle) { _angle = angle; }
	void setVelocity(float velocity) { _velocity = velocity; }
	void setAcceleration(float brakeAcceleration) { _brakeAcceleration = brakeAcceleration; }
	void setDirection(int direction) { _direction = direction; }

};

#endif