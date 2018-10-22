#ifndef CANDLE_H
#define CANDLE_H
#include "GameElement.h"
class Candle :
	public GameElement
{
public:
	Candle(int id, float x, float y, float z, float *ambient,
			float *diffuse, float *specular, float *emissive,
			float shininess, int texcount);
	~Candle();
};

#endif