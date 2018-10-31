#ifndef CARGLASS_H
#define CARGLASS_H
#include "GameElement.h"
class CarGlass :
	public GameElement
{
public:
	CarGlass(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~CarGlass();
};

#endif