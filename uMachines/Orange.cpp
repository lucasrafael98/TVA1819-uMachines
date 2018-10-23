#include "Orange.h"

Orange::Orange(int id, float x, float y, float z, float *ambient, 
				float *diffuse, float *specular, float *emissive, float shininess,
				int texcount, float angleX, float angleZ, float velocity):
				GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount)
{
	_angleX = angleX;
	_angleZ = angleZ;
	_velocity = velocity;
	float amb_stem[] = { 0.2f, 0.18f, 0.05f, 1.0f };
	float diff_stem[] = { 0.0f, 0.54f, 0.13f, 1.0f };
	float spec_stem[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	float emissive_stem[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_stem = new Stem(id + 1, 0.0f, 2.5f, 0.0f, amb_stem, diff_stem, spec_stem, emissive_stem,70.0f, 0);
}


Orange::~Orange()
{
}
