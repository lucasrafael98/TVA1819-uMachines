#include "Car.h"

Car::Car(int id, float x, float y, float z, float *ambient, 
		float *diffuse, float *specular, float *emissive, 
		float shininess, int texcount, float angle, float velocity, 
		float acceleration, float brakeAcceleration, float maxVelocity,
		float height):
		GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount) 
{
	_angle = angle;
	_velocity = velocity;
	_acceleration = acceleration;
	_brakeAcceleration = brakeAcceleration;
	_maxVelocity = maxVelocity;
	_height = height;
}