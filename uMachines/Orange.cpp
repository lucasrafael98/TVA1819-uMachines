#include "Orange.h"



Orange::Orange(int id, float x, float y, float z, float *ambient, 
				float *diffuse, float *specular, float *emissive, float shininess,
				int texcount, float angleX, float angleZ, float velocity):
				GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount)
{
	_angleX = angleX;
	_angleZ = angleZ;
	_velocity = velocity;
}


Orange::~Orange()
{
}
