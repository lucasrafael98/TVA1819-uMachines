#ifndef ORANGE_H
#define ORANGE_H
#include "GameElement.h"
#include "Stem.h"

class Orange :
	public GameElement
{
private:
	float _angleX;
	float _angleZ;
	float _velocity;
	Stem* _stem;
public:
	Orange(int id, float x, float y, float z, float *ambient, 
			float *diffuse, float *specular, float *emissive, float shininess, 
			int texcount, float angle, float velocity);
	~Orange();

	float getAngleX() const { return _angleX; }
	float getAngleZ() const { return _angleZ; }
	float getVelocity() const { return _velocity; }
	Stem* getStem() const { return _stem; }

	void setAngle(float angleX) { _angleX = angleX; }
	void setAngle(float angleZ) { _angleZ = angleZ; }
	void setVelocity(float velocity) { _velocity = velocity; }
};
#endif

