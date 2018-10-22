#include "Candle.h"



Candle::Candle(int id, float x, float y, float z, float *ambient,
				float *diffuse, float *specular, float *emissive,
				float shininess, int texcount):
				GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount)
{
}


Candle::~Candle()
{
}
