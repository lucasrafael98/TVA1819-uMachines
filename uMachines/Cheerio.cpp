#include "Cheerio.h"



Cheerio::Cheerio(int id, float x, float y, float z, float *ambient, 
				float *diffuse, float *specular, float *emissive, float shininess, 
				int texcount, float angle, float velocity, float brakeAcceleration, int direction):
				GameElement(id, x, y, z, ambient, diffuse, specular, emissive, shininess, texcount)
{
	_angle = angle;
	_velocity = velocity;
	_brakeAcceleration = brakeAcceleration;
	_direction = direction;
}


Cheerio::~Cheerio()
{
}
