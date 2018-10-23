#ifndef FLAME_H
#define FLAME_H
#include "GameElement.h"
class Flame :
	public GameElement
{
public:
	Flame(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~Flame();
};
#endif
