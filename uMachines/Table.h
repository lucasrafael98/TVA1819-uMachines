#ifndef TABLE_H
#define TABLE_H
#include "GameElement.h"
class Table :
	public GameElement
{
public:
	Table(int id, float x, float y, float z, float *ambient,
		float *diffuse, float *specular, float *emissive,
		float shininess, int texcount);
	~Table();
};
#endif
