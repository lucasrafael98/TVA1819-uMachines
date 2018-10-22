#ifndef STEM_H
#define STEM_H
#include "GameElement.h"
class Stem :
	public GameElement
{
public:
	Stem(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~Stem();
};
#endif
