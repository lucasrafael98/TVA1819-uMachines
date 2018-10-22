#ifndef HEADLIGHT_H
#define HEADLIGHT_H
#include "GameElement.h"
class Headlight :
	public GameElement
{
public:
	Headlight(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~Headlight();
};

#endif