#include "Car.h"

//// wheels materials
//float amb_wheel[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//float diff_wheel[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//float spec_wheel[] = { 0.05f, 0.05f, 0.05f, 1.0f };
//float emissive_wheel[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//shininess = 70.0f;
//texcount = 0;
Car::Car(int id, float x, float y, float z, float *ambient, 
		float *diffuse, float *specular, float *emissive, 
		float shininess, int texcount, float angle, float velocity, 
		float acceleration, float brakeAcceleration, float maxVelocity):
		GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount) 
{
	_angle = angle;
	_velocity = velocity;
	_acceleration = acceleration;
	_brakeAcceleration = brakeAcceleration;
	_maxVelocity = maxVelocity;
	int i = 0;
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			_wheels[i] = new Wheel(id + 1, x*1.0f + 1.5f, 0.10f, y*1.2f + 1.0f, 
									{ 0.1f, 0.1f, 0.1f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f },
									{ 0.05f, 0.05f, 0.05f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f },
									70.0f, 0);
			i++;
		}
	}
	for (int i = -1; i <= 1; i += 2) {
		_headlights[i] = new Headlight(id + 2, 3.0f, 0.5f, i*0.5f + 0.8,
										{ 0.1f, 0.1f, 0.1f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
										{ 0.7f, 0.7f, 0.7f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 800.0f, 0);
		i++;
	}
}