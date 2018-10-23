#include "Car.h"

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
	float amb_wheel[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float dif_wheel[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec_wheel[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emi_wheel[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int x = -1; x <= 1; x += 2) {
		for (int y = -1; y <= 1; y += 2) {
			_wheels[i] = new Wheel(id + 1, x*1.0f + 1.5f, 0.10f, y*1.2f + 1.0f, amb_wheel, dif_wheel,spec_wheel, emi_wheel, 70.0f, 0);
			i++;
		}
	}
	float amb_head[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float dif_head[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec_head[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float emi_head[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	for (int i = -1; i <= 1; i += 2) {
		_headlights[i] = new Headlight(id + 2, 3.0f, 0.5f, i*0.5f + 0.8, amb_head, dif_head, spec_head, emi_head, 800.0f, 0);
		i++;
	}
}