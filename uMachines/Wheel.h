#ifndef WHEEL_H
#define WHEEL_H
#include "GameElement.h"
class Wheel :
	public GameElement
{
public:
	Wheel(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~Wheel();
};

#endif