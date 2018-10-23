#include "Candle.h"



Candle::Candle(int id, float x, float y, float z, float *ambient,
				float *diffuse, float *specular, float *emissive,
				float shininess, int texcount):
				GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount)
{
	float amb_flame[] = { 1.0f, 0.2f, 0.0f, 1.0f };
	float dif_flame[] = { 1.0f, 0.2f, 0.0f, 1.0f };
	float spec_flame[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float emi_flame[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	_flame = new Flame(id + 1,0.0f,1.3f,0.0f,amb_flame,dif_flame,spec_flame,emi_flame, 700.0f,0);
}


Candle::~Candle()
{
}
