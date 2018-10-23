#ifndef CANDLE_H
#define CANDLE_H
#include "GameElement.h"
#include "Flame.h"

class Candle :
	public GameElement
{
private:
	Flame* _flame;
public:
	Candle(int id, float x, float y, float z, float *ambient,
			float *diffuse, float *specular, float *emissive,
			float shininess, int texcount);
	~Candle();

	Flame* getFlame() { return _flame; }
};

#endif